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

from collections import deque
from dataclasses import dataclass
import os


# Включает поддержку ANSI-последовательностей в некоторых терминалах Windows.
# В Linux эта строка ничего существенного не меняет, но и не мешает работе.
os.system("")


EPSILON_ALIASES = {"", "ε", "eps", "epsilon"}
DEFAULT_START_SYMBOL = "S"

Grammar = dict[str, list[tuple[str, ...]]]
Chart = list[list[set["EarleyState"]]]


class TerminalColor:
    """Короткие ANSI-коды для цветного вывода таблицы."""

    RESET = "\033[0m"
    BOLD = "\033[1m"
    RED = "\033[31m"
    GREEN = "\033[92m"
    CYAN = "\033[96m"
    YELLOW = "\033[93m"


@dataclass(frozen=True)
class EarleyState:
    """Одна ситуация алгоритма Эрли.

    Например, для правила S -> A B состояние с dot_position = 1 означает:
    S -> A · B. То есть A уже распознано, а B ещё нужно распознать.

    Класс сделан неизменяемым (frozen=True), чтобы объекты можно было
    безопасно класть в set. Это важно: таблица алгоритма хранит множества
    состояний и автоматически отбрасывает дубликаты.
    """

    left: str
    right: tuple[str, ...]
    dot_position: int

    def completed(self) -> bool:
        """Проверяет, дошла ли точка до конца правой части правила."""
        return self.dot_position == len(self.right)

    def expected_symbol(self) -> str | None:
        """Возвращает символ сразу после точки.

        Если состояние уже завершено, после точки ничего нет, поэтому
        возвращается None.
        """
        if self.completed():
            return None

        return self.right[self.dot_position]

    def shifted_dot(self) -> "EarleyState":
        """Создаёт новое состояние, где точка сдвинута на один символ вправо."""
        return EarleyState(
            left=self.left,
            right=self.right,
            dot_position=self.dot_position + 1,
        )

def default_grammar() -> tuple[Grammar, str]:
    """Возвращает грамматику, которая используется при запуске программы.

    В ней:
        T -> aT | b  описывает слова вида b, ab, aab, ...
        S -> ST | T  разрешает склеивать несколько таких блоков подряд.
    """
    grammar: Grammar = {
        "S": [
            ("S", "T"),
            ("T",),
        ],
        "T": [
            ("a", "T"),
            ("b",),
        ],
    }

    return grammar, DEFAULT_START_SYMBOL


def split_right_part(text: str) -> tuple[str, ...]:
    """Преобразует правую часть правила в кортеж символов.

    Поддерживаются два способа ввода:
    1. Без пробелов: aT превратится в ("a", "T").
    2. С пробелами: a T тоже превратится в ("a", "T").

    Пустая альтернатива и специальные обозначения ε/eps/epsilon считаются
    пустой цепочкой, то есть правилом вида A -> ε.
    """
    cleaned_text = text.strip()

    if cleaned_text in EPSILON_ALIASES:
        return tuple()

    if " " in cleaned_text:
        return tuple(cleaned_text.split())

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
    nonterminal = left_part.strip()

    if not nonterminal:
        raise ValueError("Пустая левая часть правила")

    alternatives = right_part.split("|")
    productions = [split_right_part(variant) for variant in alternatives]

    return nonterminal, productions


