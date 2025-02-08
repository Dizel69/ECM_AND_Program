# -------------------------------
# Проверка постояннства Дисперсий
# -------------------------------

# Считывание данных из CSV-файла
# Файл data.csv должен находиться в рабочей директории и содержать столбец "Value".
# Данные записаны с запятой (,) в качестве десятичного разделителя.
data <- read.table("data.csv", header = TRUE, sep = "\n", dec = ",", row.names = NULL)
x <- as.numeric(data$Value)

# Построение графика временного ряда
# Для наглядности делим ряд на две группы: первая половина и вторая половина.
plot(x, type = "o", col = "black", pch = 19, lwd = 2,
     xlab = "Индекс", ylab = "Значение", main = "Временной ряд, разделённый на две группы")
grid()

# Разбиваем ряд на две группы (первая половина и вторая половина)
n <- length(x)
half <- floor(n / 2)
group1 <- x[1:half]
group2 <- x[(half + 1):n]

# Выделяем точки разных групп разными цветами
points(1:half, group1, col = "blue", pch = 19)
points((half + 1):n, group2, col = "red", pch = 19)
legend("topright", legend = c("Группа 1", "Группа 2"), col = c("blue", "red"), pch = 19)

# Ручная реализация теста Фишера (F-test)
# Вычисляем выборочные дисперсии для каждой группы
n1 <- length(group1)
n2 <- length(group2)
var1 <- var(group1)
var2 <- var(group2)

# Чтобы F-статистика всегда была >= 1, выбираем отношение большей дисперсии к меньшей
if (var1 >= var2) {
  F_value <- var1 / var2
  df1 <- n1 - 1
  df2 <- n2 - 1
} else {
  F_value <- var2 / var1
  df1 <- n2 - 1
  df2 <- n1 - 1
}

# Вычисляем p-value для двухстороннего теста:
# p-value = 2 * (1 - P(F <= F_value)) по распределению F с df1 и df2 степенями свободы
p_manual <- 2 * (1 - pf(F_value, df1, df2))
if (p_manual > 1) p_manual <- 1

# Определяем вывод по гипотезе:
# Если p-value > 0.05, принимаем H₀ (дисперсии равны), иначе – H₁.
if (p_manual > 0.05) {
  hypothesis_manual <- "H0: дисперсии равны (остатки имеют постоянную вариативность)"
} else {
  hypothesis_manual <- "H1: дисперсии различаются"
}

cat("Ручная реализация теста Фишера (F-test):\n")
cat("  Группа 1: n =", n1, ", выборочная дисперсия =", var1, "\n")
cat("  Группа 2: n =", n2, ", выборочная дисперсия =", var2, "\n")
cat("  F-статистика =", F_value, "\n")
cat("  Степени свободы: df1 =", df1, ", df2 =", df2, "\n")
cat("  p-value =", p_manual, "\n")
cat("  Вывод по гипотезе:", hypothesis_manual, "\n\n")

# Применение встроенной функции var.test()
# Проводим F-тест с альтернативной гипотезой "two.sided" и уровнем доверия по умолчанию (например, 95%).
var_test_result <- var.test(group1, group2, alternative = "two.sided", conf.level = 0.95)

if (var_test_result$p.value > 0.05) {
  hypothesis_builtin <- "H0: дисперсии равны (остатки имеют постоянную вариативность)"
} else {
  hypothesis_builtin <- "H1: дисперсии различаются"
}

cat("Результаты встроенной функции var.test():\n")
print(var_test_result)
cat("Вывод по гипотезе:", hypothesis_builtin, "\n")
