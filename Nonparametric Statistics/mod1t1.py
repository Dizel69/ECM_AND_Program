#Сравнивается содержание оксида К2О в пробах гранитной интрузии, полученное
#при использовании 2-х разных методов анализа. Для проверки гипотезы об
#отсутсвии различий у методов определения процентного содержания оксида
#использовать критерий ранговых сумм Уилкоксона и приближение Имана. Оценить
#параметр delta и построить для него доверительный интервал.
#Оценить размер эффекта.

import numpy as np

# Входные данные
method1_results = np.array([3.78, 3.34, 4.71, 5.15, 4.16, 3.46, 4.18, 4.97, 4.02, 2.62])
method2_results = np.array([3.82, 3.87, 2.8, 4.02, 3.78, 4.89, 3.91, 3.56, 2.56, 4.01])

# Объединяем и ранжируем данные
combined_results = np.concatenate((method1_results, method2_results))
ranks = np.argsort(np.argsort(combined_results)) + 1  # ранги начинаются с 1

# Разделяем ранги обратно по методам
ranks_method1 = ranks[:len(method1_results)]
ranks_method2 = ranks[len(method1_results):]

# Рассчитываем W
W = np.sum(ranks_method2)
W_an = np.sum(method1_results) - 4

# Рассчитываем необходимые статистики для приближения Имана
m = len(method1_results)
n = len(method2_results)
N = m + n
mean_W = m * (N + 1) / 2
std_W = np.sqrt(m * n * (N + 1) / 12)
adj_W = W - mean_W
Z = adj_W / std_W


# Реализация CDF нормального распределения
def norm_cdf(x):
    """Аппроксимация функции распределения нормального закона (CDF)"""
    # Константы для аппроксимации
    a1, a2, a3, a4, a5 = 0.254829592, -0.284496736, 1.421413741, -1.453152027, 1.061405429
    p = 0.3275911
    sign = 1 if x >= 0 else -1
    x = abs(x) / np.sqrt(2.0)
    t = 1.0 / (1.0 + p * x)
    y = 1.0 - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * np.exp(-x * x)
    return 0.5 * (1.0 + sign * y)

# Рассчитываем p-значение
p_value = 2 * (1 - norm_cdf(abs(Z)))

# Бутстреп для доверительного интервала для delta
def bootstrap_ci(data1, data2, num_bootstrap=10000, alpha=0.05):
    deltas = []
    combined = np.concatenate((data1, data2))
    n1 = len(data1)
    for _ in range(num_bootstrap):
        np.random.shuffle(combined)
        bootstrap_sample1 = combined[:n1]
        bootstrap_sample2 = combined[n1:]
        deltas.append(np.median(bootstrap_sample1) - np.median(bootstrap_sample2))
    lower_bound = np.percentile(deltas, 100 * alpha / 2)
    upper_bound = np.percentile(deltas, 100 * (1 - alpha / 2))
    return lower_bound, upper_bound

ci_lower, ci_upper = bootstrap_ci(method1_results, method2_results)

# Оценка дельта (разницы медиан)
delta = np.median(method1_results) - np.median(method2_results)

# Размер эффекта
effect_size = delta / np.std(combined_results, ddof=1)

# Вывод результатов
print("Критерий Уилкоксона")
print(f"W = {W_an:.0f}")
print(f"p-значение = {p_value:.4f}")
print("95% доверительный интервал для delta:")
print(f"({ci_lower:.7f}, {ci_upper:.7f})")
print(f"Оценка эффекта (разница медиан) = {delta:.7f}")
print(f"Asymptotic p-value = {p_value:.4f}")
print(f"Размер эффекта = {effect_size:.4f}")

# Приближение Имана
print("Приближение Имана")
print(f"Z-статистика = {Z:.4f}")
print(f"Достигнутый уровень значимости (асимптотическое p-значение) = {p_value:.4f}")
