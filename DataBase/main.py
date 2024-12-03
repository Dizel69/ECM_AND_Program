import psycopg2
from psycopg2 import sql


# Подключение к базе данных
def connect_to_db():
    return psycopg2.connect(
        dbname="drug",
        user="user",
        password="011212",
        host="localhost"
    )


### CRUD для Производителей ###
def add_manufacturer(name, country):
    conn = connect_to_db()
    cursor = conn.cursor()
    cursor.execute("INSERT INTO Manufacturer (Name, Country) VALUES (%s, %s) RETURNING Manufacturer_ID;",
                   (name, country))
    manufacturer_id = cursor.fetchone()[0]
    conn.commit()
    cursor.close()
    conn.close()
    return manufacturer_id


def delete_manufacturer(manufacturer_id):
    conn = connect_to_db()
    cursor = conn.cursor()
    cursor.execute("DELETE FROM Manufacturer WHERE Manufacturer_ID = %s;", (manufacturer_id,))
    conn.commit()
    cursor.close()
    conn.close()
    #reset_sequence("manufacturer", "manufacturer_id")


def get_all_manufacturers():
    conn = connect_to_db()
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM Manufacturer;")
    manufacturers = cursor.fetchall()
    cursor.close()
    conn.close()
    return manufacturers

def update_manufacturer(manufacturer_id, new_name, new_country):
    """
    Обновляет данные о производителе по ID.
    """
    conn = connect_to_db()
    cursor = conn.cursor()
    try:
        cursor.execute(
            "UPDATE Manufacturer SET Name = %s, Country = %s WHERE Manufacturer_ID = %s;",
            (new_name, new_country, manufacturer_id)
        )
        conn.commit()
        print(f"Данные производителя с ID {manufacturer_id} обновлены.")
    except Exception as e:
        print(f"Ошибка при обновлении производителя: {e}")
    finally:
        cursor.close()
        conn.close()


### CRUD для Лекарств ###
def add_medicine(name, form, dosage, manufacturer_id):
    conn = connect_to_db()
    cursor = conn.cursor()
    cursor.execute(
        "INSERT INTO Medicine (Name, Form, Dosage, Manufacturer_ID) VALUES (%s, %s, %s, %s) RETURNING Medicine_ID;",
        (name, form, dosage, manufacturer_id))
    medicine_id = cursor.fetchone()[0]
    conn.commit()
    cursor.close()
    conn.close()
    return medicine_id


def delete_medicine(medicine_id):
    conn = connect_to_db()
    cursor = conn.cursor()
    cursor.execute("DELETE FROM Medicine WHERE Medicine_ID = %s;", (medicine_id,))
    conn.commit()
    cursor.close()
    conn.close()
    #reset_sequence("Medicine", "Medicine_ID")


def get_all_medicines():
    conn = connect_to_db()
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM Medicine;")
    medicines = cursor.fetchall()
    cursor.close()
    conn.close()
    return medicines

def update_medicine(medicine_id, new_name, new_form, new_dosage, new_manufacturer_id):
    """
    Обновляет данные о лекарстве по ID.
    """
    conn = connect_to_db()
    cursor = conn.cursor()
    try:
        cursor.execute(
            "UPDATE Medicine SET Name = %s, Form = %s, Dosage = %s, Manufacturer_ID = %s WHERE Medicine_ID = %s;",
            (new_name, new_form, new_dosage, new_manufacturer_id, medicine_id)
        )
        conn.commit()
        print(f"Данные лекарства с ID {medicine_id} обновлены.")
    except Exception as e:
        print(f"Ошибка при обновлении лекарства: {e}")
    finally:
        cursor.close()
        conn.close()


### CRUD для записей о применении ###
def add_application(medicine_id, disease, usage_method):
    conn = connect_to_db()
    cursor = conn.cursor()
    cursor.execute(
        "INSERT INTO Application (Medicine_ID, Disease, Usage_method) VALUES (%s, %s, %s) RETURNING Application_ID;",
        (medicine_id, disease, usage_method))
    application_id = cursor.fetchone()[0]
    conn.commit()
    cursor.close()
    conn.close()
    return application_id


