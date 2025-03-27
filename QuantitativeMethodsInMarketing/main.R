# main.R
# Основной сценарий: считывание данных, подбор лучшей ARIMA модели,
# проверка её адекватности и построение графиков (остатков и прогноза)

# Подключаем функции из файлов:
source("ARIMA.R")         # Функции select_arima_candidates() и select_arima_from_candidates()
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

cat("Подбор кандидатов для модели ARIMA с сезонностью...\n")
candidates <- select_arima_candidates(x_t)
cat("Топ-5 кандидатов (по MAPE, затем AIC):\n")
print(head(candidates$candidate_info, 5))

cat("\nПроверка кандидатов на адекватность...\n")
best_candidate <- select_arima_from_candidates(x_t)

cat("\nВыбранная модель:\n")
cat(sprintf("order = (%d, %d, %d), seasonal = (%d, %d, %d, %d)\n",
            best_candidate$best_order[1], best_candidate$best_order[2], best_candidate$best_order[3],
            best_candidate$best_seasonal_order[1], best_candidate$best_seasonal_order[2],
            best_candidate$best_seasonal_order[3], best_candidate$best_seasonal_order[4]))
cat(sprintf("MAPE модели: %.2f%%\n\n", best_candidate$best_mape))

# Сохранение графика остатков выбранной модели
png("residuals_plot.png", width = 800, height = 600)
plot(best_candidate$residuals, type = "o", col = "blue", 
     main = "Остатки выбранной модели", xlab = "Время", ylab = "Остатки")
grid()
dev.off()
cat("График остатков сохранён в 'residuals_plot.png'.\n")

# Построение прогноза (на 12 периодов вперёд) и комбинированного графика
library(forecast)
forecast_model <- tryCatch(forecast(best_candidate$best_model, h = 12),
                             error = function(e) { 
                               cat("Ошибка при построении прогноза: ", e$message, "\n")
                               return(NULL)
                             })
if (is.null(forecast_model)) {
  cat("Прогноз не был построен из-за ошибки.\n")
} else {
  png("combined_plot.png", width = 800, height = 600)
  plot(x_t, type = "o", col = "black", 
       main = "Исходные данные и прогноз модели", xlab = "Время", ylab = "Значения",
       xlim = c(1, length(x_t) + 12))
  lines(forecast_model$mean, col = "red", lwd = 2)
  points((length(x_t) + 1):(length(x_t) + 12), forecast_model$mean, col = "red", pch = 19)
  grid()
  dev.off()
  cat("Комбинированный график (данные и прогноз) сохранён в 'combined_plot.png'.\n")
}

cat("Отладка завершена.\n")
