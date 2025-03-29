# main.R
# Основной сценарий: считывание данных, подбор лучшей ARIMA модели,
# проверка её адекватности и построение графиков (остатков и прогноза)

library(forecast)  # Для forecast()
# Подключаем функции из файлов:
source("ARIMA.R")         # Функции select_arima_candidates() и select_arima_from_candidates()
source("AdequacyCheck.R") # Функция run_adequacy_tests()

# Фиксирование времени начала работы
start_time <- Sys.time()

# Считывание данных
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
cat(x_t)
cat("Подбор модели ARIMA с сезонностью...\n")
# Получаем список кандидатов и выбираем лучшую модель, проходящую тесты адекватности
results <- select_arima_from_candidates(x_t, T_range = 1:1)

cat("\nВыбранная модель:\n")
cat(sprintf("order = (%d, %d, %d), seasonal = (%d, %d, %d, %d)\n",
            results$best_order[1], results$best_order[2], results$best_order[3],
            results$best_seasonal_order[1], results$best_seasonal_order[2],
            results$best_seasonal_order[3], results$best_seasonal_order[4]))
cat(sprintf("MAPE модели: %.2f%%\n\n", results$best_mape))

# Вывод сводной таблицы тестов адекватности
adequacy <- run_adequacy_tests(results$residuals)
cat("Сводная таблица тестов для выбранной модели:\n")
print(adequacy$summary)

# Сохранение графика остатков модели
png("residuals_plot.png", width = 800, height = 600)
plot(results$residuals, type = "o", col = "blue", 
     main = "Остатки выбранной модели", xlab = "Время", ylab = "Остатки")
grid()
dev.off()

  # Построение прогноза модели (на 12 периодов вперёд)
  forecast_model <- forecast(results$best_model, h = 12)
  
  # Сохранение комбинированного графика: исходные данные и прогноз на одном графике
  png("combined_plot.png", width = 800, height = 600)
  plot(x_t, type = "o", col = "#a00e94a1", 
       main = "Исходные данные и прогноз модели", xlab = "Время", ylab = "Значения",
       xlim = c(1, length(x_t) + 12))
  lines(forecast_model$mean, col = "red", lwd = 2)
  points((length(x_t) + 1):(length(x_t) + 12), forecast_model$mean, col = "red", pch = 19)
  grid()
  dev.off()
  
  cat("Графики сохранены: 'residuals_plot.png' и 'combined_plot.png'.\n")

if (is.null(forecast_model)) {
  cat("Прогноз не был построен из-за ошибки.\n")
} else {
  png("combined_plot.png", width = 800, height = 600)
  plot(x_t, type = "o", col = "black", 
       main = "Исходные данные и прогноз модели", xlab = "Время", ylab = "Значения",
       xlim = c(1, length(x_t) + 12))
  lines(forecast_model$mean, col = "red", lwd = 2)
  # Добавляем интервалы прогноза, если они есть
  if (!is.null(forecast_model$lower) && !is.null(forecast_model$upper)) {
    lines(forecast_model$lower[,2], col = "red", lty = 2)
    lines(forecast_model$upper[,2], col = "red", lty = 2)
  }
  points((length(x_t) + 1):(length(x_t) + 12), forecast_model$mean, col = "red", pch = 19)
  grid()
  dev.off()
  cat("Комбинированный график (данные + прогноз) сохранён в 'combined_plot.png'.\n")
}

# Фиксирование времени окончания работы и вывод общего времени выполнения
end_time <- Sys.time()
total_time <- end_time - start_time
cat("Общее время работы программы: ", total_time, "\n")