def delete_application(application_id):
    conn = connect_to_db()
    cursor = conn.cursor()
    cursor.execute("DELETE FROM Application WHERE Application_ID = %s;", (application_id,))
    conn.commit()
    cursor.close()
    conn.close()
    #reset_sequence("Application", "Application_ID")


def get_all_applications():
    conn = connect_to_db()
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM Application;")
    applications = cursor.fetchall()
    cursor.close()
    conn.close()
    return applications

def update_application(application_id, new_medicine_id, new_disease, new_usage_method):
    """
    Обновляет данные о применении по ID.
    """
    conn = connect_to_db()
    cursor = conn.cursor()
    try:
        cursor.execute(
            "UPDATE Application SET Medicine_ID = %s, Disease = %s, Usage_method = %s WHERE Application_ID = %s;",
            (new_medicine_id, new_disease, new_usage_method, application_id)
        )
        conn.commit()
        print(f"Данные применения с ID {application_id} обновлены.")
    except Exception as e:
        print(f"Ошибка при обновлении применения: {e}")
    finally:
        cursor.close()
        conn.close()


### CRUD для Аптек ###
def add_pharmacy(name, address):
    conn = connect_to_db()
    cursor = conn.cursor()
    cursor.execute("INSERT INTO Pharmacy (Name, Address) VALUES (%s, %s) RETURNING Pharmacy_ID;", (name, address))
    pharmacy_id = cursor.fetchone()[0]
    conn.commit()
    cursor.close()
    conn.close()
    return pharmacy_id


def delete_pharmacy(pharmacy_id):
    conn = connect_to_db()
    cursor = conn.cursor()
    cursor.execute("DELETE FROM Pharmacy WHERE Pharmacy_ID = %s;", (pharmacy_id,))
    conn.commit()
    cursor.close()
    conn.close()
    #reset_sequence("Pharmacy", "Pharmacy_ID")


def get_all_pharmacies():
    conn = connect_to_db()
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM Pharmacy;")
    pharmacies = cursor.fetchall()
    cursor.close()
    conn.close()
    return pharmacies

def update_pharmacy(pharmacy_id, new_name, new_address):
    """
    Обновляет данные о аптеке по ID.
    """
    conn = connect_to_db()
    cursor = conn.cursor()
    try:
        cursor.execute(
            "UPDATE Pharmacy SET Name = %s, Address = %s WHERE Pharmacy_ID = %s;",
            (new_name, new_address, pharmacy_id)
        )
        conn.commit()
        print(f"Данные аптеки с ID {pharmacy_id} обновлены.")
    except Exception as e:
        print(f"Ошибка при обновлении аптеки: {e}")
    finally:
        cursor.close()
        conn.close()


### CRUD для продаж ###
def add_sale(medicine_id, pharmacy_id, sale_date, price):
    conn = connect_to_db()
    cursor = conn.cursor()
    cursor.execute(
        "INSERT INTO Sale (Medicine_ID, Pharmacy_ID, Sale_date, Price) VALUES (%s, %s, %s, %s) RETURNING Sale_ID;",
        (medicine_id, pharmacy_id, sale_date, price))
    sale_id = cursor.fetchone()[0]
    conn.commit()
    cursor.close()
    conn.close()
    return sale_id


def delete_sale(sale_id):
    conn = connect_to_db()
    cursor = conn.cursor()
    cursor.execute("DELETE FROM Sale WHERE Sale_ID = %s;", (sale_id,))
    conn.commit()
    cursor.close()
    conn.close()
    #reset_sequence("Sale", "Sale_ID")


def get_all_sales():
    conn = connect_to_db()
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM Sale;")
    sales = cursor.fetchall()
    cursor.close()
    conn.close()
    return sales

