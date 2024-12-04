#include <stdio.h>      // Библиотека для работы с вводом/выводом
#include <stdlib.h>     // Библиотека для управления памятью, работы с процессами
#include <string.h>     // Библиотека для работы со строками
#include <unistd.h>     // Библиотека для работы с системными вызовами (close(), read())
#include <pthread.h>    // Библиотека для работы с потоками
#include <arpa/inet.h>  // Библиотека для работы с интернет-протоколами (сокеты)

#define DEFAULT_BUFLEN 512     // Размер буфера для передачи данных
#define DEFAULT_PORT 27015     // Порт, который будет слушать сервер

// Функция для обработки подключения клиента
void* handle_client(void* client_socket) {
    int client_sock = *(int*)client_socket; // Получаем сокет клиента из аргумента
    free(client_socket); // Освобождаем память, выделенную для передачи сокета в поток

    char buffer[DEFAULT_BUFLEN]; // Буфер для приёма данных от клиента
    int bytes_received; // Количество байт, полученных от клиента

    printf("New client connected: %d\n", client_sock); // Сообщаем о новом подключении

    // Бесконечный цикл для обработки данных от клиента
    while ((bytes_received = recv(client_sock, buffer, DEFAULT_BUFLEN, 0)) > 0) {
        buffer[bytes_received] = '\0'; // Завершаем строку нулевым символом
        printf("Client %d sent: %s\n", client_sock, buffer); // Выводим данные от клиента

        // Отправляем клиенту то же сообщение (эхо-сообщение)
        send(client_sock, buffer, bytes_received, 0);
    }

    // Если клиент завершил соединение
    if (bytes_received == 0) {
        printf("Client %d disconnected.\n", client_sock); // Клиент отключился
    } else {
        perror("recv"); // Обрабатываем ошибку приёма данных
    }

    close(client_sock); // Закрываем сокет клиента
    return NULL; // Завершаем поток
}

int main() {
    int server_sock, client_sock; // Сокеты для сервера и клиента
    struct sockaddr_in server_addr, client_addr; // Структуры для адресов сервера и клиента
    socklen_t client_addr_size = sizeof(client_addr); // Размер структуры адреса клиента

    // Создаём сокет для сервера
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) { // Проверяем, создался ли сокет
        perror("socket"); // Выводим ошибку, если сокет не создался
        exit(EXIT_FAILURE); // Завершаем программу с ошибкой
    }

    // Заполняем структуру адреса сервера
    server_addr.sin_family = AF_INET; // Указываем, что используется IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Сервер будет слушать на всех доступных интерфейсах
    server_addr.sin_port = htons(DEFAULT_PORT); // Указываем порт, преобразованный в сетевой порядок байт

    // Привязываем сокет к указанному адресу и порту
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind"); // Выводим ошибку, если привязка не удалась
        close(server_sock); // Закрываем сокет
        exit(EXIT_FAILURE); // Завершаем программу с ошибкой
    }

    // Устанавливаем сокет в режим прослушивания входящих подключений
    if (listen(server_sock, SOMAXCONN) == -1) {
        perror("listen"); // Выводим ошибку, если не удалось включить прослушивание
        close(server_sock); // Закрываем сокет
        exit(EXIT_FAILURE); // Завершаем программу с ошибкой
    }

    printf("Server listening on port %d\n", DEFAULT_PORT); // Уведомляем, что сервер запущен

    // Основной цикл для приёма и обработки подключений
    while (1) {
        // Принимаем подключение от клиента
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_size);
        if (client_sock == -1) { // Проверяем, успешно ли принято подключение
            perror("accept"); // Выводим ошибку, если подключение не удалось
            continue; // Переходим к следующей итерации, не завершая сервер
        }

        // Выделяем память для передачи сокета в поток
        int* new_sock = (int*)malloc(sizeof(int));
        *new_sock = client_sock;

        // Создаём поток для обработки подключения клиента
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handle_client, (void*)new_sock) != 0) {
            perror("pthread_create"); // Выводим ошибку, если поток не создался
            close(client_sock); // Закрываем сокет клиента
            free(new_sock); // Освобождаем память
        } else {
            pthread_detach(client_thread); // Отсоединяем поток, чтобы он завершался самостоятельно
        }
    }

    close(server_sock); // Закрываем серверный сокет (эта строка никогда не выполнится в текущем коде)
    return 0; // Завершаем программу
}
