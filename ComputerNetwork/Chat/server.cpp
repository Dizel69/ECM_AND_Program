#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <csignal>
#include <atomic>
#include <algorithm>
#include <sys/select.h>
#include <fcntl.h>

// Константы
const int PORT = 8000;
const int BUFFER_SIZE = 1024;

// Глобальные переменные
std::vector<int> client_sockets;  // Список сокетов подключённых клиентов
std::atomic<bool> server_running(true); // Флаг работы сервера

// Обработка сообщения от клиента
void HandleClientMessage(int client_socket) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

    if (bytes_received == 0) { // Клиент закрыл соединение
        std::cout << "Клиент отключился. Сокет: " << client_socket << std::endl;
        close(client_socket);
        client_sockets.erase(std::remove(client_sockets.begin(), client_sockets.end(), client_socket), client_sockets.end());
        return;
    }

    if (bytes_received < 0) { // Ошибка чтения
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            std::cerr << "Ошибка чтения данных с клиента: " << strerror(errno) << std::endl;
            close(client_socket);
            client_sockets.erase(std::remove(client_sockets.begin(), client_sockets.end(), client_socket), client_sockets.end());
        }
        return;
    }

    // Если получили сообщение
    std::string message(buffer, bytes_received);
    std::cout << "Получено сообщение от клиента " << client_socket << ": " << message << std::endl;

    // Рассылаем сообщение всем клиентам, кроме отправителя
    for (int sock : client_sockets) {
        if (sock != client_socket) {
            send(sock, message.c_str(), message.size(), 0);
        }
    }
}

// Обработка сигнала SIGINT (Ctrl+C)
void SignalHandler(int signal) {
    std::cout << "\nОстановка сервера..." << std::endl;
    server_running = false;

    // Закрываем все клиентские соединения
    for (int sock : client_sockets) {
        close(sock);
    }
    client_sockets.clear();

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

    // Настроим серверный сокет как неблокирующий
    fcntl(server_socket, F_SETFL, O_NONBLOCK);

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
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(server_socket, &read_fds);

        int max_fd = server_socket;
        for (int client_sock : client_sockets) {
            FD_SET(client_sock, &read_fds);
            if (client_sock > max_fd) {
                max_fd = client_sock;
            }
        }

        // Ожидаем активности на сокетах
        int activity = select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr);
        if (activity < 0) {
            std::cerr << "Ошибка select: " << strerror(errno) << std::endl;
            continue;
        }
        // Проверяем, есть ли активность на серверном сокете (новые подключения)
        if (FD_ISSET(server_socket, &read_fds)) {
            sockaddr_in client_addr;
            socklen_t client_size = sizeof(client_addr);
            int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_size);
            if (client_socket == -1) {
                std::cerr << "Ошибка принятия соединения: " << strerror(errno) << std::endl;
                continue;
            }

            // Делаем клиентский сокет нон-блокинг
            fcntl(client_socket, F_SETFL, O_NONBLOCK);

            // Добавляем новый сокет в список клиентов
            client_sockets.push_back(client_socket);

            std::cout << "Подключён новый клиент. Сокет: " << client_socket << std::endl;
        }

        // Проверяем активность на каждом клиентском сокете
        for (int client_socket : client_sockets) {
            if (FD_ISSET(client_socket, &read_fds)) {
                HandleClientMessage(client_socket);
            }
        }
    }

    // Закрываем серверный сокет
    close(server_socket);
    return 0;
}