# Web UI Test Automation Framework

Автоматизированный фреймворк для тестирования графического интерфейса веб-приложений на основе Playwright и PyTest.

## Технологии

- **Playwright** — современная библиотека автоматизации браузера.
- **PyTest** — фреймворк для написания и выполнения тестов.
- **Page Object Model** — архитектурный паттерн для поддержки читаемости и переиспользования кода.
- **Python 3.9+**

## Установка

1. Клонировать репозиторий.
2. Создать виртуальное окружение:
   ```bash
   python -m venv venv
   source venv/bin/activate  # Linux/macOS
   venv\Scripts\activate     # Windows
   ```
3. Установить зависимости:
   ```bash
   pip install -r requirements.txt
   playwright install chromium firefox webkit
   ```
4. Настроить `.env` по образцу `.env.example`.

## Структура

- `tests/` — тестовые сценарии.
- `pages/` — классы страниц (Page Objects).
- `fixtures/` — тестовые данные.
- `utils/` — вспомогательные утилиты.
- `reports/` — отчёты и скриншоты.

## Запуск тестов

```bash
pytest
```

Дополнительные опции:
- `-m smoke` — запуск smoke-тестов.
- `--headed` — запуск с отображением браузера.
- `-n auto` — параллельный запуск (требуется `pytest-xdist`).

## Отчёты

После выполнения генерируется HTML-отчёт в `reports/report.html`.  
При настройке email-параметров в `.env` отчёт автоматически отправляется получателям.