import math

x = float(input("Введите точку, в которой необходимо вычислить производную sin(x): "))
h = float(input("Введите шаг для численного дифференцирования: "))

# Вычисляем производную синуса при помощи численного дифференцирования
def derivative_sin(x, h):

    derivative = (math.sin(x + h) - math.sin(x - h)) / (2 * h)   # Используем формулу центральной разности для численного дифференцирования

    return derivative


result = derivative_sin(x, h)

print(f"Производная sin({x}) = {result}")