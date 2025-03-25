# main.R
# Основной сценарий: считывание данных, подбор лучшей ARIMA модели, проверка её адекватности и построение графиков

# Подключаем функции из файлов:
source("ARIMA.R")         # Функция select_arima() и select_arima_candidates()
source("AdequacyCheck.R") # Функция run_adequacy_tests()

# 1. Считывание данных
data <- read.table("data.csv", header = TRUE, sep = "\n", dec = ",", row.names = NULL)
x <- data$Value
if (is.null(x)) {
  stop("Столбец 'Value' не найден в CSV-файле. Проверьте заголовки столбцов.")
}
x <- as.numeric(x)

if (length(x) < 10) {
  stop("Ошибка: недостаточно данных для моделирования (менее 10 наблюдений).")
}

# Предварительная обработка: удаляем соседние повторяющиеся значения
x_t <- x[c(TRUE, diff(x) != 0)]

# Подбор модели (один проход по кандидатам)
cat("Подбор модели ARIMA с сезонностью...\n")
candidates <- select_arima_candidates(x_t)
results <- select_arima(x_t)

# Проверка адекватности модели
adequacy <- run_adequacy_tests(results$model$residuals)
cat("Сводная таблица тестов:\n")
print(adequacy$summary)

if (adequacy$overall == "Модель адекватна") {
  cat("Адекватная модель найдена.\n")
  
  # Сохранение графика остатков модели в файл
  png("residuals_plot.png", width = 800, height = 600)
  plot(results$model$residuals, type = "o", col = "blue", 
       main = "Остатки адекватной модели", xlab = "Время", ylab = "Остатки")
  grid()
  dev.off()
  
  # Построение прогноза модели (на 12 периодов вперёд)
  forecast_model <- forecast(results$model, h = 12)
  
  # Сохранение комбинированного графика: исходные данные и прогноз на одном графике
  png("combined_plot.png", width = 800, height = 600)
  plot(x_t, type = "o", col = "black", 
       main = "Исходные данные и прогноз модели", xlab = "Время", ylab = "Значения",
       xlim = c(1, length(x_t) + 12))
  lines(forecast_model$mean, col = "red", lwd = 2)
  points((length(x_t) + 1):(length(x_t) + 12), forecast_model$mean, col = "red", pch = 19)
  grid()
  dev.off()
  
  cat("Графики сохранены: 'residuals_plot.png' и 'combined_plot.png'.\n")
} else {
  cat("Модель неадекватна.\n")
}
