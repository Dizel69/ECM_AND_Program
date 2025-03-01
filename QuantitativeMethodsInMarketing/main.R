# main.R
# Основной сценарий: считывание данных, подбор лучшей ARIMA модели и проверка адекватности её остатков

# Подключаем функции из отдельных файлов:
source("ARIMA.R")         # файл с функцией select_arima()
source("AdequacyCheck.R") # файл с функцией run_adequacy_tests()

# 1. Считывание данных
# Файл data.csv должен находиться в рабочей директории и содержать столбец "Value"
data <- read.table("data.csv", header = TRUE, sep = "\n", dec = ",", row.names = NULL)
x <- data$Value
if (is.null(x)) {
  stop("Столбец 'Value' не найден в CSV-файле. Проверьте заголовки столбцов.")
}
x <- as.numeric(x)  # Преобразуем в числовой вектор

# 2. Подбор лучшей ARIMA модели и получение остатков
residuals <- select_arima(x)
cat("Остатки лучшей ARIMA модели:\n")
print(residuals)

# 3. Проверка адекватности модели: выполнение 5 тестов на остатках
run_adequacy_tests(residuals)
