#include <iostream>      // Для ввода/вывода
#include <thread>        // Для работы с потоками
#include <string>        // Для работы со строками
#include <cstring>       // Для работы с C-строками (например, memset)
#include <unistd.h>      // Для функций POSIX, таких как close()
#include <arpa/inet.h>   // Для работы с сокетами и IP-адресами

// Константы
const int PORT = 8000;        // Порт для подключения
const int BUFFER_SIZE = 1024; // Размер буфера для передачи данных

// Глобальная переменная для управления работой клиента
bool client_running = true;

// Функция для приёма сообщений от сервера
void ReceiveMessages(int client_socket) {
    char buffer[BUFFER_SIZE]; // Буфер для получаемых данных
    while (client_running) { // Цикл продолжается, пока клиент работает
        memset(buffer, 0, BUFFER_SIZE); // Очищаем буфер перед приёмом данных
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0); // Получаем данные от сервера
        if (bytes_received <= 0) { // Если сервер отключился или произошла ошибка
            std::cout << "Соединение с сервером разорвано." << std::endl;
            client_running = false; // Завершаем работу клиента
            break;
        }
        std::cout << "Сервер: " << buffer << std::endl; // Выводим полученное сообщение от сервера
    }
}

int main() {
    // Создаём сокет для подключения
    int client_socket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET - IPv4, SOCK_STREAM - TCP
    if (client_socket == -1) { // Проверяем, удалось ли создать сокет
        std::cerr << "Ошибка создания сокета" << std::endl;
        return 1; // Завершаем программу с кодом ошибки
    }

    // Настраиваем адрес сервера
    sockaddr_in server_addr;                 // Структура для адреса сервера
    server_addr.sin_family = AF_INET;        // Используем IPv4
    server_addr.sin_port = htons(PORT);      // Преобразуем порт в сетевой порядок байтов

    // Преобразуем IP-адрес сервера из строки в двоичный формат
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) { // Преобразуем "127.0.0.1" в формат IPv4
        std::cerr << "Ошибка преобразования IP-адреса" << std::endl;
        close(client_socket); // Закрываем сокет
        return 1;             // Завершаем программу с кодом ошибки
    }

    // Подключаемся к серверу
    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) { // Устанавливаем соединение с сервером
        std::cerr << "Ошибка подключения к серверу" << std::endl;
        close(client_socket); // Закрываем сокет
        return 1;             // Завершаем программу с кодом ошибки
    }

    std::cout << "Подключено к серверу." << std::endl; // Уведомляем пользователя об успешном подключении

    // Запускаем поток для приёма сообщений от сервера
    std::thread receive_thread(ReceiveMessages, client_socket);

    // Основной цикл для отправки сообщений
    std::string message; // Переменная для хранения сообщения от пользователя
    while (client_running) { // Цикл продолжается, пока клиент работает
        std::getline(std::cin, message); // Считываем сообщение из консоли
        if (message == "/exit") { // Если пользователь ввёл "/exit", завершаем работу
            client_running = false;
            break;
        }

        // Отправляем сообщение серверу
        if (send(client_socket, message.c_str(), message.size(), 0) == -1) { // Отправляем данные через сокет
            std::cerr << "Ошибка отправки сообщения" << std::endl;
            client_running = false; // Завершаем работу клиента в случае ошибки
            break;
        }
    }

    // Ожидаем завершения потока для приёма сообщений
    receive_thread.join();

    // Закрываем сокет
    close(client_socket); // Освобождаем ресурсы, связанные с сокетом
    std::cout << "Клиент завершил работу." << std::endl; // Уведомляем пользователя о завершении
    return 0; // Завершаем программу
}