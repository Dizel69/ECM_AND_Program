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

# Предварительная обработка: удаляем соседние повторяющиеся значения
x <- x[c(TRUE, diff(x) != 0)]

# Максимальное число попыток подбора модели
max_attempts <- 10
attempt <- 1
adequate_model_found <- FALSE
results <- NULL

while (!adequate_model_found && attempt <= max_attempts) {
  cat("Попытка модели №", attempt, "\n")
  # Подбор ARIMA модели и получение остатков
  results <- select_arima(x)
  # Проверка адекватности модели: выполнение 5 тестов на остатках.
  # Функция run_adequacy_tests возвращает список с элементом overall,
  # равным "Модель адекватна", если в тестах H0 получено хотя бы 4 раза.
  adequacy <- run_adequacy_tests(results$residuals)
  
  if (adequacy$overall == "Модель адекватна") {
    adequate_model_found <- TRUE
    cat("Адекватная модель найдена на попытке №", attempt, "\n")
  } else {
    cat("Модель неадекватна на попытке №", attempt, ". Переходим к поиску новой модели...\n\n")
    attempt <- attempt + 1
  }
}

if (!adequate_model_found) {
  cat("Не удалось найти адекватную модель за", max_attempts, "попыток.\n")
} else {
  cat("Остатки адекватной модели:\n")
  print(results$residuals)
}
