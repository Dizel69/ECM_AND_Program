# ARIMA.R
library(forecast)

select_arima <- function(x_t, 
                         p_range = 0:3, d_range = 0:1, q_range = 0:3, 
                         P_range = 0:3, D_range = 0:1, Q_range = 0:3) {
  best_aic <- Inf
  best_model <- NULL
  best_order <- c(NA, NA, NA)
  best_seasonal_order <- c(NA, NA, NA, NA)
  
  # Определяем сезонность с помощью findfrequency()
  T_val <- findfrequency(ts(x_t))
  cat(sprintf("Определённая сезонность (T): %d\n", T_val))
  
  total_iterations <- length(p_range) * length(d_range) * length(q_range) *
                      length(P_range) * length(D_range) * length(Q_range)
  iteration <- 0
  
  for (D in D_range) {
    for (P in P_range) {
      for (Q in Q_range) {
        for (d in d_range) {
          for (p in p_range) {
            for (q in q_range) {
              iteration <- iteration + 1
              cat(sprintf("Итерация %d из %d: (p, d, q) = (%d, %d, %d), (P, D, Q, T) = (%d, %d, %d, %d)\n", 
                          iteration, total_iterations, p, d, q, P, D, Q, T_val))
              
              fit <- tryCatch({
                Arima(ts(x_t, frequency = T_val), 
                      order = c(p, d, q), 
                      seasonal = list(order = c(P, D, Q), period = T_val))
              }, error = function(e) { NULL })
              
              if (!is.null(fit)) {
                current_aic <- AIC(fit)
                if (current_aic < best_aic) {
                  best_aic <- current_aic
                  best_model <- fit
                  best_order <- c(p, d, q)
                  best_seasonal_order <- c(P, D, Q, T_val)
                }
              }
            }
          }
        }
      }
    }
  }
  
  if (is.null(best_model)) {
    stop("Ошибка: Не удалось подобрать адекватную модель ARIMA.")
  }
  
  cat("\nЛучшие параметры модели ARIMA: ")
  cat(sprintf("order = (%d, %d, %d), seasonal = (%d, %d, %d, %d)\n", 
              best_order[1], best_order[2], best_order[3], 
              best_seasonal_order[1], best_seasonal_order[2], best_seasonal_order[3], best_seasonal_order[4]))
  cat("AIC лучшей модели:", best_aic, "\n\n")
  cat("Результаты модели:\n")
  print(best_model)
  
  # Извлекаем остатки модели через residuals()
  res <- residuals(best_model)
  cat("\nОстатки лучшей модели:\n")
  print(res)
  
  return(list(best_model = best_model, best_order = best_order, 
              best_seasonal_order = best_seasonal_order, best_aic = best_aic,
              residuals = res))
}
