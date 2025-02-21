#--------------------
# Медианный критерий
#--------------------

# Считывание данных из CSV-файла
data <- read.table("data.csv", header = TRUE, sep = "\n", dec = ",", row.names = NULL)

# Извлекаем столбец с данными
x <- data$Value

# Если x получился фактором или текстом, преобразуем его в числовой вектор
if (!is.numeric(x)) {
  x <- as.numeric(x)
}

# Вычисляем медиану ряда
med <- median(x)

# Определяем знаки для каждого элемента ряда по отношению к медиане:
# Если значение больше медианы, то "+", иначе "-"
# (Если значение равно медиане, можно оставить "-" или задать по-другому. Здесь выберем "-")
s <- ifelse(x > med, "+", "-")

# Подсчитываем число серий (runs)
# Считаем, что первая наблюдаемая группа – это первая серия,
# затем каждая смена знака определяет новую серию.
nruns <- 1 + sum(s[-1] != s[-length(s)])

# Определяем количество наблюдений и считаем n0 и n1:
# n  - общее число наблюдений
# n0 - количество чисел выше медианы
# n1 - количество чисел ниже медианы
n <- length(x)
n0 <- sum(x > med)
n1 <- sum(x < med)

# Вычисляем теоретическое среднее число серий (mean)
# по формуле: mean = 1 + ((2*n0*n1)/n)
mean_runs <- 1 + ((2 * n0 * n1) / n)

# Вычисляем стандартное отклонение (sigma)
# Формула: sigma = sqrt((2*n0*n1*(2*n0*n1 - n))/(n^2*(n - 1)))
sigma <- sqrt((2 * n0 * n1 * (2 * n0 * n1 - n)) / (n^2 * (n - 1)))

# Вычисляем поправку (correct)
# Если (nruns - mean_runs) > 0, то корректировка равна -0.5, иначе +0.5.
# (Если разность равна 0, можно задать поправку равной 0)
correct <- ifelse((nruns - mean_runs) > 0, -0.5, 0.5)

# Вычисляем z-значение по формуле:
# z = (nruns - mean_runs + correct) / sigma
z <- (nruns - mean_runs + correct) / sigma

# Определяем критическое значение z
zCritical <- qnorm(0.975)  # аналог Excel: НОРМ.СТ.ОБР(0,975)

# Проверяем гипотезу:
# Если (zCritical - z) > 0, то принимаем H0 (остатки случайны), иначе H1.
hypothesis <- ifelse(zCritical - z > 0, "H0: распределение остатков имеется", "H1: распределение остатков не имеется")

# Построение графика ряда
# Строим график числового ряда с линиями и точками, а также добавляем горизонтальную линию медианы.
plot(x, type = "o", col = "blue", pch = 19, lwd = 2,
     xlab = "Индекс", ylab = "Значение",
     main = "График числового ряда с медианой")
abline(h = med, col = "red", lwd = 2, lty = 2)  # линия медианы
text(x = 1, y = med, labels = paste("Медиана =", round(med, 2)), pos = 3, col = "red")

# Вывод результатов в консоль
cat("Медиана ряда:", med, "\n")
cat("Число наблюдений (n):", n, "\n")
cat("Количество чисел выше медианы (n0):", n0, "\n")
cat("Количество чисел ниже медианы (n1):", n1, "\n\n")

cat("Число серий (nruns):", nruns, "\n")
cat("Теоретическое среднее число серий (mean):", mean_runs, "\n")
cat("Стандартное отклонение (sigma):", sigma, "\n")
cat("Поправка (correct):", correct, "\n")
cat("z-значение:", z, "\n")
cat("Критическое значение (zCritical):", zCritical, "\n")
cat("Вывод по гипотезе:", hypothesis, "\n")
