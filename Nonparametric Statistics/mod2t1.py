#Сравнивается урожайность кукурузы на полях, обработанных разным
#и агротехническими методами. Провести сравнение обработок с помощью критерия
#Краскела–Уоллиса и статистики Имана–Давенпорта. Оценить размер эффекта с помощью статистик θ – Фримана, η^2 и ε^2
#Провести сравнение обработок с контролем (метод 1), используя асимптотический метод

import numpy as np


def kruskal_wallis_test(*samples):
    """
    Критерий Краскела-Уоллиса для сравнения нескольких групп
    """
    k = len(samples)
    n = np.array([len(sample) for sample in samples])

    # Ранжирование значений в каждой группе
    ranks = [np.argsort(sample) for sample in samples]

    # Рассчитываем средние ранги для каждой группы
    mean_ranks = [np.mean(rank + 1) for rank in ranks]

    # Общее количество элементов
    N = np.sum(n)

    # Сумма квадратов отклонений рангов от среднего ранга
    SS_total = np.sum([(ranks[i] - mean_ranks[i]) ** 2 for i in range(k)])

    # Сумма квадратов отклонений средних рангов от общего среднего
    grand_mean = np.sum(np.concatenate(ranks) + 1) / N
    SS_between = np.sum([n[i] * (mean_ranks[i] - grand_mean) ** 2 for i in range(k)])

    # Остаточная сумма квадратов
    SS_within = SS_total - SS_between

    # Статистика критерия
    H = (12 / (N * (N + 1))) * SS_between - 3 * (N + 1)

    # Расчет p-значения
    p_value = 1 - chi2_cdf_approx(H, k - 1)

    return H, p_value


def chi2_cdf_approx(x, k):
    """
    Приближенное значение кумулятивной функции распределения хи-квадрат
    """
    # Приближенная формула для вычисления p-значения
    if x > 0.01:
        p_value = np.exp(-0.5772 - k * np.log(x / k))
    else:
        p_value = np.exp(-np.pi ** 2 / 6)

    return p_value


#Данные
method1_yield = np.array([83, 91, 94, 89, 89, 96])
method2_yield = np.array([91, 90, 81, 83, 84, 83])
method3_yield = np.array([101, 100, 91, 93, 96, 95])

# Критерий Краскела-Уоллиса
H, p_value_kruskal = kruskal_wallis_test(method1_yield, method2_yield, method3_yield)
print("Статистика критерия Краскела-Уоллиса:", H)
print("p-значение критерия Краскела-Уоллиса:", p_value_kruskal)