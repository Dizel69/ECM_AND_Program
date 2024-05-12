# 50Вариант. Решение задачи Коши для систем дифференциальных уравнений 1-ого порядка модифицированным методом Эйлера
import numpy as np

#вычисляется значение функции v по заданной формуле, использующей переменные x и y.
def f(x, y):
    v = y - 2 * x * x + 1;
    return v;

#предсказывает следующее значение y при данном шаге h, используя результат функции f(x, y).
def predict(x, y, h):
    y1p = y + h * f(x, y);
    return y1p;

#исправляет предсказанное значение y1 с учетом функции f для двух точек.
def correct(x, y, x1, y1, h):
    e = 0.00001;
    y1c = y1;
    while (abs(y1c - y1) > e + 1):
        y1 = y1c;
        y1c = y + 0.5 * h * (f(x, y) + f(x1, y1));
    return y1c;

#вычисляет и выводит итоговое значение y при каждом шаге x, используя предыдущие функции.
def printFinalValues(x, xn, y, h):
    while (x < xn):
        x1 = x + h;
        y1p = predict(x, y, h);
        y1c = correct(x, y, x1, y1p, h);
        x = x1;
        y = y1c;


    print("Значение y в точке x = ",
          int(x), ",равно:", y);



x = float(input("Введите x: "));
y = float(input("Введите y: "));
xn = float(input("Введите конечную точку: "));
h = float(input("Введите шаг: "));

printFinalValues(x, xn, y, h);