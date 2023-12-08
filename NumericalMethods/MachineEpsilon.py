# Программа по нахождению Машинного эпсилона
def machine_epsilon():
    epsilon = 1.0
    while 1.0 + epsilon != 1.0:
        epsilon /= 2.0
    return epsilon

print(machine_epsilon())