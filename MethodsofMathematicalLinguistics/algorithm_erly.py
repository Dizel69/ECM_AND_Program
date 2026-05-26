"""Интерактивная проверка слов алгоритмом Эрли.

Алгоритм Эрли (Earley parser) нужен для распознавания слов, заданных
контекстно-свободной грамматикой. В отличие от простого перебора выводов,
он хранит не сами деревья вывода, а промежуточные "ситуации" с точкой:

    A -> alpha · beta

Точка показывает, какая часть правой части правила уже разобрана.
В этой программе ситуации раскладываются по таблице chart[start][end]:
каждая ячейка хранит состояния, которые выводят фрагмент слова от позиции
start до позиции end.
"""

# deque — очередь ситуаций, которые ещё нужно обработать.
from collections import deque
# dataclass — удобный способ описать одну ситуацию (правило + позиция точки).
from dataclasses import dataclass
import os


# Включает поддержку ANSI-последовательностей в некоторых терминалах Windows.
# В Linux эта строка ничего существенного не меняет, но и не мешает работе.
os.system("")


# Пустое слово можно вводить так: пустая строка, ε, eps, epsilon.
EPSILON_ALIASES = {"", "ε", "eps", "epsilon"}
# Стартовый символ по умолчанию, если в файле нет строки %start.
DEFAULT_START_SYMBOL = "S"
# Имя файла с грамматикой (лежит рядом с этим скриптом).
GRAMMAR_FILE_NAME = "grammar_erly.txt"
GRAMMAR_FILE_PATH = os.path.join(os.path.dirname(__file__), GRAMMAR_FILE_NAME)

# Grammar: словарь «левая часть -> список правых частей».
# Chart: таблица M[i][j], в каждой ячейке — множество ситуаций.
Grammar = dict[str, list[tuple[str, ...]]]
Chart = list[list[set["EarleyState"]]]


class TerminalColor:
    """Короткие ANSI-коды для цветного вывода результата проверки."""

    RESET = "\033[0m"
    BOLD = "\033[1m"
    RED = "\033[31m"
    GREEN = "\033[92m"


@dataclass(frozen=True)
class EarleyState:
    """Одна ситуация алгоритма Эрли.

    Например, для правила S -> A B состояние с dot_position = 1 означает:
    S -> A · B. То есть A уже распознано, а B ещё нужно распознать.

    Класс сделан неизменяемым (frozen=True), чтобы объекты можно было
    безопасно класть в set. Это важно: таблица алгоритма хранит множества
    состояний и автоматически отбрасывает дубликаты.
    """

    left: str  # левая часть правила, например "S"
    right: tuple[str, ...]  # правая часть, например ("A", "B")
    dot_position: int  # индекс, куда вставить точку (0 = в начале)

    def completed(self) -> bool:
        """Проверяет, дошла ли точка до конца правой части правила."""
        return self.dot_position == len(self.right)

    def expected_symbol(self) -> str | None:
        """Возвращает символ сразу после точки.

        Если состояние уже завершено, после точки ничего нет, поэтому
        возвращается None.
        """
        if self.completed():
            return None  # после точки ничего нет

        return self.right[self.dot_position]

    def shifted_dot(self) -> "EarleyState":
        """Создаёт новое состояние, где точка сдвинута на один символ вправо."""
        return EarleyState(
            left=self.left,
            right=self.right,
            dot_position=self.dot_position + 1,
        )


def split_right_part(text: str) -> tuple[str, ...]:
    """Преобразует правую часть правила в кортеж символов.

    Поддерживаются два способа ввода:
    1. Без пробелов: aT превратится в ("a", "T").
    2. С пробелами: a T тоже превратится в ("a", "T").

    Пустая альтернатива и специальные обозначения ε/eps/epsilon считаются
    пустой цепочкой, то есть правилом вида A -> ε.
    """
    cleaned_text = text.strip()

    # Пустая правая часть = правило A -> ε.
    if cleaned_text in EPSILON_ALIASES:
        return tuple()

    # Если в правой части есть пробелы: "a T" -> ("a", "T").
    if " " in cleaned_text:
        return tuple(cleaned_text.split())

    # Без пробелов: "aT" -> каждый символ отдельно: ("a", "T").
    return tuple(cleaned_text)


