import pygame  # Импортируем модуль для работы с графикой
import math  # Импортируем модуль для математических функций (например, sin, cos)

# Инициализируем библиотеку Pygame
pygame.init()

# Задаем размеры окна
WIDTH, HEIGHT = 800, 600
# Создаем окно с заданными размерами
screen = pygame.display.set_mode((WIDTH, HEIGHT))
# Устанавливаем заголовок окна
pygame.display.set_caption("Эллипсы с регулируемыми скоростями")
# Создаем объект clock для управления частотой кадров
clock = pygame.time.Clock()

# Задаем размеры эллипсов (их полуоси)
a1, b1 = 200, 100  # a1 и b1 - полуоси первого (самого большого) эллипса
a2, b2 = 80, 40  # a2 и b2 - полуоси второго эллипса
a3, b3 = 40, 20  # a3 и b3 - полуоси третьего эллипса

# Задаем 5 параметров (скорости) в радианах за кадр и храним их в словаре speeds
speeds = {
    "rot1": 0.01,  # Скорость вращения первого эллипса (вокруг его центра)
    "rot2": 0.02,  # Скорость вращения второго эллипса (вокруг его собственного центра)
    "move2": 0.01,  # Скорость движения центра второго эллипса по границе первого эллипса
    "rot3": 0.03,  # Скорость вращения третьего эллипса (вокруг его собственного центра)
    "move3": 0.02  # Скорость движения центра третьего эллипса по границе второго эллипса
}

# Инициализируем углы для анимации эллипсов (в радианах)
angle1 = 0.0  # Угол поворота первого эллипса (насколько он повернут вокруг своего центра)
angle2 = 0.0  # Угол поворота второго эллипса (насколько он повернут вокруг своего центра)
move_angle2 = 0.0  # Угол, определяющий положение центра второго эллипса на границе первого эллипса
angle3 = 0.0  # Угол поворота третьего эллипса (насколько он повернут вокруг своего центра)
move_angle3 = 0.0  # Угол, определяющий положение центра третьего эллипса на границе второго эллипса


# Функция для отрисовки эллипса с учетом поворота
def draw_rotated_ellipse(surface, center, a, b, rotation, color, steps=60):
    """
    Рисует эллипс с полуосями a и b, повернутый на угол rotation (в радианах).
    Параметры:
      surface  - поверхность, на которой рисуем эллипс
      center   - координаты центра эллипса (x, y)
      a, b     - полуоси эллипса (a - горизонтальная полуось, b - вертикальная)
      rotation - угол поворота эллипса (в радианах)
      color    - цвет эллипса (RGB)
      steps    - количество точек для аппроксимации эллипса (чем больше, тем плавнее кривая)
    """
    points = []  # Создаем пустой список для хранения точек эллипса
    for i in range(steps):  # Проходим по количеству шагов (точек)
        theta = 2 * math.pi * i / steps  # Вычисляем угол для текущей точки
        # Вычисляем координаты точки на эллипсе в локальной системе координат (без учета поворота)
        x_local = a * math.cos(theta)
        y_local = b * math.sin(theta)
        # Применяем поворот к точке на угол rotation
        x_rot = x_local * math.cos(rotation) - y_local * math.sin(rotation)
        y_rot = x_local * math.sin(rotation) + y_local * math.cos(rotation)
        # Смещаем повернутую точку в мировую систему координат относительно центра эллипса
        points.append((center[0] + x_rot, center[1] + y_rot))
    # Рисуем многоугольник, аппроксимирующий эллипс, используя вычисленные точки
    pygame.draw.polygon(surface, color, points, 1)


# Класс Button для создания кнопок, позволяющих изменять параметры в окне
class Button:
    def __init__(self, x, y, width, height, label, param_name):
        # Создаем прямоугольник для кнопки с заданными координатами и размерами
        self.rect = pygame.Rect(x, y, width, height)
        self.label = label  # Текст, который будет отображаться на кнопке (например, "Rot1")
        self.param_name = param_name  # Имя параметра, который будет изменяться (ключ в словаре speeds)

    def draw(self, surface, value, font):
        # Рисуем прямоугольник кнопки с фоновым цветом (серый)
        pygame.draw.rect(surface, (200, 200, 200), self.rect)
        # Рисуем рамку вокруг кнопки (черная линия)
        pygame.draw.rect(surface, (0, 0, 0), self.rect, 2)
        # Формируем текст кнопки, который включает название и текущее значение параметра (с тремя знаками после запятой)
        text = font.render(f"{self.label}: {value:.3f}", True, (0, 0, 0))
        # Вычисляем положение текста так, чтобы он был по центру кнопки
        text_rect = text.get_rect(center=self.rect.center)
        # Рисуем текст на поверхности кнопки
        surface.blit(text, text_rect)


# Создаем объект шрифта для отображения текста на кнопках (размер 24)
font = pygame.font.SysFont(None, 24)

# Задаем размеры и отступы для кнопок, располагаемых в нижней части окна
button_width = 150  # Ширина каждой кнопки
button_height = 40  # Высота каждой кнопки
button_padding = 10  # Отступ между кнопками
start_x = 10  # Начальная координата X для первой кнопки
start_y = HEIGHT - button_height - 10  # Координата Y для кнопок (10 пикселей от нижнего края окна)

