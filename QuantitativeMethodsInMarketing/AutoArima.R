# Загружаем необходимые пакеты
library(forecast)

data <- read.table("data.csv", header = TRUE, sep = "\n", dec = ",", row.names = NULL)
x <- as.numeric(data$Value)

# Преобразуем x в числовой вектор (если ещё не сделано)
x <- as.numeric(x)

# Проверяем, что данных достаточно
if (length(x) < 10) {
  stop("Ошибка: недостаточно данных для моделирования (менее 10 наблюдений).")
}

# Опционально: удаляем соседние повторяющиеся значения
x <- x[c(TRUE, diff(x) != 0)]

# Задаём диапазон кандидатских значений периода
candidate_periods <- 2:12  # Если данные не являются не сезонными, можно добавить и 1

# Создаем пустой data.frame для сохранения метрик и список для моделей
results_df <- data.frame(period = integer(), mape = numeric(), aic = numeric(), stringsAsFactors = FALSE)
models_list <- list()

# Перебираем все кандидаты по периоду
cat("Перебор возможных периодов:\n")
for (period in candidate_periods) {
  ts_data <- ts(x, frequency = period)
  
  # Подбор модели с помощью auto.arima
  model <- auto.arima(ts_data, stepwise = FALSE, approximation = FALSE)
  
  # Получаем подогнанные значения
  fitted_vals <- fitted(model)
  
  # Вычисляем MAPE
  mape_val <- mean(abs((ts_data - fitted_vals) / abs(ts_data)), na.rm = TRUE) * 100
  
  # Получаем AIC модели
  aic_val <- AIC(model)
  
  # Сохраняем результаты
  results_df <- rbind(results_df, data.frame(period = period, mape = mape_val, aic = aic_val))
  models_list[[as.character(period)]] <- model
  
  cat(sprintf("Период: %d, MAPE: %.2f%%, AIC: %.2f\n", period, mape_val, aic_val))
}

# Выводим таблицу кандидатов
cat("\nТаблица кандидатов по периоду:\n")
print(results_df)

# Выбираем период с минимальным MAPE (при равных можно дополнительно смотреть AIC)
best_row <- results_df[which.min(results_df$mape), ]
best_period <- best_row$period
cat(sprintf("\nВыбранный период: %d (MAPE = %.2f%%, AIC = %.2f)\n", best_period, best_row$mape, best_row$aic))

# Извлекаем модель с выбранным периодом
best_model <- models_list[[as.character(best_period)]]

# Строим прогноз на 12 периодов вперёд
forecast_h <- 12
forecast_model <- forecast(best_model, h = forecast_h)

# Построение графика прогноза: исходные данные + прогноз с доверительными интервалами
png("combined_plot.png", width = 800, height = 600)
plot(ts(x, frequency = best_period), type = "o", col = "black", 
     main = sprintf("Исходные данные и прогноз (период = %d)", best_period),
     xlab = "Время", ylab = "Значения",
     xlim = c(1, length(x) + forecast_h))
lines(forecast_model$mean, col = "red", lwd = 2)
# Если есть доверительные интервалы, добавляем их:
if (!is.null(forecast_model$lower) && !is.null(forecast_model$upper)) {
  lines(forecast_model$lower[,2], col = "red", lty = 2)
  lines(forecast_model$upper[,2], col = "red", lty = 2)
}
points((length(x) + 1):(length(x) + forecast_h), forecast_model$mean, col = "red", pch = 19)
grid()
dev.off()

cat("График данных и прогноза сохранён в 'combined_plot.png'.\n")

# Выводим информацию о модели и метриках
cat("\nПодобранная модель auto.arima:\n")
print(best_model)
cat("\nMAPE модели: ", sprintf("%.2f%%", best_row$mape), "\n")
cat("AIC модели: ", best_row$aic, "\n")
