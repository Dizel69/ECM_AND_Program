# Программа по нахождению производной синуса в определённой точке, и вычислении погрешности
import math
import sympy as sp
x = float(input("Введите точку, в которой необходимо вычислить производную sin(x): "))
h = float(input("Введите шаг для численного дифференцирования: "))

 # Вычисляем производную синуса при помощи численного дифференцирования
def derivative_sin(x, h):

    derivative = (math.sin(x + h) - math.sin(x - h)) / (2 * h)   # Используем формулу центральной разности для численного дифференцирования

    return derivative


result = derivative_sin(x, h)

# Вычисление производной синуса при помощи библиотек
X = sp.Symbol('x')
derivative = sp.diff(sp.sin(X), X)

derivative_value = derivative.subs(X, x)


print(f"Производная sin({x}) = {result}, с помощью формулы")
print(f"Производная sin({x}) = {derivative_value}, с помощью библиотеки")

fault = result - derivative_value # Вычисление погрешности

print("Погрешность = ", fault)