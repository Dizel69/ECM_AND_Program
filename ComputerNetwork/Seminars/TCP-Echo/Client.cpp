#include <stdio.h>      // Библиотека для работы с вводом/выводом
#include <stdlib.h>     // Библиотека для управления памятью и работы с процессами
#include <string.h>     // Библиотека для работы со строками
#include <unistd.h>     // Библиотека для работы с системными вызовами (close(), read())
#include <arpa/inet.h>  // Библиотека для работы с интернет-протоколами (сокеты)

#define DEFAULT_BUFLEN 512     // Размер буфера для передачи данных
#define DEFAULT_PORT 27015     // Порт сервера, к которому будем подключаться

int main() {
    int client_sock;                    // Сокет для подключения к серверу
    struct sockaddr_in server_addr;     // Структура для хранения адреса сервера
    char buffer[DEFAULT_BUFLEN];        // Буфер для отправки и приёма данных
    int bytes_received;                 // Количество байт, полученных от сервера

    // Создаём сокет для клиента
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock == -1) { // Проверяем, успешно ли создан сокет
        perror("socket"); // Выводим сообщение об ошибке, если сокет не создан
        exit(EXIT_FAILURE); // Завершаем программу с кодом ошибки
    }

    // Заполняем структуру адреса сервера
    server_addr.sin_family = AF_INET;              // Используем IPv4
    server_addr.sin_port = htons(DEFAULT_PORT);    // Указываем порт сервера (в сетевом порядке байт)
    
    // Преобразуем IP-адрес сервера из строки в нужный формат и записываем в структуру
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton"); // Выводим сообщение об ошибке, если адрес не преобразован
        close(client_sock); // Закрываем сокет
        exit(EXIT_FAILURE); // Завершаем программу с кодом ошибки
    }

    // Подключаемся к серверу
    if (connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect"); // Выводим сообщение об ошибке, если подключение не удалось
        close(client_sock); // Закрываем сокет
        exit(EXIT_FAILURE); // Завершаем программу с кодом ошибки
    }

    printf("Connected to server.\n"); // Сообщаем, что успешно подключились к серверу

    // Основной цикл для отправки и приёма сообщений
    while (1) {
        printf("Enter message: "); // Просим пользователя ввести сообщение
        fgets(buffer, DEFAULT_BUFLEN, stdin); // Считываем строку, введённую пользователем
        buffer[strcspn(buffer, "\n")] = '\0'; // Убираем символ новой строки из конца сообщения

        // Отправляем сообщение серверу
        if (send(client_sock, buffer, strlen(buffer), 0) == -1) {
            perror("send"); // Выводим сообщение об ошибке, если отправка не удалась
            break; // Прерываем цикл
        }

        // Если пользователь ввёл "exit", завершаем соединение
        if (strcmp(buffer, "exit") == 0) {
            printf("Exiting...\n"); // Сообщаем о завершении работы клиента
            break; // Выходим из цикла
        }

        // Получаем ответ от сервера
        bytes_received = recv(client_sock, buffer, DEFAULT_BUFLEN, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0'; // Завершаем строку нулевым символом
            printf("Server: %s\n", buffer); // Выводим ответ сервера
        } else if (bytes_received == 0) {
            printf("Server disconnected.\n"); // Сообщаем, что сервер закрыл соединение
            break; // Выходим из цикла
        } else {
            perror("recv"); // Выводим сообщение об ошибке, если приём не удался
            break; // Прерываем цикл
        }
    }

    close(client_sock); // Закрываем сокет после завершения работы
    return 0; // Завершаем программу
}
