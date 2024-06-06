#В данной программе вычисляется критерий Фишера-Йейтса-Терри-Гёрфдинга для k>2 выборок
import numpy as np

# Первый пример
#x1 = [75, 78, 74, 76, 77, 80, 72, 74, 75, 79, 73, 77, 76, 75, 78]
#x2 = [80, 82, 85, 81, 84, 86, 83, 85, 82, 84, 81, 83, 82, 80, 85, 81, 85]
#x3 = [77, 79, 78, 81, 80, 82, 79, 81, 80, 78, 79, 81, 80, 82, 80]
# Второй пример
#x1 = [70, 75, 80, 85, 72, 77, 82, 78, 76, 79, 81, 73, 74]
#x2 = [85, 88, 87, 86, 90, 84, 89, 83, 86, 85, 88, 87]
#x3 = [90, 91, 92, 89, 93, 91, 92, 90, 91, 93, 92]
# Третий пример
#x1 = [60, 65, 62, 64, 66, 63, 67, 61, 68, 62, 64]
#x2 = [70, 73, 72, 74, 71, 75, 73, 70, 72, 74, 90]
#x3 = [78, 80, 79, 81, 77, 82, 80, 79, 81, 78, 80]
# четвертый пример
x1 = [56, 60, 62, 65, 70, 68, 72, 75, 77, 80]
x2 = [61, 64, 67, 69, 73, 76, 78, 79, 81, 84]
x3 = [58, 62, 66, 70, 74, 77, 79, 80, 83, 86, 88, 90]


combined = sorted(x1 + x2 + x3)
N = len(combined)

# Функция для вычисления значений a_N(i)
def compute_an_value(i, N):
    p = (i - 3/8) / (N + 1/4)
    an = 4.91 * ((p ** 0.14) - ((1 - p) ** 0.14))
    return an

ranks = [0 for i in range(len(combined))]


T = [(combined[i], i) for i in range(len(combined))]

# T[][0] is the data and T[][1] is
# the index of data in A

    # Sort T according to first element
T.sort(key=lambda x: x[0])

(rank, n, i) = (1, 1, 0)

while i < len(combined):
    j = i

        # Get no of elements with equal rank
    while j < len(combined) - 1 and T[j][0] == T[j + 1][0]:
        j += 1
    n = j - i + 1

    for j in range(n):
            # For each equal element use formula
            # obtain index of T[i+j][0] in A
        idx = T[i + j][1]
        ranks[idx] = rank + (n - 1) * 0.5

        # Increment rank and i
    rank += n
    i += n

# Вычисление значений a_N(i) для всех i от 1 до N
an_values = [compute_an_value(i, N) for i in range(1, N)]

# Функция для получения рангов выборки
def get_ranks(sample, combined, ranks):
    return [ranks[combined.index(x)] for x in sample]

# Получаем ранги для каждой выборки
ranks_x1 = get_ranks(x1, combined, ranks)
ranks_x2 = get_ranks(x2, combined, ranks)
ranks_x3 = get_ranks(x3, combined, ranks)

def compute_Q(*samples):
    sum_an_rj_x1 = 0
    sum_an_rj_x2 = 0
    sum_an_rj_x3 = 0

    for i in ranks_x1:
        sum_an_rj_x1 += compute_an_value(i, N)
    for i in ranks_x2:
        sum_an_rj_x2 += compute_an_value(i, N)
    for i in ranks_x3:
        sum_an_rj_x3 += compute_an_value(i, N)

    # Сумма всех квадратов
    sum_all_squared = 0
    for i in ranks:
        sum_all_squared += compute_an_value(i, N) ** 2


    # Считаем общую сумму для статистики Q
    sum_nj_an_rj = ((1 / len(x1)) * sum_an_rj_x1**2) + ((1 / len(x2)) * sum_an_rj_x2**2) + ((1 / len(x3)) * sum_an_rj_x3**2)

    # Вычисление статистики Q
    Q = (1/sum_all_squared) * ((N - 1) * (sum_nj_an_rj))
    return Q