def update_sale(sale_id, new_medicine_id, new_pharmacy_id, new_sale_date, new_price):
    """
    Обновляет данные о продаже по ID.
    """
    conn = connect_to_db()
    cursor = conn.cursor()
    try:
        cursor.execute(
            "UPDATE Sale SET Medicine_ID = %s, Pharmacy_ID = %s, Sale_date = %s, Price = %s WHERE Sale_ID = %s;",
            (new_medicine_id, new_pharmacy_id, new_sale_date, new_price, sale_id)
        )
        conn.commit()
        print(f"Данные продажи с ID {sale_id} обновлены.")
    except Exception as e:
        print(f"Ошибка при обновлении продажи: {e}")
    finally:
        cursor.close()
        conn.close()


def reset_sequence(table_name, id_column):
    """
    Сбрасывает значение автоинкрементного ID в таблице.
    Устанавливает следующий ID в зависимости от текущих данных.
    """
    conn = connect_to_db()
    cursor = conn.cursor()

    try:
        # Проверяем, существует ли таблица
        cursor.execute("""
            SELECT EXISTS (
                SELECT 1
                FROM information_schema.tables
                WHERE table_schema = 'public' AND table_name = %s
            );
        """, (table_name.lower(),))
        table_exists = cursor.fetchone()[0]

        if not table_exists:
            raise Exception(f"Таблица {table_name} не существует в схеме public.")

        # Выполняем сброс последовательности
        query = sql.SQL("""
            SELECT setval(
                pg_get_serial_sequence(%s, %s),
                COALESCE((SELECT MAX({id}) FROM {table}), 0) + 1,
                false
            )
        """).format(
            table=sql.Identifier(table_name),
            id=sql.Identifier(id_column)
        )
        cursor.execute(query, (table_name, id_column))
        conn.commit()
        print(f"Счётчик автоинкремента для таблицы {table_name} сброшен.")
    except Exception as e:
        print(f"Ошибка при сбросе автоинкремента: {e}")
    finally:
        cursor.close()
        conn.close()



### Функция удаления всех таблиц ###
def clear_all_tables():
    """
    Очищает данные во всех таблицах схемы public, не удаляя сами таблицы.
    Сбрасывает автоинкрементные счётчики.
    """
    conn = connect_to_db()
    cursor = conn.cursor()

    try:
        # Получение списка всех таблиц в схеме public
        cursor.execute("""
            SELECT table_name 
            FROM information_schema.tables 
            WHERE table_schema = 'public';
        """)
        tables = cursor.fetchall()

        if not tables:
            print("Нет таблиц для очистки.")
            return

        # Очищаем данные и сбрасываем счётчики
        for table in tables:
            table_name = table[0]
            print(f"Очищается таблица: {table_name}...")
            cursor.execute(sql.SQL("TRUNCATE TABLE {table} RESTART IDENTITY CASCADE;").format(
                table=sql.Identifier(table_name)
            ))

        conn.commit()
        print("Все таблицы успешно очищены.")
    except Exception as e:
        print(f"Ошибка при очистке таблиц: {e}")
        conn.rollback()
    finally:
        cursor.close()
        conn.close()



### Функция вывода всех таблиц ###
def show_all_data():
    """Вывод всех таблиц и их содержимого из базы данных."""
    conn = connect_to_db()
    cursor = conn.cursor()

    # Получаем список всех таблиц из схемы public
    cursor.execute("""
        SELECT table_name 
        FROM information_schema.tables 
        WHERE table_schema = 'public';
    """)
    tables = cursor.fetchall()

    # Для каждой таблицы выводим её содержимое
    print("\n=== Содержимое всей базы данных ===")
    for table in tables:
        table_name = table[0]
        print(f"\nТаблица: {table_name}")
        cursor.execute(sql.SQL("SELECT * FROM {table}").format(table=sql.Identifier(table_name)))
        rows = cursor.fetchall()
        if rows:
            for row in rows:
                print(row)
        else:
            print("Нет данных.")

    cursor.close()
    conn.close()

