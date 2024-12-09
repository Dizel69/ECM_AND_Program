#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <csignal>
#include <atomic> // Для std::atomic
#include <algorithm> // Для std::remove

// Константы
const int PORT = 8000;
const int BUFFER_SIZE = 1024;

// Глобальные переменные
std::vector<int> client_sockets;  // Список сокетов подключённых клиентов
std::mutex client_mutex;          // Мьютекс для потокобезопасного доступа к клиентам
std::atomic<bool> server_running(true); // Флаг работы сервера

// Функция для обработки сообщений от клиента
void HandleClient(int client_socket) {
    char buffer[BUFFER_SIZE];
    while (server_running) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes_received <= 0) { // Ошибка или клиент закрыл соединение
            {
                std::lock_guard<std::mutex> lock(client_mutex);
                // Удаляем сокет клиента из списка
                client_sockets.erase(
                    std::remove(client_sockets.begin(), client_sockets.end(), client_socket),
                    client_sockets.end()
                );
            }
            close(client_socket);
            std::cout << "Клиент отключился. Сокет: " << client_socket << std::endl;
            break;
        }

        std::string message(buffer, bytes_received);
        std::cout << "Получено сообщение: " << message << std::endl;

        // Рассылка сообщения всем клиентам
        {
            std::lock_guard<std::mutex> lock(client_mutex);
            for (int sock : client_sockets) {
                if (sock != client_socket) {
                    send(sock, message.c_str(), message.size(), 0);
                }
            }
        }
    }
}

// Обработка сигнала SIGINT (Ctrl+C)
void SignalHandler(int signal) {
    std::cout << "\nОстановка сервера..." << std::endl;
    server_running = false;

    // Закрываем все клиентские соединения
    {
        std::lock_guard<std::mutex> lock(client_mutex);
        for (int sock : client_sockets) {
            close(sock);
        }
        client_sockets.clear();
    }

    std::exit(0);
}

int main() {
    // Устанавливаем обработчик сигнала
    std::signal(SIGINT, SignalHandler);

    // Создаём серверный сокет
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return 1;
    }

    // Настраиваем адрес и порт
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Привязываем сокет к адресу
    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Ошибка привязки сокета" << std::endl;
        close(server_socket);
        return 1;
    }

    // Начинаем прослушивание
    if (listen(server_socket, 10) == -1) {
        std::cerr << "Ошибка прослушивания" << std::endl;
        close(server_socket);
        return 1;
    }

    std::cout << "Сервер запущен на порту " << PORT << std::endl;

    // Главный цикл сервера
    while (server_running) {
        sockaddr_in client_addr;
        socklen_t client_size = sizeof(client_addr);

        // Принимаем подключение от клиента
        int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_size);
        if (client_socket == -1) {
            if (server_running) {
                std::cerr << "Ошибка принятия соединения" << std::endl;
            }
            continue;
        }

        {
            // Добавляем сокет клиента в общий список
            std::lock_guard<std::mutex> lock(client_mutex);
            client_sockets.push_back(client_socket);
        }

        std::cout << "Подключён новый клиент. Сокет: " << client_socket << std::endl;

        // Запускаем поток для обработки клиента
        std::thread client_thread(HandleClient, client_socket);
        client_thread.detach(); // Отделяем поток, чтобы он работал независимо
    }

    // Закрываем серверный сокет
    close(server_socket);
    return 0;
}
