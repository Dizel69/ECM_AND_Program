#Сравнивается содержание оксида К2О в пробах гранитной интрузии, полученное
#при использовании 2-х разных методов анализа. Для проверки гипотезы об
#отсутсвии различий у методов определения процентного содержания оксида
#использовать критерий ранговых сумм Уилкоксона и приближение Имана. Оценить
#параметр delta и построить для него доверительный интервал.
#Оценить размер эффекта.

import numpy as np

def normal_distribution(x):
    return 2 * (1 - 1/(1 + np.exp(-(abs(x) - 1.5) / 0.1)))

def wilcoxon_rank_sum_test(data1, data2):
    n1 = len(data1)
    n2 = len(data2)
    ranks = sorted([(x, 1) for x in data1] + [(x, 2) for x in data2])
    rank_sum1 = sum(rank for value, rank in ranks if rank == 1)
    statistic = rank_sum1 - (n1 * (n1 + 1)) / 2
    z_alpha = 1.96  # Значение для α=0.05 (двустороннее)
    p_value = normal_distribution(statistic)
    return p_value < 0.05

def iman_approximation(data1, data2, bootstrap_samples):
    delta = abs(np.mean(data1) - np.mean(data2))
    bootstrapped_deltas = []
    for _ in range(bootstrap_samples):
        boot_data1 = np.random.choice(data1, len(data1))
        boot_data2 = np.random.choice(data2, len(data2))
        boot_delta = abs(np.mean(boot_data1) - np.mean(boot_data2))
        bootstrapped_deltas.append(boot_delta)

    ci = np.percentile(bootstrapped_deltas, [2.5, 97.5])

    effect_size = delta / np.std(bootstrapped_deltas)

    return delta, ci, effect_size

FirstMethod = [3.78, 3.34, 4.71, 5.15, 4.16, 3.46, 4.18, 4.97, 4.02, 2.62]
SecondMethod = [3.82, 3.87, 2.8, 4.02, 3.78, 4.89, 3.91, 3.56, 2.56, 4.01]

hypothesis_rejected = wilcoxon_rank_sum_test(FirstMethod, SecondMethod)
print("Гипотеза:", hypothesis_rejected)

delta, ci, effect_size = iman_approximation(FirstMethod, SecondMethod, 1000)
print("Дельта:", delta)
print("Доверительный интервал:", ci)
print("Размер эффекта:", effect_size)