#Экспертным методом оценивается конкурентоспособность сливочного
#масла, реализуемого в розничной торговой сети. Опираясь на коэффициент конкордации Кендалла, проверить
#согласованность экспертных оценок. Построить доверительные интервалы для W на основе симуляции и
#нецентрального хи^2–распределения.

import numpy as np


def kendall_concordance(X):
    """
    Вычисляет коэффициент конкордации Кендалла для данного набора оценок.

    Аргументы:
    X -- numpy массив размерности (n, p), где n - количество экспертов, p - количество критериев.

    Возвращает:
    W -- коэффициент конкордации Кендалла.
    """
    n, p = X.shape
    concordant = np.sum(np.fromiter(
        ((X[i, k] - X[j, k]) * (X[i, l] - X[j, l]) > 0 for i in range(n) for j in range(i + 1, n) for k in range(p) for
         l in range(p)), float))
    discordant = np.sum(np.fromiter(
        ((X[i, k] - X[j, k]) * (X[i, l] - X[j, l]) < 0 for i in range(n) for j in range(i + 1, n) for k in range(p) for
         l in range(p)), float))
    W = (concordant - discordant) / (0.5 * n * (n - 1))
    return W

def kendall_bootstrap_CI(X, B=1000, alpha=0.05):
    """
    Вычисляет доверительный интервал для коэффициента конкордации Кендалла методом бутстрэпа.

    Аргументы:
    X -- numpy массив размерности (n, p), где n - количество экспертов, p - количество критериев.
    B -- количество бутстрэп-переключений (по умолчанию 1000).
    alpha -- уровень значимости (по умолчанию 0.05).

    Возвращает:
    CI -- доверительный интервал для коэффициента конкордации Кендалла.
    """
    n, p = X.shape
    W_observed = kendall_concordance(X)
    W_boot = np.zeros(B)
    for b in range(B):
        resampled_indices = np.random.choice(np.arange(n), size=n, replace=True)
        X_resampled = X[resampled_indices]
        W_boot[b] = kendall_concordance(X_resampled)
    lower_bound = np.percentile(W_boot, 100 * (alpha / 2))
    upper_bound = np.percentile(W_boot, 100 * (1 - alpha / 2))
    return lower_bound, upper_bound

# Пример данных (замените их своими данными)
X = np.array([
    [8, 7, 4, 1, 5, 3, 1, 6],
    [8, 7, 3, 2, 4, 5, 1, 6],
    [7, 8, 5, 1, 3, 4, 2, 6],
    [7, 8, 3, 2, 4, 6, 1, 5]
])

# Вычисление коэффициента конкордации Кендалла
W = kendall_concordance(X)
print("Коэффициент конкордации Кендалла:", W)

# Вычисление доверительного интервала методом бутстрэпа
CI = kendall_bootstrap_CI(X)
print("Доверительный интервал для W:", CI)