def input_custom_grammar() -> tuple[Grammar, str]:
    """Запрашивает у пользователя новую грамматику."""
    print("\nВвод новой грамматики")
    print("Формат:")
    print("  S -> ST | T")
    print("  T -> aT | b\n")
    print("Или через пробелы:")
    print("  S -> S T | T")
    print("  T -> a T | b\n")
    print("Для пустой строки используйте ε:")
    print("  A -> ε\n")
    print("Вводите правила по одному.")
    print("Пустая строка - закончить ввод.\n")

    grammar: Grammar = {}

    while True:
        raw_rule = input("Правило: ").strip()

        if raw_rule == "":
            break

        try:
            left_part, productions = read_rule(raw_rule)
        except ValueError as error:
            print(f"Ошибка: {error}")
            print("Попробуйте ещё раз.\n")
            continue

        # Один и тот же нетерминал можно описывать несколькими строками:
        # например, отдельно ввести S -> A, а потом S -> B.
        grammar.setdefault(left_part, []).extend(productions)

    if not grammar:
        print("Грамматика не была введена. Используется грамматика по умолчанию.")
        return default_grammar()

    start_symbol = input("Стартовый символ, по умолчанию S: ").strip()

    if not start_symbol:
        start_symbol = DEFAULT_START_SYMBOL
        print(f"Стартовый символ не указан. Используется: {DEFAULT_START_SYMBOL}")

    if start_symbol not in grammar:
        print(f"Для стартового символа {start_symbol} нет правил.")
        print("Используется грамматика по умолчанию.")
        return default_grammar()

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
        return []

    if " " in cleaned_line:
        return cleaned_line.split()

    return list(cleaned_line)


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

    В классическом описании алгоритма есть три основные операции:

    1. Predict / Предсказание.
       Если после точки стоит нетерминал B, в таблицу добавляются все правила
       B -> ... с точкой в начале.

    2. Scan / Сканирование.
       Если после точки стоит терминал и он совпал с текущим символом слова,
       точка сдвигается вправо, а состояние переносится в следующую позицию.

    3. Complete / Завершение.
       Если какое-то правило B -> ... полностью распознано, все состояния,
       которые ждали B после точки, тоже продвигаются на один символ.

    В этой реализации таблица двумерная: chart[i][j] хранит ситуации, которые
    относятся к подстроке word[i:j]. Очередь нужна, чтобы каждое новое состояние
    было обработано ровно тогда, когда оно появилось.
    """
    word_length = len(word)
    nonterminals = set(grammar)
    chart: Chart = [
        [set() for _ in range(word_length + 1)]
        for _ in range(word_length + 1)
    ]
    pending_states: deque[tuple[int, int, EarleyState]] = deque()

    def symbol_is_nonterminal(symbol: str | None) -> bool:
        return symbol in nonterminals

    def put_state(start: int, end: int, state: EarleyState) -> None:
        """Добавляет состояние в таблицу и очередь, если его там ещё не было."""
        if state in chart[start][end]:
            return

        chart[start][end].add(state)
        pending_states.append((start, end, state))

    # Начинаем с правил стартового символа. Точка стоит в начале, потому что
    # перед чтением слова не распознана ещё ни одна часть правила.
    for production in grammar[start_symbol]:
        put_state(0, 0, EarleyState(start_symbol, production, 0))

    while pending_states:
        fragment_start, fragment_end, state = pending_states.popleft()

        if state.completed():
            # Complete: найдено завершённое правило state.left, которое
            # покрывает фрагмент word[fragment_start:fragment_end].
            completed_nonterminal = state.left

            # Ищем все ситуации, которые закончились ровно там, где начался
            # завершённый нетерминал, и у которых после точки ожидался именно
            # этот нетерминал. Такие ситуации можно продвинуть.
            for previous_start in range(fragment_start + 1):
                waiting_states = list(chart[previous_start][fragment_start])

                for waiting_state in waiting_states:
                    if waiting_state.expected_symbol() == completed_nonterminal:
                        put_state(
                            previous_start,
                            fragment_end,
                            waiting_state.shifted_dot(),
                        )

            continue

        expected = state.expected_symbol()

        if symbol_is_nonterminal(expected):
            # Predict: после точки стоит нетерминал, значит нужно добавить все
            # его правила в ячейку chart[fragment_end][fragment_end]. Они
            # стартуют с текущей позиции, потому что именно здесь должен
            # начаться вывод этого нетерминала.
            for production in grammar[expected]:
                put_state(fragment_end, fragment_end, EarleyState(expected, production, 0))

            # Если нужный нетерминал уже был распознан ранее, сразу используем
            # этот факт и продвигаем текущее состояние. Это не отдельный новый
            # принцип, а ускоренная проверка уже заполненных ячеек таблицы.
            for known_end in range(fragment_end, word_length + 1):
                completed_states = list(chart[fragment_end][known_end])

                for completed_state in completed_states:
                    if completed_state.left == expected and completed_state.completed():
                        put_state(fragment_start, known_end, state.shifted_dot())
        else:
            # Scan: после точки терминал. Его можно "прочитать" только если
            # слово ещё не закончилось и текущий символ совпадает с ожидаемым.
            if fragment_end < word_length and word[fragment_end] == expected:
                put_state(fragment_start, fragment_end + 1, state.shifted_dot())

    # Слово принято, если в ячейке chart[0][n] есть завершённое стартовое
    # правило: оно покрывает весь ввод от начала до конца.
    for production in grammar[start_symbol]:
        final_state = EarleyState(start_symbol, production, len(production))

        if final_state in chart[0][word_length]:
            return True, chart

    return False, chart

def colorize_state(
    state: EarleyState,
    start: int,
    end: int,
    word_length: int,
    start_symbol: str,
) -> str:
    """Раскрашивает состояние в зависимости от его роли в таблице."""
    text = render_state(state)

    # Полное стартовое состояние, покрывающее всё слово, является главным
    # доказательством того, что слово принадлежит языку грамматики.
    if start == 0 and end == word_length and state.left == start_symbol and state.completed():
        return TerminalColor.BOLD + TerminalColor.GREEN + text + TerminalColor.RESET

    # Завершённые промежуточные состояния полезно выделить отдельно: они уже
    # могут продвигать другие ситуации на этапе Complete.
    if state.completed():
        return TerminalColor.CYAN + text + TerminalColor.RESET

    # Незавершённые состояния показывают, что алгоритм ещё ожидает какие-то
    # терминалы или нетерминалы.
    return TerminalColor.YELLOW + text + TerminalColor.RESET


def sort_states(states: set[EarleyState]) -> list[EarleyState]:
    """Сортирует состояния, чтобы таблица печаталась одинаково при каждом запуске."""
    return sorted(states, key=lambda state: (state.left, state.right, state.dot_position))


def build_printable_table(
    chart: Chart,
    start_symbol: str,
) -> tuple[list[list[list[str]]], list[list[list[str]]], list[int]]:
    """Подготавливает данные для красивой печати таблицы.

    Нужны две версии текста:
    1. plain_cells - без цветов, по ним считаем ширину колонок;
    2. color_cells - с ANSI-кодами, их реально выводим в терминал.

    Если считать ширину по цветной строке, таблица "поедет", потому что
    невидимые ANSI-коды увеличивают длину строки для Python, но не занимают
    места на экране.
    """
    word_length = len(chart) - 1
    plain_cells: list[list[list[str]]] = []
    color_cells: list[list[list[str]]] = []

    for row_index in range(word_length + 1):
        plain_row: list[list[str]] = []
        color_row: list[list[str]] = []

        for column_index in range(word_length + 1):
            states = sort_states(chart[row_index][column_index])

            if states:
                plain_lines = [render_state(state) for state in states]
                color_lines = [
                    colorize_state(
                        state,
                        row_index,
                        column_index,
                        word_length,
                        start_symbol,
                    )
                    for state in states
                ]
            else:
                plain_lines = [""]
                color_lines = [""]

            plain_row.append(plain_lines)
            color_row.append(color_lines)

        plain_cells.append(plain_row)
        color_cells.append(color_row)

    column_widths: list[int] = []

    for column_index in range(word_length + 1):
        width = len(f"j={column_index}")

        for row_index in range(word_length + 1):
            for line in plain_cells[row_index][column_index]:
                width = max(width, len(line))

        # Небольшой запас делает таблицу менее сжатой.
        column_widths.append(width + 2)

    return plain_cells, color_cells, column_widths


def print_table(chart: Chart, start_symbol: str) -> None:
    """Печатает таблицу chart[i][j] после работы алгоритма."""
    word_length = len(chart) - 1
    row_header_width = 6
    plain_cells, color_cells, column_widths = build_printable_table(chart, start_symbol)

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
            len(plain_cells[row_index][column_index])
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
                plain_lines = plain_cells[row_index][column_index]
                color_lines = color_cells[row_index][column_index]

                if line_index < len(plain_lines):
                    plain_text = plain_lines[line_index]
                    color_text = color_lines[line_index]
                else:
                    plain_text = ""
                    color_text = ""

                padding = " " * (column_widths[column_index] - len(plain_text))
                row += color_text + padding + "|"

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
    print_table(chart, start_symbol)
    print_result(word, accepted)


def print_menu() -> None:
    """Показывает доступные действия."""
    print("1. Проверить слово")
    print("2. Показать грамматику")
    print("3. Изменить грамматику")
    print("4. Вернуть грамматику по умолчанию")
    print("0. Выход\n")


def main() -> None:
    """Главный цикл консольной программы."""
    grammar, start_symbol = default_grammar()

    while True:
        print_menu()
        choice = input("Выберите пункт: \n").strip()

        if choice == "1":
            check_word(grammar, start_symbol)
        elif choice == "2":
            print_grammar(grammar, start_symbol)
        elif choice == "3":
            grammar, start_symbol = input_custom_grammar()
            print_grammar(grammar, start_symbol)
        elif choice == "4":
            grammar, start_symbol = default_grammar()
            print("Грамматика по умолчанию восстановлена.")
            print_grammar(grammar, start_symbol)
        elif choice == "0":
            print("Выход.")
            break
        else:
            print("Неизвестный пункт меню. Попробуйте ещё раз.\n")


if __name__ == "__main__":
    main()