# Таблица критических значений χ² для различных уровней значимости и степеней свободы
chi2_critical_values = {
    (0.05, 1): 3.841,
    (0.05, 2): 5.991,
    (0.05, 3): 7.815,
    (0.05, 4): 9.488,
    (0.05, 5): 11.070,
    (0.01, 1): 6.635,
    (0.01, 2): 9.210,
    (0.01, 3): 11.345,
    (0.01, 4): 13.277,
    (0.01, 5): 15.086,
}


# Функция для получения критического значения χ² из таблицы
def get_chi2_critical_value(alpha, df):
    return chi2_critical_values.get((alpha, df), None)


# Точный критерий
def exact_critical_value(samples, alpha):
    combined = np.concatenate(samples)
    sample_sizes = [len(sample) for sample in samples]
    combined_size = sum(sample_sizes)

    all_permutations = permutations(np.arange(1, combined_size + 1))
    Q_values = []

    for perm in all_permutations:
        perm_samples = []
        start = 0
        for size in sample_sizes:
            perm_samples.append(np.array(perm[start:start + size]))
            start += size
        Q_values.append(compute_Q(*perm_samples))

    Q_values = np.array(Q_values)
    critical_value = np.percentile(Q_values, 100 * (1 - alpha))
    return critical_value


# Имитационный подход
def simulation_critical_value(samples, alpha, simulations=1000):
    combined = np.concatenate(samples)
    sample_sizes = [len(sample) for sample in samples]
    combined_size = sum(sample_sizes)
    Q_values = []

    for _ in range(simulations):
        np.random.shuffle(combined)
        perm_samples = []
        start = 0
        for size in sample_sizes:
            perm_samples.append(combined[start:start + size])
            start += size
        Q_values.append(compute_Q(*perm_samples))

    Q_values = np.array(Q_values)
    critical_value = np.percentile(Q_values, 100 * (1 - alpha))
    return critical_value


# Основная функция для проверки гипотезы
def fisher_yates_terry_gehfting_test(alpha=0.05, simulations=1000, *samples):
    Q = compute_Q(*samples)
    sample_sizes = [len(sample) for sample in samples]
    k = len(samples)

    # Точный критерий
    if np.prod(sample_sizes) <= 10:
        critical_value_exact = exact_critical_value(samples, alpha)
        reject_null_hypothesis_exact = Q > critical_value_exact
        result_exact = "отвергается" if reject_null_hypothesis_exact else "принимается"
    else:
        result_exact = "невозможно вычислить (слишком большие выборки)"

    # Асимптотический критерий
    chi2_critical_value = get_chi2_critical_value(alpha, k - 1)
    if chi2_critical_value is None:
        raise ValueError(f"Критическое значение для alpha={alpha} и df={k - 1} не найдено в таблице.")
    reject_null_hypothesis_asymptotic = Q > chi2_critical_value
    result_asymptotic = "отвергается" if reject_null_hypothesis_asymptotic else "принимается"

    # Имитационный подход
    critical_value_sim = simulation_critical_value(samples, alpha, simulations)
    reject_null_hypothesis_sim = Q > critical_value_sim
    result_simulation = "отвергается" if reject_null_hypothesis_sim else "принимается"

    # Вывод результатов
    print("Статистика Q:", Q)
    if np.prod(sample_sizes) <= 10:
        print("Точное критическое значение:", critical_value_exact)
        print("Гипотеза по точному критерию:", result_exact)
    else:
        print("Точное критическое значение: не вычислено")
    print("Асимптотическое критическое значение:", chi2_critical_value)
    print("Гипотеза по асимптотическому критерию:", result_asymptotic)
    print("Критическое значение методом имитации:", critical_value_sim)
    print("Гипотеза по методу имитации:", result_simulation)


fisher_yates_terry_gehfting_test(0.05, 1000, x1,x2,x3)