def read_rule(line: str) -> tuple[str, list[tuple[str, ...]]]:
    """Разбирает одну строку грамматики.

    Пример:
        S -> S T | T

    Левая часть вернётся как "S", а правая часть как список альтернатив:
        [("S", "T"), ("T",)]
    """
    if "->" not in line:
        raise ValueError("В правиле должен быть символ ->")

    left_part, right_part = line.split("->", 1)
    nonterminal = left_part.strip()  # левая часть, например S

    if not nonterminal:
        raise ValueError("Пустая левая часть правила")

    # Вертикальная черта | разделяет альтернативы одной левой части.
    alternatives = right_part.split("|")
    productions = [split_right_part(variant) for variant in alternatives]

    return nonterminal, productions


def load_grammar_from_file(file_path: str) -> tuple[Grammar, str]:
    """Считывает грамматику из файла перед запуском меню программы."""
    grammar: Grammar = {}
    start_symbol = DEFAULT_START_SYMBOL

    with open(file_path, encoding="utf-8") as grammar_file:
        for line_number, line in enumerate(grammar_file, start=1):
            raw_line = line.strip()

            # Пустые строки и комментарии (# ...) пропускаем.
            if not raw_line or raw_line.startswith("#"):
                continue

            parts = raw_line.split(maxsplit=1)

            # Строка %start S задаёт, с какого символа начинать разбор.
            if parts[0] == "%start":
                if len(parts) != 2 or not parts[1].strip():
                    raise ValueError(f"строка {line_number}: после %start нужен символ")

                start_symbol = parts[1].strip()
                continue

            try:
                left_part, productions = read_rule(raw_line)
            except ValueError as error:
                raise ValueError(f"строка {line_number}: {error}") from error

            # Один и тот же нетерминал можно описывать несколькими строками:
            # например, отдельно написать S -> A, а потом S -> B.
            grammar.setdefault(left_part, []).extend(productions)

    if not grammar:
        raise ValueError("в файле грамматики нет правил")

    if start_symbol not in grammar:
        raise ValueError(f"для стартового символа {start_symbol} нет правил")

    return grammar, start_symbol


def format_production(right_part: tuple[str, ...]) -> str:
    """Готовит правую часть правила к печати."""
    if not right_part:
        return "ε"

    return " ".join(right_part)


def print_grammar(grammar: Grammar, start_symbol: str) -> None:
    """Печатает текущую грамматику в привычном виде."""
    print("\nТекущая грамматика:")

    for left_part, productions in grammar.items():
        alternatives = [format_production(production) for production in productions]
        print(f"  {left_part} -> {' | '.join(alternatives)}")

    print(f"Стартовый символ: {start_symbol}\n")


def parse_word(line: str) -> list[str]:
    """Преобразует введённое слово в список терминалов.

    Если пользователь пишет символы без пробелов, каждый символ считается
    отдельным терминалом: "aab" -> ["a", "a", "b"].

    Если пользователь пишет через пробелы, пробелы используются как разделители:
    "id + id" -> ["id", "+", "id"].
    """
    cleaned_line = line.strip()

    if cleaned_line in EPSILON_ALIASES:
        return []  # пустое слово

    if " " in cleaned_line:
        return cleaned_line.split()  # "a + b" -> ["a", "+", "b"]

    return list(cleaned_line)  # "aab" -> ["a", "a", "b"]


def render_state(state: EarleyState) -> str:
    """Возвращает строковое представление ситуации с точкой."""
    symbols = list(state.right)
    symbols.insert(state.dot_position, "·")

    if symbols:
        right_part = " ".join(symbols)
    else:
        # У epsilon-правила правая часть пустая, поэтому сама точка показывает,
        # что правило уже завершено.
        right_part = "·"

    return f"{state.left} -> {right_part}"


