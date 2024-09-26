#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "80"

int main(int argc, char** argv)
{
    int sockfd;
    struct addrinfo hints, *res, *p;
    int status;
    const char* sendbuf = "GET / HTTP/1.1\r\nHost: pmk.tversu.ru\r\nUser-Agent: Mozilla/5.0\r\nConnection: close\r\n\r\n";
    char recvbuf[DEFAULT_BUFLEN];
    int numbytes;

    if (argc != 2) {
        fprintf(stderr, "usage: %s server-name\n", argv[0]);
        return 1;
    }

    // Подготовка структуры hints
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // Получаем информацию об адресе
    if ((status = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    // Перебираем результаты и пытаемся подключиться к первому доступному
    for (p = res; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("connect");
            continue;
        }

        break; // Успешное подключение
    }

    if (p == NULL) {
        fprintf(stderr, "failed to connect\n");
        return 2;
    }

    freeaddrinfo(res); // Освобождаем список

    // Отправляем запрос
    if (send(sockfd, sendbuf, strlen(sendbuf), 0) == -1) {
        perror("send");
        close(sockfd);
        return 1;
    }

    // Получаем ответ
    while ((numbytes = recv(sockfd, recvbuf, DEFAULT_BUFLEN - 1, 0)) > 0) {
        recvbuf[numbytes] = '\0';
        printf("%s", recvbuf);
    }

    if (numbytes == -1) {
        perror("recv");
    }

    close(sockfd); // Закрываем соединение

    return 0;
}
