#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// Определяем размер буфера для чтения данных
#define DEFAULT_BUFLEN 512

// Определяем порт по умолчанию (в данном случае HTTP-порт 80)
#define DEFAULT_PORT "80"

int main(int argc, char** argv)
{
    // Переменная для хранения дескриптора сокета
    int sockfd;

    // Структуры для хранения информации об адресах
    struct addrinfo hints, *res, *p;

    // Переменная для хранения статуса выполнения функций
    int status;

    // Строка с HTTP-запросом для отправки на сервер
    const char* sendbuf = "GET / HTTP/1.1\r\nHost: pmk.tversu.ru\r\nUser-Agent: Mozilla/5.0\r\nConnection: close\r\n\r\n";

    // Буфер для хранения данных, полученных от сервера
    char recvbuf[DEFAULT_BUFLEN];

    // Переменная для хранения количества полученных байт
    int numbytes;

    // Проверяем, что пользователь передал адрес сервера в командной строке
    if (argc != 2) {
        fprintf(stderr, "usage: %s server-name\n", argv[0]);
        return 1; // Завершаем программу с ошибкой
    }

    // Обнуляем структуру `hints`, чтобы задать параметры для функции `getaddrinfo`
    memset(&hints, 0, sizeof hints);

    // Указываем, что будем работать с IPv4 или IPv6
    hints.ai_family = AF_UNSPEC;

    // Указываем, что будем использовать потоковые сокеты (TCP)
    hints.ai_socktype = SOCK_STREAM;

    // Получаем информацию об адресе указанного сервера
    if ((status = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &res)) != 0) {
        // Если произошла ошибка, выводим сообщение и завершаем программу
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    // Перебираем все найденные адреса из списка `res`
    for (p = res; p != NULL; p = p->ai_next) {
        // Создаем сокет на основе текущего адреса
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            // Если не удалось создать сокет, выводим сообщение и продолжаем
            perror("socket");
            continue;
        }

        // Пытаемся подключиться к серверу по текущему адресу
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            // Если подключение не удалось, закрываем сокет и пробуем следующий адрес
            close(sockfd);
            perror("connect");
            continue;
        }

        // Если подключение успешно, выходим из цикла
        break;
    }

    // Если не удалось подключиться ни к одному адресу
    if (p == NULL) {
        fprintf(stderr, "failed to connect\n");
        return 2; // Завершаем программу с ошибкой
    }

    // Освобождаем память, выделенную для списка адресов
    freeaddrinfo(res);

    // Отправляем HTTP-запрос на сервер
    if (send(sockfd, sendbuf, strlen(sendbuf), 0) == -1) {
        // Если не удалось отправить данные, выводим сообщение и завершаем программу
        perror("send");
        close(sockfd);
        return 1;
    }

    // Цикл для получения ответа от сервера
    while ((numbytes = recv(sockfd, recvbuf, DEFAULT_BUFLEN - 1, 0)) > 0) {
        // Завершаем строку в буфере, чтобы корректно ее вывести
        recvbuf[numbytes] = '\0';

        // Выводим полученные данные на экран
        printf("%s", recvbuf);
    }

    // Если произошла ошибка при чтении данных
    if (numbytes == -1) {
        perror("recv");
    }

    // Закрываем сокет после завершения работы
    close(sockfd);

    // Возвращаем 0 для обозначения успешного завершения программы
    return 0;
}
