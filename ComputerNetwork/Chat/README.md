# Chat Server-Client

Проект реализует простую клиент-серверную архитектуру для общения через TCP-соединение. Сервер и клиент написаны на C++ и используют базовые библиотеки для работы с сокетами. В этом файле подробно описаны шаги по исправлению ошибок и функции, использованные в коде.

## Как запустить

### Шаг 1: Компиляция и запуск сервера
1. Перейдите в директорию с серверным кодом:

2. Компилируйте сервер:
    ```bash
    g++ server.cpp -o server
    ```
3. Убедитесь, что у исполняемого файла есть права на выполнение:
    ```bash
    chmod +x server
    ```
4. Запустите сервер:
    ```bash
    ./server
    ```

### Шаг 2: Компиляция и запуск клиента
1. Перейдите в директорию с клиентским кодом:

2. Компилируйте клиент:
    ```bash
    g++ -pthread client.cpp -o client
    ```
3. Убедитесь, что у исполняемого файла есть права на выполнение:
    ```bash
    chmod +x client
    ```
4. Запустите клиент:
    ```bash
    ./client
    ```

### Шаг 3: Завершение работы
- Чтобы завершить работу клиента, введите `/exit` в консоли или нажмите `Ctrl+C`.
- Сервер завершает свою работу автоматически при нажатии `Ctrl+C`.

---

## Исправление ошибок

1. **Ошибка доступа при запуске сервера и клиента:**
   - Решение: Использовали команду `chmod +x` для добавления прав на выполнение:
     ```bash
     chmod +x server
     chmod +x client
     ```

2. **Ошибка `namespace: команда не найдена` в клиенте:**
   - Причина: Попытка выполнить бинарный файл с ошибочной компиляцией.
   - Решение: Убедились, что клиент правильно компилируется с ключом `-pthread` для поддержки потоков:
     ```bash
     g++ -pthread client.cpp -o client
     ```

---

## Функции, используемые в коде

### Общие функции

#### `socket`
- **Описание:** Создаёт сокет для работы с сетью.
- **Синтаксис:** `int socket(int domain, int type, int protocol);`
- **Пример:** `int server_socket = socket(AF_INET, SOCK_STREAM, 0);`
- **Параметры:**
  - `AF_INET`: Использование IPv4.
  - `SOCK_STREAM`: Протокол TCP.
  - `0`: Использовать протокол по умолчанию.

#### `bind`
- **Описание:** Привязывает сокет к определённому адресу и порту.
- **Синтаксис:** `int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);`
- **Пример:** `bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));`

#### `listen`
- **Описание:** Переводит сокет в режим ожидания входящих соединений.
- **Синтаксис:** `int listen(int sockfd, int backlog);`
- **Пример:** `listen(server_socket, 5);`

#### `accept`
- **Описание:** Принимает входящее соединение от клиента.
- **Синтаксис:** `int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);`
- **Пример:** `int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);`

#### `connect`
- **Описание:** Устанавливает соединение с сервером.
- **Синтаксис:** `int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);`
- **Пример:** `connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr));`

#### `recv`
- **Описание:** Получает данные из сокета.
- **Синтаксис:** `ssize_t recv(int sockfd, void *buf, size_t len, int flags);`
- **Пример:** `recv(client_socket, buffer, BUFFER_SIZE - 1, 0);`

#### `send`
- **Описание:** Отправляет данные через сокет.
- **Синтаксис:** `ssize_t send(int sockfd, const void *buf, size_t len, int flags);`
- **Пример:** `send(client_socket, message.c_str(), message.size(), 0);`

#### `close`
- **Описание:** Закрывает сокет, освобождая ресурсы.
- **Синтаксис:** `int close(int fd);`
- **Пример:** `close(client_socket);`

---

### Функции из `<thread>`

#### `std::thread`
- **Описание:** Создаёт новый поток для выполнения функции.
- **Синтаксис:** `std::thread t(function, args...);`
- **Пример:** `std::thread receive_thread(ReceiveMessages, client_socket);`

#### `join`
- **Описание:** Ожидает завершения потока.
- **Синтаксис:** `t.join();`
- **Пример:** `receive_thread.join();`

---

### Функции из `<cstring>`

#### `memset`
- **Описание:** Заполняет блок памяти указанным значением.
- **Синтаксис:** `void* memset(void* str, int c, size_t n);`
- **Пример:** `memset(buffer, 0, BUFFER_SIZE);`

