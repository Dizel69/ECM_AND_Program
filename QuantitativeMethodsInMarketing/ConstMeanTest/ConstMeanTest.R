# -----------------------------
# Проверка постоянного среднего
# -----------------------------

# Считывание данных из CSV-файла
# Файл data.csv должен находиться в рабочей директории и содержать столбец "Value".
# Данные записаны с запятой (,) в качестве десятичного разделителя.
data <- read.table("data.csv", header = TRUE, sep = "\n", dec = ",", row.names = NULL)
x <- as.numeric(data$Value)

# Построение графика временного ряда
plot(x, type = "o", col = "blue", pch = 19, lwd = 2,
     xlab = "Индекс", ylab = "Значение", main = "Временной ряд (Value)")
grid()

# Ручная реализация одновыборочного t-теста
# Проверяем гипотезу:
#   H0: Среднее значение равно mu0 (остатки имеют постоянное среднее)
#   H1: Среднее значение не равно mu0 (остатки не имеют постоянное среднее)
mu0 <- 0  # Гипотетическое значение среднего (можно изменить при необходимости)
n <- length(x)
x_bar <- mean(x)
s <- sd(x)

# t-статистика рассчитывается по формуле:
# t = (x̄ - mu0) / (s / sqrt(n))
t_stat <- (x_bar - mu0) / (s / sqrt(n))
df <- n - 1

# Для двустороннего теста вычисляем p-value:
p_value <- 2 * (1 - pt(abs(t_stat), df))

# Задаем уровень доверия (например, 99%; можно изменить на 0.95 или 0.97)
conf_level <- 0.99
alpha <- 1 - conf_level
t_crit <- qt(1 - alpha/2, df)
conf_interval <- c(x_bar - t_crit * s/sqrt(n), x_bar + t_crit * s/sqrt(n))

# Определяем вывод по гипотезе
if (p_value > 0.05) {
  hypothesis_manual <- "H0: остатки имеют постоянное среднее"
} else {
  hypothesis_manual <- "H1: остатки не имеют постоянное среднее"
}

cat("Ручная реализация одновыборочного t-теста:\n")
cat("  Число наблюдений (n):", n, "\n")
cat("  Среднее значение (x̄):", x_bar, "\n")
cat("  Стандартное отклонение (s):", s, "\n")
cat("  t-статистика:", t_stat, "\n")
cat("  Степени свободы:", df, "\n")
cat("  p-value:", p_value, "\n")
cat("  Доверительный интервал (", conf_level, "): [", conf_interval[1], ", ", conf_interval[2], "]\n", sep = "")
cat("  Вывод по гипотезе:", hypothesis_manual, "\n\n")

# Применение встроенной функции t.test()
# Проводим одновыборочный t-тест с уровнем доверия conf_level.
t_test_result <- t.test(x, mu = mu0, alternative = "two.sided", conf.level = conf_level)

if (t_test_result$p.value > 0.05) {
  hypothesis_builtin <- "H0: остатки имеют постоянное среднее"
} else {
  hypothesis_builtin <- "H1: остатки не имеют постоянное среднее"
}

cat("Результаты встроенной функции t.test():\n")
print(t_test_result)
cat("Вывод по гипотезе:", hypothesis_builtin, "\n")
