import numpy as np
from matplotlib import pyplot as plt


print("Введите число узлов:")
numOfNodes = int(input())
print("Введите первый узел:")
L = float(input())
print("Введите последний узел:")
R = float(input())
print("Введите аргумент функции:")
ArgFunk = float(input())

# Изначальная функция
f = np.sin


#Получение границ и значений Х и У
def generateData(func, L, R, numOfNodes):
    x_data = np.linspace(L, R, numOfNodes)
    y_data = func(x_data)
    return x_data, y_data


#Получение кусочно-линейной интерполяции
def GetPieLinInterpollation(point, x_data, y_data):
    leftBord = 0
    for i in range(1, numOfNodes):
        if x_data[i] > point:
            leftBord = i - 1
            break

    v_int = y_data[leftBord] + ((y_data[leftBord + 1] - y_data[leftBord]) / (x_data[leftBord + 1] - x_data[leftBord])) * (point - x_data[leftBord])

    return v_int


x_Plot = np.linspace(0, np.pi, 100)
y_Plot = f(x_Plot)

plotObj = plt
x_data, y_data = generateData(f, L, R, numOfNodes)
result = GetPieLinInterpollation(ArgFunk, x_data, y_data)

# Вывод таблицы данных
print("x    |   y")
print("--------------------------")
for i in range(len(x_data)):
    print(f"{x_data[i]:.1f}  |  {y_data[i]}")

print('Погрешность интерполяции: ', f(ArgFunk) - result)

# Функция для настройки графика
def plotGeneration(plotObj, x_Plot, y_Plot, point_x, point_y):
    plotObj.plot(x_Plot, y_Plot, linewidth = 0.5, label="Изначальная функция")
    plotObj.scatter(point_x, point_y, color = "red", label="Значение Интерполяции")
    plotObj.xlabel('X')
    plotObj.ylabel('Y')


plotGeneration(plotObj, x_Plot, y_Plot, ArgFunk, result)
plotObj.legend()
plotObj.show()