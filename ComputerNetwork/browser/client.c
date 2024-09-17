#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ncurses.h>
#include "utils.h"  // Подключаем наш заголовочный файл

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "80"

void display_header(WINDOW* win) {
    wattron(win, A_BOLD | COLOR_PAIR(1));
    mvwprintw(win, 1, 1, "Simple Text Browser");
    wattroff(win, A_BOLD | COLOR_PAIR(1));
    wrefresh(win);
}

void display_response(WINDOW* win, const char* response) {
    wclear(win);
    wattron(win, COLOR_PAIR(2));
    mvwprintw(win, 1, 1, "=== Ответ сервера ===");
    wattroff(win, COLOR_PAIR(2));

    int y = 3; // Начальная позиция для вывода ответа

    // Выводим построчно ответ
    const char* line = strtok(strdup(response), "\n");
    while (line != NULL) {
        if (y < LINES - 2) {
            mvwprintw(win, y++, 1, "%s", line);
        }
        line = strtok(NULL, "\n");
    }

    wattron(win, COLOR_PAIR(2));
    mvwprintw(win, y + 1, 1, "=== Конец ответа ===");
    wattroff(win, COLOR_PAIR(2));

    wrefresh(win);
}

int main() {
    int sockfd;
    struct addrinfo hints, *res, *p;
    int status;
    char url[256], *host, path[256] = "/";
    char sendbuf[512];
    char recvbuf[DEFAULT_BUFLEN];
    char response[4096] = ""; // Буфер для полного ответа
    int numbytes;

    // Инициализация ncurses
    initscr();
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);  // Цвет для заголовков
    init_pair(2, COLOR_GREEN, COLOR_BLACK); // Цвет для вывода ответа

    WINDOW* win = newwin(LINES - 2, COLS, 1, 0); // Окно для вывода

    // Заголовок
    display_header(win);

    // Запрашиваем URL у пользователя
    wattron(stdscr, A_BOLD);
    mvprintw(0, 0, " Введите URL ( например, http://pmk.tversu.ru/): ");
    wattroff(stdscr, A_BOLD);
    getstr(url);

    // Парсим хост из URL
    host = parse_host_from_url(url);

    // Подготовка структуры hints
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // Получаем информацию об адресе
    if ((status = getaddrinfo(host, DEFAULT_PORT, &hints, &res)) != 0) {
        mvprintw(3, 0, "getaddrinfo: %s", gai_strerror(status));
        free(host);
        endwin();
        return 2;
    }

    // Перебираем результаты и пытаемся подключиться к первому доступному
    for (p = res; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("connect");
            continue;
        }

        break; // Успешное подключение
    }

    if (p == NULL) {
        mvprintw(3, 0, "Не удалось подключиться к серверу");
        freeaddrinfo(res);
        free(host);
        endwin();
        return 2;
    }

    freeaddrinfo(res); // Освобождаем список

    // Формируем HTTP-запрос
    snprintf(sendbuf, sizeof(sendbuf),
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n"
             "User-Agent: SimpleBrowser/1.0\r\n"
             "\r\n", path, host);

    free(host);  // Хост больше не нужен

    // Отправляем запрос
    if (send(sockfd, sendbuf, strlen(sendbuf), 0) == -1) {
        perror("send");
        close(sockfd);
        endwin();
        return 1;
    }

    // Получаем ответ и сохраняем его в буфер
    while ((numbytes = recv(sockfd, recvbuf, DEFAULT_BUFLEN - 1, 0)) > 0) {
        recvbuf[numbytes] = '\0';
        strcat(response, recvbuf);  // Копируем в общий буфер
    }

    if (numbytes == -1) {
        perror("recv");
    }

    // Выводим ответ в интерфейсе
    display_response(win, response);

    // Ожидаем нажатия клавиши перед завершением
    mvprintw(LINES - 1, 0, "Нажмите любую клавишу для выхода...");
    refresh();
    getch();

    // Закрываем окно ncurses
    close(sockfd);
    endwin();

    return 0;
}
