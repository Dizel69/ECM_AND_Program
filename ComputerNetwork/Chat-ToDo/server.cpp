#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <iostream>
#include <chrono>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 27015

void handleClient(int ClientSocket, char* recvbuf, int recvbuflen);

int main(void)
{
    int ListenSocket = -1;
    int ClientSocket = -1;

    struct sockaddr_in server_addr;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Создаем сокет
    ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ListenSocket == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Заполняем информацию о сервере
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(DEFAULT_PORT);

    // Привязываем сокет
    if (bind(ListenSocket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        close(ListenSocket);
        exit(EXIT_FAILURE);
    }

    // Начинаем прослушивание
    if (listen(ListenSocket, SOMAXCONN) == -1) {
        perror("listen failed");
        close(ListenSocket);
        exit(EXIT_FAILURE);
    }

    while (true) {
        std::cout << "Waiting for the connection\n";
        // Принимаем соединение
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == -1) {
            perror("accept failed");
            close(ListenSocket);
            exit(EXIT_FAILURE);
        }
        std::cout << "Connected!\n";

        std::thread clientThread(handleClient, ClientSocket, recvbuf, recvbuflen);
        clientThread.detach();
    }

    close(ListenSocket);
    return 0;
}

void handleClient(int ClientSocket, char* recvbuf, int recvbuflen) {
    int iResult;
    int iSendResult;

    do {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

            // Отправляем данные обратно клиенту
            iSendResult = send(ClientSocket, recvbuf, iResult, 0);
            if (iSendResult == -1) {
                perror("send failed");
                close(ClientSocket);
                return;
            }
            printf("Bytes sent: %d\n", iSendResult);
        } else if (iResult == 0) {
            printf("Connection closed\n");
        } else {
            perror("recv failed");
            close(ClientSocket);
            return;
        }

    } while (iResult > 0);

    // Закрываем соединение
    if (shutdown(ClientSocket, SHUT_WR) == -1) {
        perror("shutdown failed");
    }
    close(ClientSocket);
}