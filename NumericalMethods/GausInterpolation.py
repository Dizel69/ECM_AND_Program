#Первая интерполяционная формула Гаусса для равноотстоящих узлов
import numpy as np

#Вводим числа
numOfNodes = int(input("Введите число узлов: "))
L = float(input("Введите первый узел: "))
R = float(input("Введите последний узел: "))
ArgFunk = float(input("Введите аргумент функции: "))

f = lambda x: 2.7**x

#Получение границ и значений Х и У
def generateData(func, L, R, numOfNodes):
    x_data = np.linspace(L, R, numOfNodes)
    y_data = func(x_data)
    return x_data, y_data

def gauss_interpolation(x_data, y_data, ArgFunk):

    # Находим шаг между соседними узлами
    h = x_data[1] - x_data[0]

    # Находим индекс ближайшего узла x к ArgFunk
    nearest_index = round(ArgFunk)

    # Находим x-координаты двух ближайших узлов
    x0 = x_data[nearest_index]
    x1 = x_data[nearest_index + 1]

    # Находим значения функции в двух ближайших узлах
    y0 = y_data[nearest_index]
    y1 = y_data[nearest_index + 1]

    # Вычисляем значение интерполированной функции в desired_x
    interpolated_y = y0 + ((ArgFunk - x0) * (y1 - y0) / (x1 - x0))

    return interpolated_y

x_data, y_data = generateData(f, L, R, numOfNodes)
interpolated_y = gauss_interpolation(x_data, y_data, ArgFunk)

# Вывод таблицы данных
print("x    |   y")
print("--------------------------")
for i in range(len(x_data)):
    print(f"{x_data[i]:.1f}  |  {y_data[i]}")
    
print(f"Значение функции в точке x={ArgFunk}: {interpolated_y}")