### Меню для работы с производителем ###
def manufacturer_menu():
    while True:
        print("\nМеню производителей:")
        print("1. Добавить нового производителя")
        print("2. Удалить производителя")
        print("3. Обновить данные производителя")
        print("4. Вывести всех производителей")
        print("5. Вернуться назад")
        choice = input("Выберите действие: ")

        if choice == "1":
            name = input("Введите название производителя: ")
            country = input("Введите страну производителя: ")
            manufacturer_id = add_manufacturer(name, country)
            print(f"Производитель добавлен с ID: {manufacturer_id}")
        elif choice == "2":
            manufacturer_id = int(input("Введите ID производителя для удаления: "))
            delete_manufacturer(manufacturer_id)
        elif choice == "3":
            manufacturer_id = int(input("Введите ID производителя для обновления: "))
            new_name = input("Введите новое название производителя: ")
            new_country = input("Введите новую страну производителя: ")
            update_manufacturer(manufacturer_id, new_name, new_country)
        elif choice == "4":
            manufacturers = get_all_manufacturers()
            print("Производители:")
            for row in manufacturers:
                print(row)
        elif choice == "5":
            break
        else:
            print("Некорректный выбор. Попробуйте снова.")

### Меню для работы с Лекарствами ###
def medicine_menu():
    while True:
        print("\nМеню лекарств:")
        print("1. Добавить новое лекарство")
        print("2. Удалить лекарство")
        print("3. Обновить данные лекарства")
        print("4. Вывести все лекарства")
        print("5. Вернуться назад")
        choice = input("Выберите действие: ")

        if choice == "1":
            name = input("Введите название лекарства: ")
            form = input("Введите форму лекарства (например, таблетка): ")
            dosage = input("Введите дозировку: ")
            manufacturer_id = int(input("Введите ID производителя: "))
            medicine_id = add_medicine(name, form, dosage, manufacturer_id)
            print(f"Лекарство добавлено с ID: {medicine_id}")
        elif choice == "2":
            medicine_id = int(input("Введите ID лекарства для удаления: "))
            delete_medicine(medicine_id)
        elif choice == "3":
            medicine_id = int(input("Введите ID лекарства для обновления: "))
            new_name = input("Введите новое название лекарства: ")
            new_form = input("Введите новую форму лекарства: ")
            new_dosage = input("Введите новую дозировку: ")
            new_manufacturer_id = int(input("Введите новый ID производителя: "))
            update_medicine(medicine_id, new_name, new_form, new_dosage, new_manufacturer_id)
        elif choice == "4":
            medicines = get_all_medicines()
            print("Лекарства:")
            for row in medicines:
                print(row)
        elif choice == "5":
            break
        else:
            print("Некорректный выбор. Попробуйте снова.")

### Меню для работы с записями о применении ###
def application_menu():
    while True:
        print("\nМеню записей о применении:")
        print("1. Добавить запись о применении")
        print("2. Удалить запись о применении")
        print("3. Обновить данные записи о применении")
        print("4. Вывести все записи о применении")
        print("5. Вернуться назад")
        choice = input("Выберите действие: ")

        if choice == "1":
            medicine_id = int(input("Введите ID лекарства: "))
            disease = input("Введите заболевание: ")
            usage_method = input("Введите способ применения: ")
            application_id = add_application(medicine_id, disease, usage_method)
            print(f"Запись о применении добавлена с ID: {application_id}")
        elif choice == "2":
            application_id = int(input("Введите ID записи о применении для удаления: "))
            delete_application(application_id)
        elif choice == "3":
            application_id = int(input("Введите ID записи о применении для обновления: "))
            new_medicine_id = int(input("Введите новый ID лекарства: "))
            new_disease = input("Введите новое заболевание: ")
            new_usage_method = input("Введите новый способ применения: ")
            update_application(application_id, new_medicine_id, new_disease, new_usage_method)
        elif choice == "4":
            applications = get_all_applications()
            print("Записи о применении:")
            for row in applications:
                print(row)
        elif choice == "5":
            break
        else:
            print("Некорректный выбор. Попробуйте снова.")

