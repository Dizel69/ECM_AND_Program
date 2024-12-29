#include <iostream>              // Для работы с консольным вводом/выводом
#include <string>                // Для использования std::string
#include <cstring>               // Для работы с C-строками
#include <arpa/inet.h>           // Для работы с сетевыми функциями
#include <sys/socket.h>          // Для работы с сокетами
#include <unistd.h>              // Для функции close()

int main() {
    // Создаём UDP-сокет
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl; // Вывод ошибки при создании сокета
        return 1; // Завершаем программу
    }

    // Настраиваем адрес сервера, к которому будем отправлять сообщения
    struct sockaddr_in server_addr {};
    server_addr.sin_family = AF_INET;       // IPv4
    server_addr.sin_port = htons(8080);    // Порт сервера

    // Преобразуем строковый IP-адрес в формат, подходящий для работы с сетью
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) { // Локальный сервер (localhost)
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        close(client_socket); // Закрываем сокет
        return 1; // Завершаем программу
    }

    std::cout << "Enter your message (type 'exit' to quit):" << std::endl;

    while (true) {
        std::string message;
        std::cout << "> ";
        std::getline(std::cin, message); // Считываем сообщение от пользователя

        if (message == "exit") { // Если пользователь ввёл "exit", завершаем программу
            break;
        }

        // Отправляем сообщение серверу
        ssize_t sent_bytes = sendto(client_socket, message.c_str(), message.size(), 0,
                                    reinterpret_cast<const struct sockaddr *>(&server_addr), sizeof(server_addr));
        if (sent_bytes < 0) {
            std::cerr << "Failed to send message: " << strerror(errno) << std::endl; // Выводим ошибку при отправке
            continue;
        }

        // Буфер для получения ответа от сервера
        char buffer[1024];
        struct sockaddr_in from_addr {};
        socklen_t from_len = sizeof(from_addr);

        // Получаем ответ от сервера
        ssize_t received_bytes = recvfrom(client_socket, buffer, sizeof(buffer) - 1, 0,
                                          reinterpret_cast<struct sockaddr *>(&from_addr), &from_len);
        if (received_bytes < 0) {
            std::cerr << "Failed to receive response: " << strerror(errno) << std::endl; // Вывод ошибки при получении
            continue;
        }

        buffer[received_bytes] = '\0'; // Завершаем строку символом '\0'
        std::cout << "Server response: " << buffer << std::endl; // Выводим ответ сервера
    }

    // Закрываем сокет
    close(client_socket);
    return 0;
}