# Создаем список кнопок для изменения параметров
buttons = []
buttons.append(Button(start_x, start_y, button_width, button_height, "Rot1", "rot1"))
buttons.append(Button(start_x + (button_width + button_padding), start_y, button_width, button_height, "Rot2", "rot2"))
buttons.append(
    Button(start_x + 2 * (button_width + button_padding), start_y, button_width, button_height, "Move2", "move2"))
buttons.append(
    Button(start_x + 3 * (button_width + button_padding), start_y, button_width, button_height, "Rot3", "rot3"))
buttons.append(
    Button(start_x + 4 * (button_width + button_padding), start_y, button_width, button_height, "Move3", "move3"))

# Основной цикл программы, который продолжается, пока running == True
running = True
while running:
    # Обрабатываем все события (например, закрытие окна, нажатия мыши)
    for event in pygame.event.get():
        # Если событие - закрытие окна, выходим из цикла
        if event.type == pygame.QUIT:
            running = False
        # Если произошло событие нажатия кнопки мыши
        elif event.type == pygame.MOUSEBUTTONDOWN:
            # Получаем координаты позиции мыши при нажатии
            mouse_pos = event.pos
            # Проходим по каждой кнопке, чтобы проверить, была ли нажата
            for btn in buttons:
                # Если координаты мыши находятся внутри области кнопки
                if btn.rect.collidepoint(mouse_pos):
                    # Если нажата левая кнопка мыши (обычно event.button == 1)
                    if event.button == 1:
                        # Увеличиваем значение соответствующего параметра на 0.005
                        speeds[btn.param_name] += 0.005
                    # Если нажата правая кнопка мыши (обычно event.button == 3)
                    elif event.button == 3:
                        # Уменьшаем значение соответствующего параметра на 0.005
                        speeds[btn.param_name] -= 0.005

    # Обновляем углы для анимации эллипсов, добавляя значение скорости к текущему углу
    angle1 += speeds["rot1"]  # Обновляем угол поворота первого эллипса
    angle2 += speeds["rot2"]  # Обновляем угол поворота второго эллипса (сам эллипс вращается)
    move_angle2 += speeds["move2"]  # Обновляем угол движения центра второго эллипса по границе первого
    angle3 += speeds["rot3"]  # Обновляем угол поворота третьего эллипса
    move_angle3 += speeds["move3"]  # Обновляем угол движения центра третьего эллипса по границе второго

    # Вычисляем центры эллипсов
    # Центр первого эллипса фиксирован и находится в центре экрана
    center1 = (WIDTH // 2, HEIGHT // 2)

    # Вычисляем центр второго эллипса:
    # Если бы первый эллипс не вращался, точка на его границе имела бы координаты (a1 * cos(move_angle2), b1 * sin(move_angle2))
    local_x2 = a1 * math.cos(move_angle2)  # Локальная координата X на границе первого эллипса
    local_y2 = b1 * math.sin(move_angle2)  # Локальная координата Y на границе первого эллипса
    # Учитываем поворот первого эллипса (на угол angle1) и поворачиваем локальные координаты
    x2_offset = local_x2 * math.cos(angle1) - local_y2 * math.sin(angle1)
    y2_offset = local_x2 * math.sin(angle1) + local_y2 * math.cos(angle1)
    # Вычисляем мировой центр второго эллипса, добавляя смещение к центру первого эллипса
    center2 = (center1[0] + x2_offset, center1[1] + y2_offset)

    # Вычисляем центр третьего эллипса:
    # Если бы второй эллипс не вращался, точка на его границе имела бы координаты (a2 * cos(move_angle3), b2 * sin(move_angle3))
    local_x3 = a2 * math.cos(move_angle3)  # Локальная координата X на границе второго эллипса
    local_y3 = b2 * math.sin(move_angle3)  # Локальная координата Y на границе второго эллипса
    # Учитываем поворот второго эллипса (на угол angle2) и поворачиваем локальные координаты
    x3_offset = local_x3 * math.cos(angle2) - local_y3 * math.sin(angle2)
    y3_offset = local_x3 * math.sin(angle2) + local_y3 * math.cos(angle2)
    # Вычисляем мировой центр третьего эллипса, добавляя смещение к центру второго эллипса
    center3 = (center2[0] + x3_offset, center2[1] + y3_offset)

    # Очищаем экран, заливая его белым цветом
    screen.fill((255, 255, 255))

    # Рисуем эллипсы с учетом их текущих центров и углов поворота
    draw_rotated_ellipse(screen, center1, a1, b1, angle1, (0, 0, 0))  # Рисуем первый эллипс (черный)
    draw_rotated_ellipse(screen, center2, a2, b2, angle2, (0, 150, 0))  # Рисуем второй эллипс (зеленый)
    draw_rotated_ellipse(screen, center3, a3, b3, angle3, (150, 0, 0))  # Рисуем третий эллипс (красный)

    # Рисуем кнопки поверх эллипсов, чтобы они всегда были видны
    for btn in buttons:
        # Для каждой кнопки вызываем метод draw и передаем текущий параметр (значение скорости) для отображения
        btn.draw(screen, speeds[btn.param_name], font)

    # Обновляем содержимое экрана (перерисовываем)
    pygame.display.flip()
    # Ограничиваем частоту кадров до 60 кадров в секунду
    clock.tick(60)

# После выхода из основного цикла закрываем Pygame
pygame.quit()