### Меню для работы с Аптеками ###
def pharmacy_menu():
    while True:
        print("\nМеню аптек:")
        print("1. Добавить новую аптеку")
        print("2. Удалить аптеку")
        print("3. Обновить данные аптеки")
        print("4. Вывести все аптеки")
        print("5. Вернуться назад")
        choice = input("Выберите действие: ")

        if choice == "1":
            name = input("Введите название аптеки: ")
            address = input("Введите адрес аптеки: ")
            pharmacy_id = add_pharmacy(name, address)
            print(f"Аптека добавлена с ID: {pharmacy_id}")
        elif choice == "2":
            pharmacy_id = int(input("Введите ID аптеки для удаления: "))
            delete_pharmacy(pharmacy_id)
        elif choice == "3":
            pharmacy_id = int(input("Введите ID аптеки для обновления: "))
            new_name = input("Введите новое название аптеки: ")
            new_address = input("Введите новый адрес аптеки: ")
            update_pharmacy(pharmacy_id, new_name, new_address)
        elif choice == "4":
            pharmacies = get_all_pharmacies()
            print("Аптеки:")
            for row in pharmacies:
                print(row)
        elif choice == "5":
            break
        else:
            print("Некорректный выбор. Попробуйте снова.")


### Меню для работы с Продажами ###
def sale_menu():
    while True:
        print("\nМеню продаж:")
        print("1. Добавить запись о продаже")
        print("2. Удалить запись о продаже")
        print("3. Обновить данные о продаже")
        print("4. Вывести все продажи")
        print("5. Вернуться назад")
        choice = input("Выберите действие: ")

        if choice == "1":
            medicine_id = int(input("Введите ID лекарства: "))
            pharmacy_id = int(input("Введите ID аптеки: "))
            sale_date = input("Введите дату продажи (YYYY-MM-DD): ")
            price = float(input("Введите цену: "))
            sale_id = add_sale(medicine_id, pharmacy_id, sale_date, price)
            print(f"Продажа добавлена с ID: {sale_id}")
        elif choice == "2":
            sale_id = int(input("Введите ID продажи для удаления: "))
            delete_sale(sale_id)
        elif choice == "3":
            sale_id = int(input("Введите ID продажи для обновления: "))
            new_medicine_id = int(input("Введите новый ID лекарства: "))
            new_pharmacy_id = int(input("Введите новый ID аптеки: "))
            new_sale_date = input("Введите новую дату продажи (YYYY-MM-DD): ")
            new_price = float(input("Введите новую цену: "))
            update_sale(sale_id, new_medicine_id, new_pharmacy_id, new_sale_date, new_price)
        elif choice == "4":
            sales = get_all_sales()
            print("Продажи:")
            for row in sales:
                print(row)
        elif choice == "5":
            break
        else:
            print("Некорректный выбор. Попробуйте снова.")


def main_menu():
    while True:
        print("\nГлавное меню:")
        print("1. Редактировать Производителя")
        print("2. Редактировать Лекарство")
        print("3. Редактировать Запись о применении")
        print("4. Редактировать Аптеку")
        print("5. Редактировать Данные о продажах")
        print("6. Вывести всю базу данных")
        print("7. Очистить все таблицы")
        print("8. Выход")
        choice = input("Выберите действие: ")

        if choice == "1":
            manufacturer_menu()
        elif choice == "2":
            medicine_menu()
        elif choice == "3":
            application_menu()
        elif choice == "4":
            pharmacy_menu()
        elif choice == "5":
            sale_menu()
        elif choice == "6":
            show_all_data()
        elif choice == "7":
            confirm = input("Вы уверены, что хотите очистить все таблицы? (yes/no): ")
            if confirm.lower() == "yes":
                clear_all_tables()
        elif choice == "8":
            print("Выход из программы.")
            break
        else:
            print("Неверный выбор. Попробуйте снова.")





if __name__ == "__main__":
    main_menu()
