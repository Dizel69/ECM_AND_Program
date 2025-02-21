# ================================================================
# Общая программа: 5 тестов для анализа временного ряда
# ================================================================

# Считывание данных
# Файл data.csv должен находиться в рабочей директории и содержать один столбец "Value"
data <- read.table("data.csv", header = TRUE, sep = "\n", dec = ",", row.names = NULL)
x <- data$Value
if(is.null(x)) {
  stop("Столбец 'Value' не найден в CSV-файле. Проверьте заголовки столбцов.")
}
x <- as.numeric(x)  # преобразуем в числовой вектор (если требуется)

# ======================================================================
# 1 Test: Критерий пиков и впадин (реализация вручную)
# ======================================================================
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

# Вывод результатов теста 1
cat("----- 1 Test: Критерий пиков и впадин -----\n")
cat("Число серий (nruns):", nruns, "\n")
cat("Теоретическое среднее серий:", mean_runs, "\n")
cat("Стандартное отклонение (sigma):", sigma, "\n")
cat("Поправка (correct):", correct, "\n")
cat("z-значение:", z, "\n")
cat("Критическое значение (zCritical):", z_critical, "\n")
cat("Вывод по гипотезе:", hypothesis1, "\n\n")

# ======================================================================
# 2 Test: Медианный критерий (реализация вручную)
# ======================================================================
med <- median(x)
s <- ifelse(x > med, "+", "-")
nruns2 <- 1 + sum(s[-1] != s[-length(s)])
n0 <- sum(x > med)
n1 <- sum(x < med)
mean_runs2 <- 1 + ((2 * n0 * n1) / n)
sigma2 <- sqrt((2 * n0 * n1 * (2 * n0 * n1 - n)) / (n^2 * (n - 1)))
correct2 <- ifelse((nruns2 - mean_runs2) > 0, -0.5, 0.5)
z2 <- (nruns2 - mean_runs2 + correct2) / sigma2
zCritical2 <- qnorm(0.975)
hypothesis2 <- ifelse((zCritical2 - z2) > 0, "H0", "H1")

# Вывод результатов теста 2
cat("----- 2 Test: Медианный критерий -----\n")
cat("Медиана ряда:", med, "\n")
cat("Число наблюдений (n):", n, "\n")
cat("Количество чисел выше медианы (n0):", n0, "\n")
cat("Количество чисел ниже медианы (n1):", n1, "\n")
cat("Число серий (nruns):", nruns2, "\n")
cat("Теоретическое среднее серий:", mean_runs2, "\n")
cat("Стандартное отклонение (sigma):", sigma2, "\n")
cat("Поправка (correct):", correct2, "\n")
cat("z-значение:", z2, "\n")
cat("Критическое значение (zCritical):", zCritical2, "\n")
cat("Вывод по гипотезе:", hypothesis2, "\n\n")

# ======================================================================
# 3 Test: Box-Pierce тест (встроенная функция)
# ======================================================================
box_test_result <- Box.test(x, lag = 10, type = "Box-Pierce")
hypothesis3 <- ifelse(box_test_result$p.value > 0.05, "H0", "H1")
cat("----- 3 Test: Box-Pierce -----\n")
cat("Box-Pierce Test Statistic:", box_test_result$statistic, "\n")
cat("p-value:", box_test_result$p.value, "\n")
cat("Вывод по гипотезе:", hypothesis3, "\n\n")

# ======================================================================
# 4 Test: t-тест для проверки постоянного среднего (встроенная функция)
# ======================================================================
mu0 <- 0  # Гипотетическое значение среднего
conf_level <- 0.99
t_test_result <- t.test(x, mu = mu0, alternative = "two.sided", conf.level = conf_level)
hypothesis4 <- ifelse(t_test_result$p.value > 0.05, "H0", "H1")
cat("----- 4 Test: t-тест для постоянного среднего -----\n")
cat("t-статистика:", t_test_result$statistic, "\n")
cat("p-value:", t_test_result$p.value, "\n")
cat("Доверительный интервал:", paste("[", paste(round(t_test_result$conf.int, 3), collapse = ", "), "]"), "\n")
cat("Вывод по гипотезе:", hypothesis4, "\n\n")

# ======================================================================
# 5 Test: F-тест Фишера для равенства дисперсий (встроенная функция)
# ======================================================================
# Разбиваем ряд на две группы (первая и вторая половины)
half <- floor(n / 2)
group1 <- x[1:half]
group2 <- x[(half + 1):n]

# Для наглядности выделим группы на графике
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

# ======================================================================
# Итоговая сводная таблица результатов
# ======================================================================
# Если гипотеза H0 принимается, ставим "OK", если отвергается (принимаем H1) – "*"
flag1 <- ifelse(hypothesis1 == "H0", "OK", "*")
flag2 <- ifelse(hypothesis2 == "H0", "OK", "*")
flag3 <- ifelse(hypothesis3 == "H0", "OK", "*")
flag4 <- ifelse(hypothesis4 == "H0", "OK", "*")
flag5 <- ifelse(hypothesis5 == "H0", "OK", "*")

# Формируем строку сводной таблицы
cat("----------------------------------------------------------\n")
cat("| 1 Test | 2 Test | 3 Test | 4 Test | 5 Test |\n")
cat("----------------------------------------------------------\n")
cat(sprintf("|   %3s   |   %3s   |   %3s   |   %3s   |   %3s   |\n", flag1, flag2, flag3, flag4, flag5))
cat("----------------------------------------------------------\n")
