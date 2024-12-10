#include <iostream>              // Для работы с консольным выводом
#include <string>                // Для использования std::string
#include <thread>                // Для работы с потоками
#include <vector>                // Для хранения потоков в векторе
#include <queue>                 // Для очереди задач
#include <mutex>                 // Для блокировки потоков (мьютексы)
#include <condition_variable>    // Для синхронизации потоков
#include <functional>            // Для std::function и std::bind
#include <cstring>               // Для работы с C-строками
#include <arpa/inet.h>           // Для работы с сетевыми функциями (IPv4)
#include <sys/socket.h>          // Для работы с сокетами
#include <unistd.h>              // Для функции close()

constexpr int THREAD_POOL_SIZE = 10; // Количество потоков в пуле (можно изменить)

class ThreadPool {
public:
    // Конструктор. Создаём потоки в количестве num_threads
    explicit ThreadPool(size_t num_threads) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back([this] { // Лямбда-функция для каждого потока
                while (true) {
                    std::function<void()> task; // Переменная для хранения задачи

                    // Забираем задачу из очереди или ждём, если её нет
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        condition.wait(lock, [this] { return !tasks.empty() || stop; });

                        // Если сервер завершает работу и задачи закончились, выходим из цикла
                        if (stop && tasks.empty())
                            return;

                        // Берём первую задачу из очереди
                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    // Выполняем задачу
                    task();
                }
            });
        }
    }

    // Деструктор. Завершаем все потоки
    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true; // Устанавливаем флаг завершения работы
        }

        condition.notify_all(); // Уведомляем все потоки
        for (std::thread &worker : workers) {
            worker.join(); // Дожидаемся завершения каждого потока
        }
    }

    // Добавление новой задачи в пул потоков
    template<typename F, typename... Args>
    void NewTask(F &&f, Args &&... args) {
        // Создаём задачу с помощью std::bind
        auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace(task); // Добавляем задачу в очередь
        }

        condition.notify_one(); // Уведомляем один поток о наличии новой задачи
    }

private:
    std::vector<std::thread> workers;        // Вектор потоков
    std::queue<std::function<void()>> tasks; // Очередь задач
    std::mutex queue_mutex;                  // Мьютекс для защиты доступа к очереди
    std::condition_variable condition;       // Условная переменная для синхронизации
    bool stop = false;                       // Флаг завершения работы
};

// Функция, которая отвечает за отправку сообщения обратно клиенту
void Echo(int socket_fd, const std::string &message, const struct sockaddr_in &to) {
    std::cout << "Echoing: " << message << std::endl; // Выводим сообщение в консоль
    sendto(socket_fd, message.c_str(), message.size(), 0,
           reinterpret_cast<const struct sockaddr *>(&to), sizeof(to)); // Отправляем сообщение клиенту
}

int main() {
    // Создаём UDP-сокет
    int listen_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (listen_socket < 0) {
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl; // Выводим ошибку, если не удалось создать сокет
        return 1; // Завершаем работу программы
    }

    // Настраиваем адрес и порт для прослушивания
    struct sockaddr_in server_addr {};
    server_addr.sin_family = AF_INET;       // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Принимаем данные с любого IP
    server_addr.sin_port = htons(8080);    // Привязываем к порту 8080

    // Привязываем сокет к указанному адресу и порту
    if (bind(listen_socket, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr)) < 0) {
        std::cerr << "Failed to bind socket: " << strerror(errno) << std::endl; // Выводим ошибку, если не удалось привязать сокет
        close(listen_socket); // Закрываем сокет
        return 1; // Завершаем работу программы
    }

    // Выводим сообщение о том, что сервер запущен
    std::cout << "UDP Echo Server is running on port 8080..." << std::endl;

    // Создаём пул потоков
    ThreadPool pool(THREAD_POOL_SIZE);

    while (true) {
        // Буфер для приёма данных
        char buffer[1024];
        struct sockaddr_in client_addr {}; // Структура для хранения адреса клиента
        socklen_t client_addr_len = sizeof(client_addr);

        // Получаем данные от клиента
        ssize_t bytes_received = recvfrom(listen_socket, buffer, sizeof(buffer) - 1, 0,
                                          reinterpret_cast<struct sockaddr *>(&client_addr), &client_addr_len);

        if (bytes_received < 0) {
            std::cerr << "Failed to receive data: " << strerror(errno) << std::endl; // Выводим ошибку, если не удалось получить данные
            continue;
        }

        buffer[bytes_received] = '\0'; // Завершаем строку символом '\0'

        // Добавляем задачу в пул потоков
        pool.NewTask(Echo, listen_socket, std::string(buffer), client_addr);
    }

    // Закрываем сокет (этот код не будет выполнен, пока сервер работает)
    close(listen_socket);
    return 0;
}
