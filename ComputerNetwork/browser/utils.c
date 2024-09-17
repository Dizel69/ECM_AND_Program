#include <string.h>
#include <stdlib.h>

// Функция для обработки URL и выделения хоста
char* parse_host_from_url(const char* url) {
    char* host;
    if (strncmp(url, "http://", 7) == 0) {
        host = strdup(url + 7);  // Убираем "http://"
    } else {
        host = strdup(url);      // Обрабатываем без "http://"
    }

    // Находим '/' после хоста, чтобы выделить домен
    char* slash = strchr(host, '/');
    if (slash) {
        *slash = '\0';  // Отделяем домен
    }

    return host;
}
