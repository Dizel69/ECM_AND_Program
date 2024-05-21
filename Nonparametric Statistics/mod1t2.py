#В средней школе проведены тесты на определение уровня психологической готовности к
# вузовской системе обучения для мальчиков (группа A) и девочек
#(группа B). Исследования отвергли предположение о наличии существенного сдвига
#между выборками. Используя критерий Ансари–Бредли, определите, однородны ли
#эти выборки по степени разброса значений показателя
import numpy as np

def ansari_bradley_test(x, y):
    """
    Критерий Ансари-Бредли для проверки однородности дисперсий
    """
    n1 = len(x)
    n2 = len(y)

    # Расчет статистики критерия
    AB = (np.sum(np.abs(np.subtract.outer(x, y)))) / (n1 * n2 * (n1 + n2 + 1))

    return AB


def bootstrap_ab_test(x, y, alpha=0.05, n_bootstrap=1000):
    """
    Проведение бутстрэпа для оценки p-значения критерия Ансари-Бредли
    """
    observed_AB = ansari_bradley_test(x, y)
    combined_data = np.concatenate([x, y])
    n1 = len(x)
    n2 = len(y)
    count = 0

    for _ in range(n_bootstrap):
        np.random.shuffle(combined_data)
        AB = ansari_bradley_test(combined_data[:n1], combined_data[n1:])
        if AB >= observed_AB:
            count += 1

    p_value = count / n_bootstrap

    return p_value


# Данные
group_A = np.array([85, 64, 50, 78, 49, 96, 34, 58, 75, 54])
group_B = np.array([65, 78, 93, 55, 70, 90, 80, 79, 82, 67])

# Вычисление критерия Ансари-Бредли
AB_statistic = ansari_bradley_test(group_A, group_B)
print("Статистика критерия Ансари-Бредли:", AB_statistic)

# Проведение бутстрэпа для оценки p-значения
p_value = bootstrap_ab_test(group_A, group_B)
print("p-значение критерия Ансари-Бредли (бутстрэп):", p_value)