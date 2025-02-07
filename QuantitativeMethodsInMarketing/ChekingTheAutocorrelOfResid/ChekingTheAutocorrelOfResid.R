# --------------------------
# Тест автокорреляции остатков:
# Ручная реализация Box-Pierce теста и применение встроенной функции Box.test()
# --------------------------

# Считывание данных из CSV-файла
# Файл data.csv должен находиться в рабочей директории и содержать столбец "Value"
data <- read.table("data.csv", header = TRUE, sep = "\n", dec = ",", row.names = NULL)
x <- data$Value

# Если данные не числовые, преобразуем их в numeric
if (!is.numeric(x)) {
  x <- as.numeric(x)
}

# Построение графика временного ряда
plot(x, type = "o", col = "blue", pch = 19, lwd = 2,
     xlab = "Индекс", ylab = "Значение", main = "Временной ряд (остатки)")
grid()

# Ручная реализация теста Box-Pierce

# Функция для вычисления автокорреляций для лагов от 1 до max_lag
compute_autocorrelations <- function(x, max_lag) {
  n <- length(x)
  x_mean <- mean(x)
  denom <- sum((x - x_mean)^2)
  acf_values <- numeric(max_lag)
  
  for (lag in 1:max_lag) {
    # Вычисляем автокорреляцию для данного лага
    num <- sum((x[(lag + 1):n] - x_mean) * (x[1:(n - lag)] - x_mean))
    acf_values[lag] <- num / denom
  }
  return(acf_values)
}

# Функция для проведения ручного теста Box-Pierce
box_pierce_test_manual <- function(x, lag = 10) {
  n <- length(x)
  
  # Вычисляем автокорреляции для лагов от 1 до lag
  acf_values <- compute_autocorrelations(x, lag)
  
  # Статистика Box-Pierce: Q = n * сумма квадратов автокорреляций
  Q <- n * sum(acf_values^2)
  
  # p-value по распределению хи-квадрат с lag степенями свободы
  p_value <- 1 - pchisq(Q, df = lag)
  
  # Определяем гипотезу:
  # H0: остатки независимы (p_value > 0.05)
  # H1: остатки коррелируют (p_value <= 0.05)
  if (p_value > 0.05) {
    hypothesis <- "H0: остатки независимы"
  } else {
    hypothesis <- "H1: остатки коррелируют"
  }
  
  return(list(statistic = Q, p.value = p_value, lag = lag, acf = acf_values, hypothesis = hypothesis))
}

# Выполним ручной тест
manual_result <- box_pierce_test_manual(x, lag = 10)

# Тест с использованием встроенной функции Box.test()
# Внимание: для корректного задания типа теста используем type = "Box-Pierce"
box_test_result <- Box.test(x, lag = 10, type = "Box-Pierce")

# Вывод результатов
cat("Ручной тест Box-Pierce:\n")
cat("  Статистика (Q):", manual_result$statistic, "\n")
cat("  p-value:", manual_result$p.value, "\n")
cat("  Гипотеза:", manual_result$hypothesis, "\n")
cat("  Автокорреляции для лагов:\n")
print(manual_result$acf)
cat("\n---------------------------------\n")

cat("Тест с использованием Box.test():\n")
print(box_test_result)

# Вывод интерпретации для теста Box.test():
if (box_test_result$p.value > 0.05) {
  cat("Вывод: H0 (остатки независимы, p-value >", box_test_result$p.value, ")\n")
} else {
  cat("Вывод: H1 (остатки коррелируют, p-value <=", box_test_result$p.value, ")\n")
}
