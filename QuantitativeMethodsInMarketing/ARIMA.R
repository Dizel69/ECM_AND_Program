library(forecast)
library(tseries)
library(seasonal)

# Функция для вычисления MAPE
calculate_mape <- function(actual, predicted) {
  return(mean(abs((actual - predicted) / actual)) * 100)
}

# Функция для подбора лучшей модели ARIMA с учетом сезонности
select_arima <- function(x_t) {
  best_mape <- Inf
  best_model <- NULL
  best_seasonality <- NULL
  best_order <- NULL

  # Проверяем сезонность с помощью тестов
  seasonality_tests <- list(
    "Friedman" = friedmanTest(x_t),
    "Kruskal-Wallis" = kruskalTest(x_t),
    "Welch" = welchTest(x_t)
  )
  
  # Собираем результаты тестов в таблицу
  seasonality_results <- data.frame(Test = names(seasonality_tests),
                                    P_Value = sapply(seasonality_tests, function(test) test$p.value))
  
  print("Результаты тестов на сезонность:")
  print(seasonality_results)
  
  # Если хотя бы один тест даёт p-value < 0.05, считаем, что сезонность есть
  has_seasonality <- any(seasonality_results$P_Value < 0.05)
  
  # Определяем возможные сезонные параметры
  seasonal_periods <- if (has_seasonality) 2:12 else c(1)

  # Подбор ARIMA-модели с различными параметрами
  for (seasonality in seasonal_periods) {
    for (d in 0:1) {
      for (p in 0:3) {
        for (q in 0:3) {
          for (D in 0:1) {
            for (P in 0:3) {
              for (Q in 0:3) {
                order <- c(p, d, q)
                seasonal <- c(P, D, Q, seasonality)
                
                fit <- tryCatch({
                  auto.arima(data, d=d, D=D, max.p=3, max.q=3, max.P=3, max.Q=3, seasonal=TRUE, stepwise=FALSE, approximation=FALSE, trace=FALSE)
                }, error = function(e) NULL)
                
                if (!is.null(fit)) {
                  fitted_values <- fitted(fit)
                  mape <- calculate_mape(data, fitted_values)
                  
                  if (mape < best_mape) {
                    best_mape <- mape
                    best_model <- fit
                    best_seasonality <- seasonality
                    best_order <- list(order = order, seasonal = seasonal)
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  
  print(paste("Лучшая сезонность:", best_seasonality))
  print(paste("Лучшее MAPE:", best_mape))
  print("Лучшие параметры модели:")
  print(best_order)
  
  return(best_model)
}