def earley_recognize(
    grammar: Grammar,
    start_symbol: str,
    word: list[str],
) -> tuple[bool, Chart]:
    """Проверяет слово алгоритмом Эрли и возвращает итог вместе с таблицей.

    В конспекте шаги алгоритма обозначены буквами:

    A. Добавить S -> · γ в M[0][0] для всех правил S -> γ.
    B. Если A -> α · a β лежит в M[i][j] и следующий символ слова равен a,
       добавить A -> α a · β в M[i][j + 1].
    C. Если A -> α · B β лежит в M[i][j], а B -> γ · лежит в M[j][k],
       добавить A -> α B · β в M[i][k].
    D. Если A -> α · B β лежит в M[i][j], добавить B -> · γ в M[j][j]
       для всех правил B -> γ.

    В этой реализации таблица двумерная: chart[i][j] хранит ситуации, которые
    относятся к подстроке word[i:j]. Очередь нужна, чтобы каждое новое состояние
    было обработано ровно тогда, когда оно появилось.
    """
    word_length = len(word)
    # Все ключи grammar — это «большие буквы» (нетерминалы) из левых частей правил.
    nonterminals = set(grammar)
    # Пустая таблица (n+1) x (n+1), в каждой ячейке пока пустое множество.
    chart: Chart = [
        [set() for _ in range(word_length + 1)]
        for _ in range(word_length + 1)
    ]
    # Очередь: (i, j, ситуация) — что ещё нужно обработать.
    pending_states: deque[tuple[int, int, EarleyState]] = deque()

    def symbol_is_nonterminal(symbol: str | None) -> bool:
        """Проверяет, является ли символ нетерминалом грамматики."""
        return symbol in nonterminals

    def put_state(start: int, end: int, state: EarleyState) -> None:
        """Добавляет состояние в таблицу и очередь, если его там ещё не было."""
        if state in chart[start][end]:
            return  # дубликат не добавляем

        chart[start][end].add(state)
        pending_states.append((start, end, state))

    # =============================
    # Шаг A
    # =============================
    # Шаг A: добавляем в M[0][0] все правила стартового символа с точкой
    # перед правой частью.
    # В самом начале: все правила стартового символа S с точкой перед правой частью.
    # Пример: S -> · S T и S -> · T попадают в M[0][0].
    for production in grammar[start_symbol]:
        put_state(0, 0, EarleyState(start_symbol, production, 0))

    # Пока в очереди есть ситуации — применяем к ним шаги B, C, D.
    while pending_states:
        fragment_start, fragment_end, state = pending_states.popleft()
        # fragment_start = i, fragment_end = j для этой ситуации в M[i][j].

        # Если точка в конце правой части — правило B -> γ · полностью готово.
        if state.completed():
            # =============================
            # Шаг C
            # =============================
            # Шаг C: найдено завершённое правило B -> γ · в M[j][k],
            # где j = fragment_start, k = fragment_end.
            # Нашли готовый нетерминал B (state.left) на отрезке [j..k].
            completed_nonterminal = state.left

            # Ищем все A -> α · B β в M[i][j] и добавляем
            # A -> α B · β в M[i][k].
            # previous_start = i: откуда начинался фрагмент у «ждущих» ситуаций.
            for previous_start in range(fragment_start + 1):
                waiting_states = list(chart[previous_start][fragment_start])

                for waiting_state in waiting_states:
                    if waiting_state.expected_symbol() == completed_nonterminal:
                        put_state(
                            previous_start,
                            fragment_end,
                            waiting_state.shifted_dot(),
                        )

            continue  # к этой ситуации шаги B и D не применяем

        expected = state.expected_symbol()  # символ сразу после точки

        if symbol_is_nonterminal(expected):
            # =============================
            # Шаг D
            # =============================
            # Шаг D: если после точки стоит нетерминал B, добавляем все
            # правила B -> γ в M[j][j] с точкой перед γ.
            # После точки большая буква B — раскрываем: все правила B -> γ с точкой в начале.
            # Они кладутся в M[j][j], то есть «начинаем разбирать B с позиции j».
            for production in grammar[expected]:
                put_state(fragment_end, fragment_end, EarleyState(expected, production, 0))

            # =============================
            # Шаг C
            # =============================
            # Шаг C для уже заполненных ячеек: если B -> γ · уже есть
            # в M[j][k], сразу добавляем A -> α B · β в M[i][k].
            # Если B уже где-то полностью разобран (B -> γ · в M[j][k]),
            # сразу продвигаем текущую ситуацию A -> α · B β -> A -> α B · β в M[i][k].
            for known_end in range(fragment_end, word_length + 1):
                completed_states = list(chart[fragment_end][known_end])

                for completed_state in completed_states:
                    if completed_state.left == expected and completed_state.completed():
                        put_state(fragment_start, known_end, state.shifted_dot())
        else:
            # =============================
            # Шаг B
            # =============================
            # Шаг B: если после точки стоит терминал a и он совпадает со
            # следующим символом слова, переносим точку за этот терминал.
            # После точки буква из слова (терминал). Если она совпала с word[j] —
            # сдвигаем точку и переносим ситуацию в M[i][j+1].
            if fragment_end < word_length and word[fragment_end] == expected:
                put_state(fragment_start, fragment_end + 1, state.shifted_dot())

    # Слово принято, если в ячейке chart[0][n] есть завершённое стартовое
    # правило: оно покрывает весь ввод от начала до конца.
    # Проверка ответа: в M[0][n] должно быть завершённое правило S -> ... ·
    for production in grammar[start_symbol]:
        final_state = EarleyState(start_symbol, production, len(production))

        if final_state in chart[0][word_length]:
            return True, chart

    return False, chart


