import numpy as np
import matplotlib
matplotlib.use('TkAgg')  # Устанавливаем бэкенд для отображения
import matplotlib.pyplot as plt


# Параметры
l = 1.0      # Длина струны
R = 1.0      # Параметр R
a = 1.0      # Скорость волны
T = 10       # Время моделирования
dx = 0.2     # Шаг по пространству (увеличен для наглядности вывода)
dt = 0.1     # Шаг по времени

# Количество шагов
nx = int(l / dx) + 1
nt = int(T / dt)

# Инициализация массива
u = np.zeros((nx, nt))

# Начальные условия
for i in range(nx):
    x = i * dx
    u[i, 0] = (4 * R / l**2) * x * (l - x)

# Граничные условия
u[0, :] = 0
u[-1, :] = 0

# Явная схема
for n in range(0, nt - 1):
    for i in range(1, nx - 1):
        u[i, n + 1] = (a**2 * dt**2 / dx**2) * (u[i + 1, n] - 2 * u[i, n] + u[i - 1, n]) + 2 * u[i, n] - u[i, n - 1]

# Вывод в виде таблицы
print("  x/t |", end="")
for n in range(nt):
    print(f" {n*dt:.2f} ", end="")
print()

print("-" * (7 + nt * 7))  # Разделитель

for i in range(nx):
    print(f"{i*dx:.2f} |", end="")
    for n in range(nt):
        print(f" {u[i, n]:.2f} ", end="")
    print()

# Визуализация результатов
plt.figure(figsize=(10, 6))

# Выбор нескольких временных шагов для отображения
for n in range(0, nt, 10):  # Каждый 10-й временной шаг
    plt.plot(np.linspace(0, l, nx), u[:, n], label=f't={n*dt:.2f}s')

plt.title('Решение уравнения колебания струны')
plt.xlabel('Положение x (м)')
plt.ylabel('u(x,t)')
plt.legend()
plt.grid()
plt.show()
plt.savefig('wave_equation_solution.png')
