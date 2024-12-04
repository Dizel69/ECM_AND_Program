#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

int main(int argc, char** argv)
{
    int ConnectSocket = -1;
    struct addrinfo* result = NULL, *ptr = NULL, hints;
    const char* sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    // Заполняем структуру hints для дальнейшего использования в getaddrinfo
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // AF_INET или AF_INET6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_protocol = IPPROTO_TCP; // TCP протокол

    // Определение адреса сервера и порта
    iResult = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(iResult));
        return 1;
    }

    // Пытаемся подключиться к одному из адресов
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Создаем сокет для подключения к серверу
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == -1) {
            perror("socket failed");
            continue;
        }

        // Подключаемся к серверу
        iResult = connect(ConnectSocket, ptr->ai_addr, ptr->ai_addrlen);
        if (iResult == -1) {
            close(ConnectSocket);
            ConnectSocket = -1;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == -1) {
        fprintf(stderr, "Unable to connect to server!\n");
        return 1;
    }

    // Отправляем данные
    iResult = send(ConnectSocket, sendbuf, strlen(sendbuf), 0);
    if (iResult == -1) {
        perror("send failed");
        close(ConnectSocket);
        return 1;
    }

    printf("Bytes Sent: %d\n", iResult);

    // Отключаем соединение для отправки
    iResult = shutdown(ConnectSocket, SHUT_WR);
    if (iResult == -1) {
        perror("shutdown failed");
        close(ConnectSocket);
        return 1;
    }

    // Принимаем данные от сервера
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
            printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            perror("recv failed");

    } while (iResult > 0);

    // Закрываем сокет
    close(ConnectSocket);

    return 0;
}