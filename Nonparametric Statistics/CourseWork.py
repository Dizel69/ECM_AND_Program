# Сериальный критерий Вальда–Вольфовица

import numpy as np


def wald_wolfowitz_test(data):
    n = len(data)
    num_ones = np.sum(data)
    num_zeroes = n - num_ones

    # Точный критерий
    def exact_test():
        runs = 1
        for i in range(1, n):
            if data[i] != data[i - 1]:
                runs += 1
        return runs

    # Асимптотический критерий
    def asymptotic_test():
        mu = (2 * num_zeroes * num_ones) / n + 1
        sigma = np.sqrt((mu - 1) * (mu - 2) / (n - 1))
        z = (num_runs - mu) / sigma
        return z

    # Имитационный подход
    def simulation_test(num_simulations=10000):
        simulated_runs = []
        for _ in range(num_simulations):
            simulated_data = np.random.choice([0, 1], size=n, replace=True)
            simulated_runs.append(exact_test())
        simulated_runs = np.array(simulated_runs)
        p_value = np.mean(simulated_runs >= num_runs)
        return p_value

    num_runs = exact_test()
    z = asymptotic_test()
    p_value = simulation_test()

    return num_runs, z, p_value


# Пример использования
#data = np.array([0, 1, 0, 1, 1, 0, 0, 1, 1, 1])
#data = np.random.choice([0, 1], size=100, p=[0.3, 0.7])
data = np.random.randint(0, 2, size=50)
#data = np.array([1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1])
num_runs, z, p_value = wald_wolfowitz_test(data)
print("Number of runs:", num_runs)
print("Z-score:", z)
print("P-value (simulation):", p_value)