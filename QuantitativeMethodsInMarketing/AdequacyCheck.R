# AdequacyCheck.R
run_adequacy_tests <- function(x) {
  # Приводим x к числовому типу и удаляем соседние дубликаты
  x <- as.numeric(x)
  if (length(x) > 1) {
    x <- x[c(TRUE, diff(x) != 0)]
  }
  
  results <- list()
  ok_flags <- c()
  
  ## Тест 1: Критерий пиков и впадин
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
  cat("----- Тест 1: Критерий пиков и впадин -----\n")
  cat("nruns:", nruns, "mean_runs:", mean_runs, "sigma:", sigma, "z:", z, "\n")
  results$test1 <- list(p_value = p_value1, hypothesis = hypothesis1)
  ok_flags[1] <- ifelse(hypothesis1 == "H0", "OK", "*")
  
  ## Тест 2: Медианный критерий
  med <- median(x)
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
  cat("----- Тест 2: Медианный критерий -----\n")
  results$test2 <- list(p_value = p_value2, hypothesis = hypothesis2)
  ok_flags[2] <- ifelse(hypothesis2 == "H0", "OK", "*")
  
  ## Тест 3: Box-Pierce тест
  box_test <- Box.test(x, lag = 10, type = "Box-Pierce")
  hypothesis3 <- ifelse(box_test$p.value > 0.05, "H0", "H1")
  cat("----- Тест 3: Box-Pierce -----\n")
  results$test3 <- list(p_value = box_test$p.value, hypothesis = hypothesis3)
  ok_flags[3] <- ifelse(hypothesis3 == "H0", "OK", "*")
  
  ## Тест 4: t-тест для проверки постоянного среднего
  half <- floor(n / 2)
  group1 <- x[1:half]
  group2 <- x[(half + 1):n]
  t_test <- t.test(group1, group2, alternative = "two.sided", conf.level = 0.99)
  hypothesis4 <- ifelse(t_test$p.value > 0.05, "H0", "H1")
  cat("----- Тест 4: t-тест -----\n")
  results$test4 <- list(p_value = t_test$p.value, hypothesis = hypothesis4)
  ok_flags[4] <- ifelse(hypothesis4 == "H0", "OK", "*")
  
  ## Тест 5: F-тест Фишера для равенства дисперсий
  f_test <- var.test(group1, group2, alternative = "two.sided", conf.level = 0.95)
  hypothesis5 <- ifelse(f_test$p.value > 0.05, "H0", "H1")
  cat("----- Тест 5: F-тест -----\n")
  results$test5 <- list(p_value = f_test$p.value, hypothesis = hypothesis5)
  ok_flags[5] <- ifelse(hypothesis5 == "H0", "OK", "*")
  
  overall_ok <- sum(ok_flags == "OK")
  overall <- if (overall_ok >= 4) "Модель адекватна" else "Модель неадекватна"
  
  results$overall <- overall
  results$summary <- sprintf("| %6s | %6s | %6s | %6s | %6s | => %s", 
                              ok_flags[1], ok_flags[2], ok_flags[3], ok_flags[4], ok_flags[5], overall)
  cat("----------------------------------------------------------\n")
  cat("| Тест 1 | Тест 2 | Тест 3 | Тест 4 | Тест 5 |\n")
  cat("----------------------------------------------------------\n")
  cat(results$summary, "\n")
  cat("----------------------------------------------------------\n")
  
  return(results)
}
