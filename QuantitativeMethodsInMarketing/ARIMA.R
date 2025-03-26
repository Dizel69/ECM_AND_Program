library(forecast)

# Функция для вычисления MAPE
calculate_mape <- function(actual, predicted) {
  return(mean(abs((actual - predicted) / actual), na.rm = TRUE) * 100)
}

# Функция, которая перебирает все варианты параметров и сохраняет кандидатов
select_arima_candidates <- function(x_t, 
                                    p_range = 0:3, d_range = 0:1, q_range = 0:3, 
                                    P_range = 0:3, D_range = 0:1, Q_range = 0:3) {
  candidates <- list()
  candidate_info <- data.frame(p = integer(), d = integer(), q = integer(), 
                               P = integer(), D = integer(), Q = integer(), T = integer(),
                               mape = numeric(), aic = numeric())
  
  # Здесь можно использовать findfrequency(ts(x_t)); для ускорения можно задать сезон вручную:
  T_val <- 6  
  cat(sprintf("Определённая сезонность (T): %d\n", T_val))
  
  iteration <- 0
  for (D in D_range) {
    for (P in P_range) {
      for (Q in Q_range) {
        for (d in d_range) {
          for (p in p_range) {
            for (q in q_range) {
              iteration <- iteration + 1
              cat(sprintf("Итерация %d: (p, d, q) = (%d, %d, %d), (P, D, Q, T) = (%d, %d, %d, %d)\n", 
                          iteration, p, d, q, P, D, Q, T_val))
              
              fit <- tryCatch({
                arima(ts(x_t, frequency = T_val), 
                      order = c(p, d, q), 
                      seasonal = list(order = c(P, D, Q), period = T_val))
              }, error = function(e) { NULL })
              
              if (!is.null(fit)) {
                aic_val <- AIC(fit)
                # Прогноз для длины ряда (чтобы вычислить MAPE)
                fc <- tryCatch({
                  forecast(fit, h = length(x_t))$mean
                }, error = function(e) { rep(NA, length(x_t)) })
                if (length(fc) == length(x_t)) {
                  mape_val <- calculate_mape(x_t, fc)
                } else {
                  mape_val <- NA
                }
                candidate <- list(model = fit, order = c(p, d, q), 
                                  seasonal = c(P, D, Q, T_val), mape = mape_val, aic = aic_val)
                candidates[[length(candidates) + 1]] <- candidate
                candidate_info <- rbind(candidate_info, 
                                        data.frame(p = p, d = d, q = q, P = P, D = D, Q = Q, T = T_val,
                                                   mape = mape_val, aic = aic_val))
              }
            }
          }
        }
      }
    }
  }
  
  # Убираем кандидатов с NA в MAPE
  candidate_info <- candidate_info[!is.na(candidate_info$mape), ]
  
  if (nrow(candidate_info) == 0) {
    stop("Не найдено кандидатов с вычисленным MAPE.")
  }
  
  # Сортируем кандидатов по MAPE, затем по AIC
  candidate_info <- candidate_info[order(candidate_info$mape, candidate_info$aic), ]
  cat("Топ-5 кандидатов:\n")
  print(head(candidate_info, 5))
  
  # Перестраиваем список кандидатов в том же порядке:
  sorted_candidates <- candidates[order(sapply(candidates, function(cand) cand$mape),
                                          sapply(candidates, function(cand) cand$aic))]
  
  return(sorted_candidates)
}

# Функция для выбора лучшей модели, которая проходит тесты адекватности
select_arima <- function(x_t, 
                         p_range = 0:3, d_range = 0:1, q_range = 0:3, 
                         P_range = 0:3, D_range = 0:1, Q_range = 0:3) {
  candidates <- select_arima_candidates(x_t, p_range, d_range, q_range, P_range, D_range, Q_range)
  
  # Перебираем кандидатов в порядке возрастания MAPE и проверяем адекватность через run_adequacy_tests (из AdequacyCheck.R)
  for (cand in candidates) {
    resids <- residuals(cand$model)
    adequacy <- run_adequacy_tests(resids)
    if (adequacy$overall == "Модель адекватна") {
      cat("Выбрана модель с параметрами:\n")
      cat(sprintf("order = (%d, %d, %d), seasonal = (%d, %d, %d, %d)\n", 
                  cand$order[1], cand$order[2], cand$order[3],
                  cand$seasonal[1], cand$seasonal[2], cand$seasonal[3], cand$seasonal[4]))
      return(cand)
    }
  }
  
  cat("Ни одна модель не прошла тесты адекватности, возвращается лучшая по MAPE.\n")
  return(candidates[[1]])
}