def sort_states(states: set[EarleyState]) -> list[EarleyState]:
    """Сортирует состояния, чтобы таблица печаталась одинаково при каждом запуске."""
    return sorted(states, key=lambda state: (state.left, state.right, state.dot_position))


def build_printable_table(chart: Chart) -> tuple[list[list[list[str]]], list[int]]:
    """Подготавливает данные для красивой печати таблицы.

    Все состояния печатаются одним обычным цветом терминала.
    """
    word_length = len(chart) - 1
    cells: list[list[list[str]]] = []

    for row_index in range(word_length + 1):
        row: list[list[str]] = []

        for column_index in range(word_length + 1):
            states = sort_states(chart[row_index][column_index])

            if states:
                lines = [render_state(state) for state in states]
            else:
                lines = [""]  # пустая ячейка

            row.append(lines)

        cells.append(row)

    column_widths: list[int] = []

    for column_index in range(word_length + 1):
        width = len(f"j={column_index}")

        for row_index in range(word_length + 1):
            for line in cells[row_index][column_index]:
                width = max(width, len(line))

        # Небольшой запас делает таблицу менее сжатой.
        column_widths.append(width + 2)

    return cells, column_widths


def print_table(chart: Chart) -> None:
    """Печатает таблицу chart[i][j] после работы алгоритма."""
    word_length = len(chart) - 1
    row_header_width = 6
    cells, column_widths = build_printable_table(chart)

    def border() -> str:
        line = "+"
        line += "-" * row_header_width
        line += "+"

        for width in column_widths:
            line += "-" * width
            line += "+"

        return line

    print()
    print("ТАБЛИЦА M[i][j]")
    print(border())

    header = "|"
    header += f"{'':^{row_header_width}}"
    header += "|"

    for column_index in range(word_length + 1):
        header += f"{f'j={column_index}':^{column_widths[column_index]}}"
        header += "|"

    print(header)
    print(border())

    for row_index in range(word_length + 1):
        row_height = max(
            len(cells[row_index][column_index])
            for column_index in range(word_length + 1)
        )

        for line_index in range(row_height):
            row = "|"

            if line_index == 0:
                row += f"{f'i={row_index}':^{row_header_width}}"
            else:
                row += " " * row_header_width

            row += "|"

            for column_index in range(word_length + 1):
                lines = cells[row_index][column_index]

                if line_index < len(lines):
                    text = lines[line_index]
                else:
                    text = ""

                padding = " " * (column_widths[column_index] - len(text))
                row += text + padding + "|"

            print(row)

        print(border())

    print()


def print_result(word: list[str], accepted: bool) -> None:
    """Печатает финальный ответ распознавателя."""
    word_as_text = "".join(word) if word else "ε"

    if accepted:
        print(
            TerminalColor.BOLD
            + TerminalColor.GREEN
            + f"{word_as_text} принимается"
            + TerminalColor.RESET
        )
    else:
        print(
            TerminalColor.BOLD
            + TerminalColor.RED
            + f"{word_as_text} не принимается"
            + TerminalColor.RESET
        )

    print()


def check_word(grammar: Grammar, start_symbol: str) -> None:
    """Запрашивает слово, запускает алгоритм и показывает таблицу."""
    raw_word = input("Введите слово: \n").strip()

    if not raw_word:
        print("Слово не введено.\n")
        return

    word = parse_word(raw_word)
    accepted, chart = earley_recognize(grammar, start_symbol, word)

    print(f"Слово: {''.join(word) if word else 'ε'}")
    print_table(chart)
    print_result(word, accepted)


def print_menu() -> None:
    """Показывает доступные действия."""
    print("1. Проверить слово")
    print("2. Показать грамматику")
    print("0. Выход\n")


def main() -> None:
    """Главный цикл консольной программы."""
    try:
        grammar, start_symbol = load_grammar_from_file(GRAMMAR_FILE_PATH)
    except (OSError, ValueError) as error:
        print(f"Не удалось загрузить грамматику из файла {GRAMMAR_FILE_NAME}.")
        print(f"Ошибка: {error}")
        print("Исправьте файл грамматики и запустите программу заново.")
        return

    print(f"Грамматика загружена из файла {GRAMMAR_FILE_NAME}.")
    print_grammar(grammar, start_symbol)

    while True:
        print_menu()
        choice = input("Выберите пункт: \n").strip()

        if choice == "1":
            check_word(grammar, start_symbol)
        elif choice == "2":
            print_grammar(grammar, start_symbol)
        elif choice == "0":
            print("Выход.")
            break
        else:
            print("Неизвестный пункт меню. Попробуйте ещё раз.\n")


if __name__ == "__main__":
    main()
