run_adequacy_tests <- function(x) {
  # Приводим входной вектор к числовому типу
  x <- as.numeric(x)
  
  # Предварительная обработка: удаляем соседние повторяющиеся значения,
  # если два одинаковых числа стоят подряд, оставляем только первое
  if (length(x) > 1) {
    x <- x[c(TRUE, diff(x) != 0)]
  }
  
  # Построение базового графика временного ряда
  plot(x, type = "o", col = "blue", pch = 19, lwd = 2,
       xlab = "Индекс", ylab = "Значение",
       main = "График числового ряда (без соседних дубликатов)")
  grid()
  
  results <- list()
  ok_flags <- c()  # вектор для результатов тестов ("OK" или "*")
  
  # ------------------------------------------------------------------
  # 1 Test: Критерий пиков и впадин (реализация вручную)
  # ------------------------------------------------------------------
  diffs <- diff(x)
  signs <- ifelse(diffs > 0, "+", "-")
  nruns <- 1 + sum(signs[-1] != signs[-length(signs)])
  n <- length(x)
  mean_runs <- (2 * n - 1) / 3
  sigma <- sqrt((16 * n - 29) / 90)
  correct <- ifelse((nruns - mean_runs) > 0, -0.5, 0.5)
  z <- (nruns - mean_runs + correct) / sigma
  z_critical <- qnorm(0.975)
  hypothesis1 <- ifelse((z_critical - z) > 0, "H0", "H1")
  p_value1 <- 2 * (1 - pnorm(abs(z)))
  
  cat("----- 1 Test: Критерий пиков и впадин -----\n")
  cat("Число серий (nruns):", nruns, "\n")
  cat("Теоретическое среднее серий:", mean_runs, "\n")
  cat("Стандартное отклонение (sigma):", sigma, "\n")
  cat("Поправка (correct):", correct, "\n")
  cat("z-значение:", z, "\n")
  cat("Критическое значение (zCritical):", z_critical, "\n")
  cat("p-value:", p_value1, "\n")
  cat("Вывод по гипотезе:", hypothesis1, "\n\n")
  results$test1 <- list(nruns=nruns, mean_runs=mean_runs, sigma=sigma, correct=correct, z=z, 
                          z_critical=z_critical, p_value=p_value1, hypothesis=hypothesis1)
  ok_flags[1] <- ifelse(hypothesis1=="H0", "OK", "*")
  
  # ------------------------------------------------------------------
  # 2 Test: Медианный критерий (реализация вручную)
  # ------------------------------------------------------------------
  med <- median(x)
  # Исключаем значения, равные медиане
  x2 <- x[x != med]
  if (length(x2) == 0) {
    stop("Все значения равны медиане. Невозможно выполнить медианный критерий.")
  }
  s <- ifelse(x2 > med, "+", "-")
  nruns2 <- 1 + sum(s[-1] != s[-length(s)])
  n2 <- length(x2)
  n0 <- sum(x2 > med)
  n1 <- sum(x2 < med)
  mean_runs2 <- 1 + ((2 * n0 * n1) / n2)
  sigma2 <- sqrt((2 * n0 * n1 * (2 * n0 * n1 - n2)) / (n2^2 * (n2 - 1)))
  correct2 <- ifelse((nruns2 - mean_runs2) > 0, -0.5, 0.5)
  z2 <- (nruns2 - mean_runs2 + correct2) / sigma2
  zCritical2 <- qnorm(0.975)
  hypothesis2 <- ifelse((zCritical2 - z2) > 0, "H0", "H1")
  p_value2 <- 2 * (1 - pnorm(abs(z2)))
  
  # График с медианой (на основе исходного ряда)
  plot(x, type = "o", col = "blue", pch = 19, lwd = 2,
       xlab = "Индекс", ylab = "Значение",
       main = "График числового ряда с медианой")
  abline(h = med, col = "red", lwd = 2, lty = 2)
  text(1, med, labels = paste("Медиана =", round(med, 2)), pos = 3, col = "red")
  
  cat("----- 2 Test: Медианный критерий -----\n")
  cat("Медиана ряда:", med, "\n")
  cat("Число наблюдений (n) (без медианных значений):", n2, "\n")
  cat("Количество чисел выше медианы (n0):", n0, "\n")
  cat("Количество чисел ниже медианы (n1):", n1, "\n")
  cat("Число серий (nruns):", nruns2, "\n")
  cat("Теоретическое среднее серий:", mean_runs2, "\n")
  cat("Стандартное отклонение (sigma):", sigma2, "\n")
  cat("Поправка (correct):", correct2, "\n")
  cat("z-значение:", z2, "\n")
  cat("Критическое значение (zCritical):", zCritical2, "\n")
  cat("p-value:", p_value2, "\n")
  cat("Вывод по гипотезе:", hypothesis2, "\n\n")
  results$test2 <- list(median=med, n=n2, n0=n0, n1=n1, nruns=nruns2, mean_runs=mean_runs2,
                          sigma=sigma2, correct=correct2, z=z2, zCritical=zCritical2, p_value=p_value2,
                          hypothesis=hypothesis2)
  ok_flags[2] <- ifelse(hypothesis2=="H0", "OK", "*")
  
  # ------------------------------------------------------------------
  # 3 Test: Box-Pierce тест (встроенная функция)
  # ------------------------------------------------------------------
  box_test_result <- Box.test(x, lag = 10, type = "Box-Pierce")
  hypothesis3 <- ifelse(box_test_result$p.value > 0.05, "H0", "H1")
  cat("----- 3 Test: Box-Pierce -----\n")
  cat("Box-Pierce Test Statistic:", box_test_result$statistic, "\n")
  cat("p-value:", box_test_result$p.value, "\n")
  cat("Вывод по гипотезе:", hypothesis3, "\n\n")
  results$test3 <- list(statistic=box_test_result$statistic, p_value=box_test_result$p.value, hypothesis=hypothesis3)
  ok_flags[3] <- ifelse(hypothesis3=="H0", "OK", "*")
  
  # ------------------------------------------------------------------
  # 4 Test: t-тест для проверки постоянного среднего (встроенная функция)
  # ------------------------------------------------------------------
  n <- length(x)
  half <- floor(n / 2)
  group1 <- x[1:half]
  group2 <- x[(half + 1):n]
  t_test_result <- t.test(group1, group2, alternative = "two.sided", conf.level = 0.99)
  hypothesis4 <- ifelse(t_test_result$p.value > 0.05, "H0", "H1")
  cat("----- 4 Test: t-тест для постоянного среднего -----\n")
  cat("t-статистика:", t_test_result$statistic, "\n")
  cat("p-value:", t_test_result$p.value, "\n")
  cat("Доверительный интервал:", paste("[", paste(round(t_test_result$conf.int, 3), collapse = ", "), "]"), "\n")
  cat("Вывод по гипотезе:", hypothesis4, "\n\n")
  results$test4 <- list(t_statistic=t_test_result$statistic, p_value=t_test_result$p.value, 
                          conf_int=t_test_result$conf.int, hypothesis=hypothesis4)
  ok_flags[4] <- ifelse(hypothesis4=="H0", "OK", "*")
  
  # ------------------------------------------------------------------
  # 5 Test: F-тест Фишера для равенства дисперсий (встроенная функция)
  # ------------------------------------------------------------------
  half <- floor(n / 2)
  group1 <- x[1:half]
  group2 <- x[(half + 1):n]
  
  # График для визуализации разделения на группы
  plot(x, type = "o", col = "black", pch = 19, lwd = 2,
       xlab = "Индекс", ylab = "Значение", main = "Временной ряд, разделённый на две группы")
  grid()
  points(1:half, group1, col = "blue", pch = 19)
  points((half + 1):n, group2, col = "red", pch = 19)
  legend("topright", legend = c("Группа 1", "Группа 2"), col = c("blue", "red"), pch = 19)
  
  var_test_result <- var.test(group1, group2, alternative = "two.sided", conf.level = 0.95)
  hypothesis5 <- ifelse(var_test_result$p.value > 0.05, "H0", "H1")
  cat("----- 5 Test: F-тест Фишера -----\n")
  cat("F-статистика:", var_test_result$statistic, "\n")
  cat("p-value:", var_test_result$p.value, "\n")
  cat("Доверительный интервал отношения дисперсий:", paste("[", paste(round(var_test_result$conf.int, 3), collapse = ", "), "]"), "\n")
  cat("Вывод по гипотезе:", hypothesis5, "\n\n")
  results$test5 <- list(F_statistic=var_test_result$statistic, p_value=var_test_result$p.value,
                          conf_int=var_test_result$conf.int, hypothesis=hypothesis5)
  ok_flags[5] <- ifelse(hypothesis5=="H0", "OK", "*")
  
  # ------------------------------------------------------------------
  # Итоговая сводная таблица результатов
  # ------------------------------------------------------------------
  overall_ok <- sum(ok_flags == "OK")
  overall <- if (overall_ok >= 4) "Модель адекватна" else "Модель неадекватна"
  
  cat("----------------------------------------------------------\n")
  cat("| 1 Test | 2 Test | 3 Test | 4 Test | 5 Test |\n")
  cat("----------------------------------------------------------\n")
  cat(sprintf("|   %3s   |   %3s   |   %3s   |   %3s   |   %3s   |\n", 
              ok_flags[1], ok_flags[2], ok_flags[3], ok_flags[4], ok_flags[5]))
  cat("----------------------------------------------------------\n")
  cat("Общий вывод:", overall, "\n")
  
  results$overall <- overall
  results$summary <- sprintf("|   %3s   |   %3s   |   %3s   |   %3s   |   %3s   | => %s", 
                             ok_flags[1], ok_flags[2], ok_flags[3], ok_flags[4], ok_flags[5], overall)
  
  return(results)
}
