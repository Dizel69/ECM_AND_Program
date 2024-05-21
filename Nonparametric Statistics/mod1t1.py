#Сравнивается содержание оксида К2О в пробах гранитной интрузии, полученное
#при использовании 2-х разных методов анализа. Для проверки гипотезы об
#отсутсвии различий у методов определения процентного содержания оксида
#использовать критерий ранговых сумм Уилкоксона и приближение Имана. Оценить
#параметр delta и построить для него доверительный интервал.
#Оценить размер эффекта.

import numpy as np


def wilcoxon_rank_sum_test(x, y):
    """
    Критерий ранговых сумм Уилкоксона
    """
    n1 = len(x)
    n2 = len(y)

    # Объединение выборок
    all_values = np.concatenate([x, y])

    # Ранжирование значений
    ranked = np.argsort(all_values)
    ranks = np.empty_like(ranked)
    ranks[ranked] = np.arange(1, len(all_values) + 1)

    # Сумма рангов одной из выборок
    rank_sum_x = np.sum(ranks[:n1])

    # Статистика критерия
    W = min(rank_sum_x, n1 * n2 - rank_sum_x)

    # Расчет p-значения (вручную, без использования scipy)
    z = (W - n1 * n2 / 2) / np.sqrt(n1 * n2 * (n1 + n2 + 1) / 12)
    p_value = 2 * (1 - 0.5 * (1 + np.sign(z) * np.exp(-2 * z ** 2)))

    return W, p_value


def iman_approximation(x, y):
    """
    Приближение Имана
    """
    n1 = len(x)
    n2 = len(y)
    ranks = np.concatenate([x, y])
    T = np.sum(ranks[:n1]) - (n1 * (n1 + 1)) / 2
    U = n1 * n2 - T
    W = min(T, U)
    return W


def bootstrap_ci_delta(x, y, alpha=0.05, n_bootstrap=1000):
    """
    Построение доверительного интервала для delta с помощью бутстрэпа
    """
    differences = np.array(
        [np.mean(np.random.choice(x, len(x))) - np.mean(np.random.choice(y, len(y))) for _ in range(n_bootstrap)])
    lower_bound = np.percentile(differences, 100 * alpha / 2)
    upper_bound = np.percentile(differences, 100 * (1 - alpha / 2))
    return lower_bound, upper_bound


# Данные
method1_results = np.array([3.78, 3.34, 4.71, 5.15, 4.16, 3.46, 4.18, 4.97, 4.02, 2.62])
method2_results = np.array([3.82, 3.87, 2.8, 4.02, 3.78, 4.89, 3.91, 3.56, 2.56, 4.01])

# Критерий ранговых сумм Уилкоксона
W, p_value = wilcoxon_rank_sum_test(method1_results, method2_results)
print("Критерий ранговых сумм Уилкоксона:")
print("W =", W)
print("p-значение =", p_value)

# Приближение Имана
W_iman = iman_approximation(method1_results, method2_results)
print("\nПриближение Имана:")
print("W =", W_iman)

# Доверительный интервал для delta
lower_bound, upper_bound = bootstrap_ci_delta(method1_results, method2_results)
print("\nДоверительный интервал для delta:")
print("Нижняя граница =", lower_bound)
print("Верхняя граница =", upper_bound)

# Оценка размера эффекта (разница между средними)
effect_size = np.mean(method1_results) - np.mean(method2_results)
print("\nРазмер эффекта (разница между средними):", effect_size)