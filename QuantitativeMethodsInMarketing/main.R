# main.R

# Подключаем функции из отдельных файлов:
source("ARIMA.R")         # файл с функцией select_arima()
source("AdequacyCheck.R") # файл с функцией run_adequacy_tests()

# 1. Считывание данных
data <- read.table("data.csv", header = TRUE, sep = "\n", dec = ",", row.names = NULL)
x <- data$Value
if (is.null(x)) {
  stop("Столбец 'Value' не найден в CSV-файле. Проверьте заголовки столбцов.")
}
x <- as.numeric(x)  # Преобразуем в числовой вектор

if (length(x) < 10) {
  stop("Ошибка: недостаточно данных для моделирования (менее 10 наблюдений).")
}

# Предварительная обработка: удаляем соседние повторяющиеся значения
x_t <- x[c(TRUE, diff(x) != 0)]

# Максимальное число попыток подбора модели
max_attempts <- 10
attempt <- 1
adequate_model_found <- FALSE
results <- NULL

while (!adequate_model_found && attempt <= max_attempts) {
  cat("Попытка модели №", attempt, "\n")
  
  # Подбор ARIMA модели и получение остатков
  results <- select_arima(x_t)
  
  # Проверка адекватности модели
  adequacy <- run_adequacy_tests(results$residuals)
  
  print(adequacy$summary)  # вывод сводной таблицы результатов тестов
  
  if (adequacy$overall == "Модель адекватна") {
    adequate_model_found <- TRUE
    cat("Адекватная модель найдена на попытке №", attempt, "\n")
    
    # Сохранение графика
    png("residuals_plot.png", width = 800, height = 600)
    plot(results$residuals, type = "o", col = "blue", 
         main = "Остатки адекватной модели", xlab = "Время", ylab = "Остатки")
    grid()
    dev.off()  # Закрываем устройство сохранения
    
  } else {
    cat("Модель неадекватна на попытке №", attempt, ". Переходим к следующей попытке...\n\n")
    attempt <- attempt + 1
  }
}

# Если адекватная модель найдена
if (adequate_model_found) {
  
  # Построение прогноза
  library(forecast)  # Убедись, что пакет установлен
  forecast_model <- forecast(results$best_model, h = 12)
  
  # Сохранение комбинированного графика
  png("combined_plot.png", width = 800, height = 600)
  
  # Все данные (точки) и прогноз на одном графике
  plot(x_t, type = "o", col = "black", 
       main = "Данные и Прогноз", xlab = "Время", ylab = "Значения", xlim = c(1, length(x_t) + 12))
  lines(forecast_model$mean, col = "red", lwd = 2)  # Прогноз
  points(length(x_t) + 1:length(forecast_model$mean), forecast_model$mean, col = "red", pch = 19)  # Прогнозные точки
  grid()
  
  dev.off()  # Закрываем устройство сохранения

  cat("Графики сохранены в файлы residuals_plot.png и combined_plot.png\n")
  
} else {
  cat("Не удалось найти адекватную модель за", max_attempts, "попыток.\n")
}
