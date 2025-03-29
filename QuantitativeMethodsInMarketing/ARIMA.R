# ARIMA.R
library(forecast)

# Функция для вычисления MAPE (in-sample)
calculate_mape <- function(actual, predicted) {
  # MAPE = (1/n) * Σ(|actual - predicted| / |actual|) * 100
  return(mean(abs((actual - predicted) / abs(actual)), na.rm = TRUE) * 100)
}

# Функция для перебора всех вариантов параметров и сохранения кандидатов
select_arima_candidates <- function(x_t, 
                                    p_range = 0:3, d_range = 0:1, q_range = 0:3, 
                                    P_range = 0:3, D_range = 0:1, Q_range = 0:3,
                                    T_range = 1:1) {
  candidate_info <- data.frame(p = integer(), d = integer(), q = integer(), 
                               P = integer(), D = integer(), Q = integer(), T = integer(),
                               mape = numeric(), aic = numeric(), stringsAsFactors = FALSE)
  models_list <- list()
  
  cat(sprintf("Перебор сезонности из диапазона: T_range = %s\n", paste(T_range, collapse = ", ")))
  
  iteration <- 0
  total_iterations <- length(T_range) * length(p_range) * length(d_range) * length(q_range) *
                      length(P_range) * length(D_range) * length(Q_range)
  
  for (T_val in T_range) {
    for (D in D_range) {
      for (P in P_range) {
        for (Q in Q_range) {
          for (d in d_range) {
            for (p in p_range) {
              for (q in q_range) {
                iteration <- iteration + 1
                cat(sprintf("Итерация %d из %d: (p,d,q)=(%d,%d,%d), (P,D,Q,T)=(%d,%d,%d,%d)\n",
                            iteration, total_iterations, p, d, q, P, D, Q, T_val))
                
                fit <- tryCatch({
                  Arima(ts(x_t, frequency = T_val), 
                        order = c(p, d, q), 
                        seasonal = list(order = c(P, D, Q), period = T_val))
                }, error = function(e) { NULL })
                
                if (!is.null(fit)) {
                  aic_val <- AIC(fit)
                  fc <- tryCatch({
                    fitted(fit)
                  }, error = function(e) { rep(NA, length(x_t)) })
                  if (length(fc) == length(x_t)) {
                    mape_val <- calculate_mape(x_t, fc)
                  } else {
                    mape_val <- NA
                  }
                  
                  if (!is.na(mape_val)) {
                    candidate_info <- rbind(candidate_info,
                                              data.frame(p = p, d = d, q = q,
                                                         P = P, D = D, Q = Q, T = T_val,
                                                         mape = mape_val, aic = aic_val,
                                                         stringsAsFactors = FALSE))
                    models_list[[nrow(candidate_info)]] <- fit
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  
  candidate_info <- candidate_info[!is.na(candidate_info$mape), ]
  if (nrow(candidate_info) == 0) {
    stop("Не найдено кандидатов с вычисленным MAPE.")
  }
  
  # Сортировка по MAPE, затем по AIC
  candidate_info <- candidate_info[order(candidate_info$mape, candidate_info$aic), ]
  cat("Топ-5 кандидатов:\n")
  print(head(candidate_info, 5))
  
  sorted_indices <- order(candidate_info$mape, candidate_info$aic)
  sorted_models <- models_list[sorted_indices]
  
  return(list(candidate_info = candidate_info, models = sorted_models))
}

# Функция для выбора лучшей модели из списка кандидатов с проверкой адекватности
select_arima_from_candidates <- function(x_t, 
                         p_range = 0:3, d_range = 0:1, q_range = 0:3, 
                         P_range = 0:3, D_range = 0:1, Q_range = 0:3,
                         T_range = 1:12) {
  candidates <- select_arima_candidates(x_t, p_range, d_range, q_range, P_range, D_range, Q_range, T_range)
  candidate_info <- candidates$candidate_info
  models_list <- candidates$models
  
  for (idx in 1:nrow(candidate_info)) {
    fit <- models_list[[idx]]
    resids <- residuals(fit)
    adequacy <- run_adequacy_tests(resids)
    cat(sprintf("Проверка кандидата %d из %d:\n", idx, nrow(candidate_info)))
    cat(adequacy$summary, "\n")
    if (adequacy$overall == "Модель адекватна") {
      cat("Кандидат ", idx, " прошёл проверку адекватности.\n", sep = "")
      return(list(best_model = fit, 
                  best_order = as.numeric(candidate_info[idx, c("p", "d", "q")]),
                  best_seasonal_order = as.numeric(candidate_info[idx, c("P", "D", "Q", "T")]),
                  best_mape = candidate_info$mape[idx],
                  residuals = resids))
    } else {
      cat("Кандидат ", idx, " не прошёл проверку адекватности.\n\n")
    }
  }
  cat("Ни одна модель не прошла тесты адекватности, возвращается лучшая по MAPE.\n")
  best_idx <- 1
  return(list(best_model = models_list[[best_idx]], 
              best_order = as.numeric(candidate_info[best_idx, c("p", "d", "q")]),
              best_seasonal_order = as.numeric(candidate_info[best_idx, c("P", "D", "Q", "T")]),
              best_mape = candidate_info$mape[best_idx],
              residuals = residuals(models_list[[best_idx]])))
}
