# ================================================================
# Программа для подбора лучшей ARIMA модели и вывода её остатков
# ================================================================

# Входной временной ряд
x_t <- c(1, 4, 7, 9, 7, 3, 8, 10, 12, 13, 14, 15, 9, 10, 11)

# Зададим диапазоны для параметров ARIMA
# Например, будем перебирать p от 0 до 3, d от 0 до 2, q от 0 до 3
p_range <- 0:3
d_range <- 0:2
q_range <- 0:3

best_aic <- Inf
best_model <- NULL
best_order <- c(NA, NA, NA)

# Перебор всех комбинаций
for (p in p_range) {
  for (d in d_range) {
    for (q in q_range) {
      # Попытаемся подогнать модель, используя tryCatch для обработки ошибок
      fit <- tryCatch(
        {
          arima(x_t, order = c(p, d, q))
        },
        error = function(e) { NULL }
      )
      if (!is.null(fit)) {
        # Вычисляем AIC
        current_aic <- AIC(fit)
        if (current_aic < best_aic) {
          best_aic <- current_aic
          best_model <- fit
          best_order <- c(p, d, q)
        }
      }
    }
  }
}

# Вывод информации о лучшей модели
cat("Лучшие параметры модели ARIMA: order = (", best_order[1], ", ", best_order[2], ", ", best_order[3], ")\n", sep = "")
cat("AIC лучшей модели:", best_aic, "\n\n")
cat("Результаты модели:\n")
print(best_model)

# Извлечение коэффициентов, стандартных ошибок, t-статистик и p-value
coef <- best_model$coef
cat("\nКоэффициенты модели:\n")
print(coef)

if(!is.null(best_model$var.coef)) {
  se <- sqrt(diag(best_model$var.coef))
  cat("\nСтандартные ошибки коэффициентов:\n")
  print(se)
  
  t_val <- abs(coef / se)
  cat("\nТ-статистики коэффициентов:\n")
  print(t_val)
  
  # Степени свободы: число наблюдений минус число коэффициентов
  df <- length(x_t) - length(coef)
  p_val <- 2 * (1 - pt(t_val, df))
  cat("\nP-value для коэффициентов:\n")
  print(p_val)
}

# Остатки лучшей модели
res <- best_model$residuals
cat("\nОстатки лучшей модели:\n")
print(res)

