# main.R
# Основной сценарий: считывание данных, подбор лучшей ARIMA модели,
# проверка её адекватности и построение графиков (остатков и прогноза)
library(forecast)
# Подключаем функции из отдельных файлов:
source("ARIMA.R")         # функции select_arima_candidates() и select_arima()
source("AdequacyCheck.R") # функция run_adequacy_tests()

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

# Подбор кандидатов и выбор лучшей модели (с проверкой адекватности)
cat("Подбор модели ARIMA с сезонностью...\n")
candidates <- select_arima_candidates(x_t)
cat("Топ-5 кандидатов (по MAPE, затем AIC):\n")
print(head(candidates$candidate_info, 5))

cat("\nПроверка кандидатов на адекватность...\n")
best_candidate <- NULL
for (idx in 1:nrow(candidates$candidate_info)) {
  model_candidate <- candidates$models[[idx]]
  resids <- residuals(model_candidate)
  adequacy <- run_adequacy_tests(resids)
  cat(sprintf("Проверка кандидата %d из %d:\n", idx, nrow(candidates$candidate_info)))
  cat(adequacy$summary, "\n")
  if (adequacy$overall == "Модель адекватна") {
    cat("Кандидат ", idx, " прошёл проверку адекватности.\n", sep = "")
    best_candidate <- list(best_model = model_candidate, 
                           best_order = as.numeric(candidates$candidate_info[idx, c("p", "d", "q")]),
                           best_seasonal_order = as.numeric(candidates$candidate_info[idx, c("P", "D", "Q", "T")]),
                           best_mape = candidates$candidate_info$mape[idx],
                           residuals = resids)
    break
  } else {
    cat("Кандидат ", idx, " не прошёл проверку адекватности.\n\n")
  }
}
if (is.null(best_candidate)) {
  cat("Ни одна модель не прошла тесты адекватности, возвращается лучшая по MAPE.\n")
  best_candidate <- list(best_model = candidates$models[[1]], 
                         best_order = as.numeric(candidates$candidate_info[1, c("p", "d", "q")]),
                         best_seasonal_order = as.numeric(candidates$candidate_info[1, c("P", "D", "Q", "T")]),
                         best_mape = candidates$candidate_info$mape[1],
                         residuals = residuals(candidates$models[[1]]))
}

cat("\nВыбранная модель:\n")
cat(sprintf("order = (%d, %d, %d), seasonal = (%d, %d, %d, %d)\n",
            best_candidate$best_order[1], best_candidate$best_order[2], best_candidate$best_order[3],
            best_candidate$best_seasonal_order[1], best_candidate$best_seasonal_order[2],
            best_candidate$best_seasonal_order[3], best_candidate$best_seasonal_order[4]))
cat(sprintf("MAPE модели: %.2f%%\n\n", best_candidate$best_mape))

# Сохранение графика остатков модели
png("residuals_plot.png", width = 800, height = 600)
plot(best_candidate$residuals, type = "o", col = "blue", 
     main = "Остатки выбранной модели", xlab = "Время", ylab = "Остатки")
grid()
dev.off()
cat("График остатков сохранён в 'residuals_plot.png'.\n")

# Построение прогноза модели (на 12 периодов вперёд) и комбинированного графикаvf
forecast_model <- tryCatch({
  forecast(best_candidate$best_model, h = 12)
}, error = function(e) {
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
  lines(forecast_model$lower[,2], col = "red", lty = 2)
  lines(forecast_model$upper[,2], col = "red", lty = 2)
  points((length(x_t) + 1):(length(x_t) + 12), forecast_model$mean, col = "red", pch = 19)
  grid()
  dev.off()
  cat("Комбинированный график (данные + прогноз) сохранён в 'combined_plot.png'.\n")
}

cat("Отладка завершена.\n")
