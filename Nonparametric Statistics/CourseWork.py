import numpy as np
from itertools import permutations, combinations


# Функция для вычисления статистики S
def compute_S(*samples):
    combined = np.sort(np.concatenate(samples))
    ranks = np.argsort(np.argsort(combined)) + 1
    sample_sizes = [len(sample) for sample in samples]
    combined_size = sum(sample_sizes)
    an_values = []

    for i, sample in enumerate(samples):
        R = ranks[sum(sample_sizes[:i]):sum(sample_sizes[:i + 1])]  # ранги элементов из текущей выборки
        p = (R - 3 / 8) / (combined_size + 1 / 4)
        an = 4.91 * (p ** 0.14) * ((1 - p) ** 0.14)
        an_values.append(np.sum(an))

    S = np.sum(an_values)
    return S


# Функция для асимптотического распределения
def asymptotic_critical_value(alpha, *sample_sizes):
    combined_size = sum(sample_sizes)
    p = (np.arange(1, sample_sizes[0] + 1) - 3 / 8) / (combined_size + 1 / 4)
    an = 4.91 * p ** 0.14 * (1 - p) ** 0.14
    D_S = (np.prod(sample_sizes) / combined_size / (combined_size - 1)) * np.sum(an ** 2)
    u_alpha = np.sqrt(2) * np.sqrt(2) * erfinv(1 - alpha)  # приближение к квантилю нормального распределения
    return u_alpha * np.sqrt(D_S)


# Реализация функции erfinv
def erfinv(y):
    a = 0.147  # Константа для аппроксимации
    ln_part = np.log(1 - y ** 2)
    sqrt_part = np.sqrt(ln_part ** 2 - (2 * a) / (np.pi * (1 - 2 / (np.pi * a * ln_part + 2))))
    result = np.sign(y) * np.sqrt(sqrt_part - ln_part)
    return result


# Функция для точного критерия
def exact_critical_value(samples, alpha):
    combined = np.concatenate(samples)
    sample_sizes = [len(sample) for sample in samples]
    combined_size = sum(sample_sizes)

    # Генерация всех возможных перестановок
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


# Пример использования
x = np.random.normal(0, 1, 3)
y = np.random.normal(0.5, 1, 3)
z = np.random.normal(0.2, 1, 3)
fisher_yates_terry_gehfting_test(0.05, 1000, x, y, z)