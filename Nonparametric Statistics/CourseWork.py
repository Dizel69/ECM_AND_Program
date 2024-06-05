#В данной программе вычисляется критерий Фишера-Йейтса-Терри-Гёрфдинга
import numpy as np
from scipy.special import erfinv
from itertools import combinations

# Функция для вычисления значений a_{m+n}(i)
def compute_an_values(m, n):
    combined_size = m + n
    i = np.arange(1, combined_size)
    p = (i - 3/8) / (combined_size + 1/4)
    an = 4.91 * (np.power(p, 0.14) - np.power(1 - p, 0.14))
    return an

# Функция для вычисления статистики S
def compute_S(*samples):
    combined = np.concatenate(samples)
    ranks = np.argsort(np.argsort(combined)) + 1
    sample_sizes = [len(sample) for sample in samples]

    m = sample_sizes[0]
    n = sample_sizes[1]
    an_values = compute_an_values(m, n)

    S = 0
    for i, sample in enumerate(samples):
        R = ranks[sum(sample_sizes[:i]):sum(sample_sizes[:i + 1])]  # ранги элементов из текущей выборки
        R = np.clip(R, 1, len(an_values))  # Убедимся, что ранги не выходят за пределы
        S += np.sum(an_values[R - 1])

    return S

# Функция для асимптотического распределения
def asymptotic_critical_value(alpha, *sample_sizes):
    combined_size = sum(sample_sizes)
    m, n = sample_sizes[0], sum(sample_sizes[1:])
    an_values = compute_an_values(m, n)
    D_S = (m * n / (combined_size * (combined_size - 1))) * np.sum(an_values ** 2)
    u_alpha = erfinv(1 - alpha) * np.sqrt(2)
    return u_alpha * np.sqrt(D_S)

# Функция для точного критерия
def exact_critical_value(samples, alpha):
    combined = np.concatenate(samples)
    sample_sizes = [len(sample) for sample in samples]
    combined_size = sum(sample_sizes)

    indices = np.arange(combined_size)
    all_combinations = list(combinations(indices, sample_sizes[0]))
    S_values = []

    for comb in all_combinations:
        remaining_indices = np.setdiff1d(indices, comb)
        remaining_combinations = list(combinations(remaining_indices, sample_sizes[1]))
        for rem_comb in remaining_combinations:
            final_indices = np.setdiff1d(remaining_indices, rem_comb)
            perm_samples = [
                combined[list(comb)],
                combined[list(rem_comb)],
                combined[list(final_indices)]
            ]
            S_values.append(compute_S(*perm_samples))

    S_values = np.array(S_values)
    critical_value = np.percentile(S_values, 100 * (1 - alpha / 2))
    return critical_value

# Основная функция для проверки гипотезы
def fisher_yates_terry_gehfting_test(alpha=0.05, simulations=1000, *samples):
    S = compute_S(*samples)
    sample_sizes = [len(sample) for sample in samples]

    # Метод имитации
    S_sim = []
    combined = np.concatenate(samples)
    for _ in range(simulations):
        np.random.shuffle(combined)
        sim_samples = [combined[sum(sample_sizes[:i]):sum(sample_sizes[:i + 1])] for i in range(len(samples))]
        S_sim.append(compute_S(*sim_samples))
    critical_value_sim = np.percentile(S_sim, 100 * (1 - alpha / 2))

    # Точный критерий
    if np.prod(sample_sizes) <= 1000:  # Порог для вычислений точного критерия, можно подстроить
        critical_value_exact = exact_critical_value(samples, alpha)
        reject_null_hypothesis_exact = np.abs(S) > critical_value_exact
        result_exact = "отвергается" if reject_null_hypothesis_exact else "принимается"
    else:
        result_exact = "невозможно вычислить (слишком большие выборки)"

    # Асимптотический критерий
    asymptotic_cv = asymptotic_critical_value(alpha, *sample_sizes)

    # Проверка гипотезы
    reject_null_hypothesis_asymptotic = np.abs(S) > asymptotic_cv
    result_asymptotic = "отвергается" if reject_null_hypothesis_asymptotic else "принимается"

    # Проверка гипотезы методом имитации
    reject_null_hypothesis_sim = np.abs(S) > critical_value_sim
    result_simulation = "отвергается" if reject_null_hypothesis_sim else "принимается"

    # Вывод результатов
    print("Статистика S:", S)
    if np.prod(sample_sizes) <= 1000:
        print("Точное критическое значение:", critical_value_exact)
        print("Гипотеза по точному критерию:", result_exact)
    else:
        print("Точное критическое значение: не вычислено")
    print("Асимптотическое критическое значение:", asymptotic_cv)
    print("Гипотеза по асимптотическому критерию:", result_asymptotic)
    print("Критическое значение методом имитации:", critical_value_sim)
    print("Гипотеза по методу имитации:", result_simulation)

# Первый пример
#x = [75, 78, 74, 76, 77, 80, 72, 74, 75, 79, 73, 77, 76, 75, 78]
#y = [80, 82, 85, 81, 84, 86, 83, 85, 82, 84, 81, 83, 82, 80, 85, 81, 85]
#z = [77, 79, 78, 81, 80, 82, 79, 81, 80, 78, 79, 81, 80, 82, 80]
# Второй пример
#x = [70, 75, 80, 85, 72, 77, 82, 78, 76, 79, 81, 73, 74]
#y = [85, 88, 87, 86, 90, 84, 89, 83, 86, 85, 88, 87]
#z = [90, 91, 92, 89, 93, 91, 92, 90, 91, 93, 92]
# Третий пример
#x = [60, 65, 62, 64, 66, 63, 67, 61, 68, 62, 64]
#y = [70, 73, 72, 74, 71, 75, 73, 70, 72, 74, 90]
#z = [78, 80, 79, 81, 77, 82, 80, 79, 81, 78, 80]
# четвертый пример
x = [56, 60, 62, 65, 70, 68, 72, 75, 77, 80]
y = [61, 64, 67, 69, 73, 76, 78, 79, 81, 84]
z = [58, 62, 66, 70, 74, 77, 79, 80, 83, 86, 88, 90]

fisher_yates_terry_gehfting_test(0.05, 1000, x, y, z)
