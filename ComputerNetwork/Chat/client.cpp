#include <iostream>
#include <thread>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

// Константы
const int PORT = 8000;
const int BUFFER_SIZE = 1024;

// Глобальная переменная для работы клиента
bool client_running = true;

// Функция для приёма сообщений от сервера
void ReceiveMessages(int client_socket) {
    char buffer[BUFFER_SIZE];
    while (client_running) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            std::cout << "Соединение с сервером разорвано." << std::endl;
            client_running = false;
            break;
        }
        std::cout << "Сервер: " << buffer << std::endl;
    }
}

int main() {
    // Создаём сокет
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return 1;
    }

    // Настраиваем адрес сервера
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Преобразуем адрес сервера из строки в бинарный формат
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        std::cerr << "Ошибка преобразования IP-адреса" << std::endl;
        close(client_socket);
        return 1;
    }

    // Подключаемся к серверу
    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Ошибка подключения к серверу" << std::endl;
        close(client_socket);
        return 1;
    }

    std::cout << "Подключено к серверу." << std::endl;

    // Запускаем поток для приёма сообщений
    std::thread receive_thread(ReceiveMessages, client_socket);

    // Основной цикл отправки сообщений
    std::string message;
    while (client_running) {
        std::getline(std::cin, message);
        if (message == "/exit") {
            client_running = false;
            break;
        }

        if (send(client_socket, message.c_str(), message.size(), 0) == -1) {
            std::cerr << "Ошибка отправки сообщения" << std::endl;
            client_running = false;
            break;
        }
    }

    // Ожидаем завершения потока приёма
    receive_thread.join();

    // Закрываем сокет
    close(client_socket);
    std::cout << "Клиент завершил работу." << std::endl;
    return 0;
}
