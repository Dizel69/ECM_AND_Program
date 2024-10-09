import numpy as np
import matplotlib.pyplot as plt

# Параметры
R = 1.0         # Радиус
a = 1.0         # Скорость волны
l = 1.0         # Длина струны
tau = 0.05       # Шаг по времени
h = 0.125        # Шаг по пространству
x_max = l       # Максимальное значение x
t_max = 4       # Максимальное время
n_x = int(x_max / h) + 1  # Количество точек по x
n_t = 9  # Ограничиваем временные шаги до 4

# Функция f(i, j) - считаем, что она равна нулю
def f(i, j):
    return 0

# Начальные условия u_0(x_i)
def u0(x):
    return (4 * R / l**2) * x * (l - x)  # Начальное распределение u(x, 0)

# Точное решение для сравнения
def exact_solution(x, t, a=1.0, l=1.0):
    exact = 0
    for n in range(50):  # Ограничиваем сумму ряда до 50 членов
        term = ((-1)**n) * np.sin((2 * n + 1) * np.pi * x / l) * np.cos((2 * n + 1) * np.pi * a * t / l) / (2 * n + 1)**3
        exact += term
    return 32 * R / np.pi**3 * exact

# Сетка для хранения решений
u = np.zeros((n_x, n_t))

# Начальное распределение u(i, 0)
for i in range(n_x):
    u[i, 0] = u0(i * h)

# Граничные условия: u(0, t) = u(l, t) = 0
u[0, :] = 0
u[-1, :] = 0

# Численное решение с использованием явной схемы
for i in range(1, n_x - 1):
    u[i, 1] = u[i, 0] + tau * 0

for j in range(1, n_t - 1):
    for i in range(1, n_x - 1):
        u[i, j + 1] = 2 * u[i, j] - u[i, j - 1] + ((tau ** 2 * a ** 2) / h ** 2) * (u[i - 1, j] - 2 * u[i, j] + u[i + 1, j])

# Вывод численного решения в таблицу
print("Численное решение u(i,j):")
print(f"{'i\\j':<7}", end="")
for j in range(n_t):
    print(f"{j:<7}", end="")
print()

for i in range(n_x):
    print(f"{i:<7}", end="")
    for j in range(n_t):
        print(f"{u[i, j]:<7.3f}", end="")
    print()

# Вывод точного решения в таблицу для сравнения
print("\nТочное решение u_exact(i,j):")
print(f"{'i\\j':<7}", end="")
for j in range(n_t):
    print(f"{j:<7}", end="")
print()

for i in range(n_x):
    print(f"{i:<7}", end="")
    for j in range(n_t):
        x = i * h
        t = j * tau
        print(f"{exact_solution(x, t):<7.3f}", end="")
    print()

# Визуализация численного и точного решения
plt.figure(figsize=(10, 6))

# Отображение численного решения для каждого временного шага
for j in range(n_t):
    plt.plot(np.linspace(0, x_max, n_x), u[:, j], label=f'Численное t={j * tau:.2f}s')

# Отображение точного решения для тех же временных шагов
for j in range(n_t):
    plt.plot(np.linspace(0, x_max, n_x), [exact_solution(x, j * tau) for x in np.linspace(0, x_max, n_x)], '--', label=f'Точное t={j * tau:.2f}s')

plt.title('Сравнение численного и точного решений уравнения колебания струны')
plt.xlabel('Положение x (м)')
plt.ylabel('u(x,t)')
plt.legend()
plt.grid()
plt.show()
