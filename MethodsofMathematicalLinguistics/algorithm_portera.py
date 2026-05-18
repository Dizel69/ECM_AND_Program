"""Стемминг английских слов по алгоритму Портера.

Стемминг - это грубое выделение основы слова. Например, слова connected,
connecting и connection после обработки должны стать ближе к общей форме.
Алгоритм Портера делает это не через словарь, а через набор формальных правил:
он смотрит на окончание слова, проверяет несколько условий и заменяет или
удаляет суффикс.

Важно: результат стемминга не всегда является настоящим английским словом.
Например, relational превращается в relat. Это нормально: задача алгоритма -
не получить литературную начальную форму, а привести похожие формы к одному
и тому же техническому представлению.
"""

from __future__ import annotations

from typing import Optional, Tuple


class PorterStemmer:
    """
    Стеммер для английских слов по алгоритму Портера.

    Внутри алгоритма слово проходит несколько шагов. Каждый шаг отвечает за
    свою группу окончаний:
    - множественное число и окончания -ed/-ing;
    - длинные словообразовательные суффиксы вроде -ational, -fulness;
    - дополнительные суффиксы вроде -icate, -ness;
    - финальную очистку -e и двойной -ll.

    Правила применяются осторожно: перед заменой часто проверяется "мера" m,
    наличие гласной или вид последних букв. Благодаря этому алгоритм не удаляет
    слишком много символов у коротких слов.
    """

    VOWELS = set("aeiou")

    def stem(self, word: str) -> str:
        """
        Возвращает основу слова.

        Это главный метод класса. Пользователь передаёт одно слово, а метод
        по очереди прогоняет его через все шаги алгоритма Портера.
        """
        if not word:
            return word

        # Алгоритм Портера обычно описывается для нижнего регистра. Поэтому
        # сначала приводим слово к lower-case, чтобы Porter и porter
        # обрабатывались одинаково.
        word = word.lower()

        # Эта реализация рассчитана только на английские слова из букв a-z.
        # Если встретились цифры, дефисы, кириллица или другие символы, слово
        # возвращается как есть: так программа не будет применять английские
        # правила к неподходящему вводу.
        if any(not ("a" <= ch <= "z") for ch in word):
            return word

        # Очень короткие слова не обрабатываются. Для них срезание суффиксов
        # почти всегда даёт плохой результат.
        if len(word) <= 2:
            return word

        # Каждый шаг получает результат предыдущего шага. Например, сначала
        # может быть удалено -ing, затем на новом слове проверятся следующие
        # группы суффиксов.
        for step in (
            self._step1a,
            self._step1b,
            self._step1c,
            self._step2,
            self._step3,
            self._step4,
            self._step5a,
            self._step5b,
        ):
            word = step(word)

        return word

    # ---------------------------
    # Базовые проверки
    # ---------------------------

    def _is_consonant(self, word: str, index: int) -> bool:
        """
        Проверяет, является ли буква в позиции index согласной.

        Особый случай: y
        - в начале слова y считается согласной;
        - после согласной y считается гласной;
        - после гласной y считается согласной.
        """
        ch = word[index]

        # Обычные гласные в алгоритме - a, e, i, o, u.
        if ch in self.VOWELS:
            return False

        if ch == "y":
            # Буква y зависит от контекста. В начале слова она считается
            # согласной, а дальше меняет роль относительно предыдущей буквы.
            # Например, в toy буква y согласная, а в rhythm часть y может
            # выполнять роль гласной.
            if index == 0:
                return True
            return not self._is_consonant(word, index - 1)

        return True

    def _measure(self, word: str) -> int:
        """
        Вычисляет меру слова m:
        число чередований VC в представлении [C](VC)^m[V].

        Если упростить:
        - C означает группу согласных;
        - V означает группу гласных;
        - m показывает, сколько раз после группы гласных встретилась группа
          согласных.

        Примеры из описания алгоритма:
        - tr, ee, tree имеют m = 0;
        - trouble, oats, trees имеют m = 1;
        - troubles, private имеют m = 2.
        """
        length = len(word)
        i = 0
        measure = 0

        # Сначала пропускаем начальную группу согласных. В формуле [C] она
        # необязательная и не увеличивает m.
        while i < length and self._is_consonant(word, i):
            i += 1

        while i < length:
            # Затем пропускаем группу гласных V.
            while i < length and not self._is_consonant(word, i):
                i += 1

            if i >= length:
                break

            # После гласных должна идти группа согласных C. Именно переход
            # V -> C даёт один счётчик меры.
            while i < length and self._is_consonant(word, i):
                i += 1

            measure += 1

        return measure

    def _has_vowel(self, word: str) -> bool:
        """
        Проверка условия *v*: есть ли хотя бы одна гласная.

        Это условие защищает от неправильных срезаний. Например, окончание -ed
        можно удалить только тогда, когда перед ним действительно была гласная:
        plastered -> plaster, но слово без гласной в основе трогать нельзя.
        """
        return any(not self._is_consonant(word, i) for i in range(len(word)))

    def _double_consonant(self, word: str) -> bool:
        """
        Проверка условия *d: слово оканчивается на двойную согласную.

        Примеры: hopping после удаления -ing даёт hopp. Здесь pp - двойная
        согласная, поэтому одна p удаляется и получается hop.
        """
        if len(word) < 2:
            return False

        # Сначала две последние буквы должны быть одинаковыми.
        if word[-1] != word[-2]:
            return False

        # Затем проверяем, что эта буква именно согласная, а не гласная.
        return self._is_consonant(word, len(word) - 1)

    def _cvc(self, word: str) -> bool:
        """
        Проверка условия *o:
        слово оканчивается на CVC, а последняя согласная не w/x/y.

        CVC означает "согласная-гласная-согласная" на конце слова. Это условие
        используется, чтобы вернуть букву e в некоторых случаях:
        motoring -> motor, но после удаления -ing из hoping получилась бы hop,
        и алгоритм может добавить e, если структура слова этого требует.
        """
        if len(word) < 3:
            return False

        last = len(word) - 1

        # Проверяем три последние позиции справа налево:
        # последняя буква C, предпоследняя V, третья с конца C.
        if not self._is_consonant(word, last):
            return False
        if self._is_consonant(word, last - 1):
            return False
        if not self._is_consonant(word, last - 2):
            return False

        return word[last] not in ("w", "x", "y")

    def _replace_if(
        self,
        word: str,
        suffix: str,
        replacement: str,
        *,
        min_m: Optional[int] = None,
        eq_m: Optional[int] = None,
        require_vowel: bool = False,
    ) -> Tuple[str, bool]:
        """
        Универсальная замена суффикса.

        Если word заканчивается на suffix, берётся stem = word без suffix,
        затем проверяются условия:
        - require_vowel: в stem должна быть хотя бы одна гласная;
        - min_m=k: мера stem должна быть больше k;
        - eq_m=k: мера stem должна быть равна k.

        Возвращает:
        - новую строку;
        - True, если замена выполнена, иначе False.
        """
        # Если нужного суффикса нет, правило не подходит.
        if not word.endswith(suffix):
            return word, False

        # stem - это часть слова до найденного суффикса. Почти все условия
        # алгоритма проверяются именно на stem, а не на полном исходном слове.
        stem = word[:-len(suffix)]

        if require_vowel and not self._has_vowel(stem):
            return word, False

        m = self._measure(stem)

        # min_m=k соответствует условию m > k в оригинальном алгоритме.
        if min_m is not None and not (m > min_m):
            return word, False

        # eq_m=k соответствует условию m == k.
        if eq_m is not None and not (m == eq_m):
            return word, False

        return stem + replacement, True

    # ---------------------------
    # Шаги алгоритма Портера
    # ---------------------------

    def _step1a(self, word: str) -> str:
        """Обрабатывает множественное число и окончания на s."""
        # sses -> ss: caresses -> caress
        if word.endswith("sses"):
            return word[:-2]

        # ies -> i: ponies -> poni, ties -> ti
        if word.endswith("ies"):
            return word[:-2]

        # ss оставляется без изменений: caress не должно стать cares.
        if word.endswith("ss"):
            return word

        # Одиночная s удаляется: cats -> cat.
        if word.endswith("s"):
            return word[:-1]

        return word

    def _step1b(self, word: str) -> str:
        """Обрабатывает окончания прошедшего времени и герундия: eed, ed, ing."""
        # eed -> ee применяется только при m > 0:
        # agreed -> agree, но feed остаётся feed.
        if word.endswith("eed"):
            stem = word[:-3]
            if self._measure(stem) > 0:
                return stem + "ee"
            return word

        # ed удаляется, если перед ним есть гласная:
        # plastered -> plaster.
        if word.endswith("ed"):
            stem = word[:-2]
            if self._has_vowel(stem):
                return self._step1b_after_drop(stem)
            return word

        # ing удаляется по тому же принципу:
        # motoring -> motor, hopping -> hopp, после чего будет постобработка.
        if word.endswith("ing"):
            stem = word[:-3]
            if self._has_vowel(stem):
                return self._step1b_after_drop(stem)
            return word

        return word

    def _step1b_after_drop(self, word: str) -> str:
        """
        Постобработка после удаления -ed или -ing.

        После срезания -ed/-ing иногда нужно не просто вернуть получившуюся
        основу, а поправить последние буквы. Именно это описано в подпунктах
        шага 1b алгоритма Портера.
        """
        # at -> ate, bl -> ble, iz -> ize:
        # например, conflated после удаления -ed даёт conflat,
        # затем conflat превращается в conflate.
        for suffix, replacement in (("at", "ate"), ("bl", "ble"), ("iz", "ize")):
            if word.endswith(suffix):
                return word[:-len(suffix)] + replacement

        # Если слово закончилось двойной согласной, удаляем одну букву,
        # кроме случаев l, s, z. Поэтому hopping -> hop, но falling -> fall.
        if self._double_consonant(word) and word[-1] not in ("l", "s", "z"):
            return word[:-1]

        # Если слово короткое по мере m и заканчивается на CVC, добавляем e.
        # Это помогает не сделать основу слишком короткой.
        if self._measure(word) == 1 and self._cvc(word):
            return word + "e"

        return word

    def _step1c(self, word: str) -> str:
        """Заменяет конечную y на i, если в основе есть гласная."""
        if word.endswith("y"):
            stem = word[:-1]
            if self._has_vowel(stem):
                return stem + "i"
        return word

    def _step2(self, word: str) -> str:
        """Заменяет длинные суффиксы, если перед ними достаточно длинная основа."""
        replacements = [
            ("ational", "ate"),
            ("tional", "tion"),
            ("enci", "ence"),
            ("anci", "ance"),
            ("izer", "ize"),
            ("abli", "able"),
            ("alli", "al"),
            ("entli", "ent"),
            ("eli", "e"),
            ("ousli", "ous"),
            ("ization", "ize"),
            ("ation", "ate"),
            ("ator", "ate"),
            ("alism", "al"),
            ("iveness", "ive"),
            ("fulness", "ful"),
            ("ousness", "ous"),
            ("aliti", "al"),
            ("iviti", "ive"),
            ("biliti", "ble"),
            ("logi", "log"),
        ]

        # Правила проверяются от самых длинных суффиксов к коротким. Это важно:
        # например, -ization должен сработать раньше, чем более короткий -ation.
        for suffix, replacement in sorted(
            replacements,
            key=lambda item: len(item[0]),
            reverse=True,
        ):
            if word.endswith(suffix):
                new_word, _ = self._replace_if(word, suffix, replacement, min_m=0)
                return new_word

        return word

    def _step3(self, word: str) -> str:
        """Убирает или упрощает ещё одну группу словообразовательных суффиксов."""
        replacements = [
            ("icate", "ic"),
            ("ative", ""),
            ("alize", "al"),
            ("iciti", "ic"),
            ("ical", "ic"),
            ("ful", ""),
            ("ness", ""),
        ]

        for suffix, replacement in sorted(
            replacements,
            key=lambda item: len(item[0]),
            reverse=True,
        ):
            if word.endswith(suffix):
                new_word, _ = self._replace_if(word, suffix, replacement, min_m=0)
                return new_word

        return word

    def _step4(self, word: str) -> str:
        """
        Удаляет часть суффиксов на основе меры m.
        Отдельно обработан случай -ion: он удаляется только после s или t.

        На этом шаге алгоритм работает строже, чем на предыдущих: суффикс
        удаляется только при m > 1. Это значит, что перед окончанием должна
        быть достаточно "длинная" основа.
        """
        suffixes = [
            "al", "ance", "ence", "er", "ic", "able", "ible", "ant",
            "ement", "ment", "ent", "ou", "ism", "ate", "iti", "ous", "ive", "ize",
        ]

        # Суффикс ion имеет дополнительное ограничение: перед ним должна стоять
        # s или t. Поэтому adoption -> adopt, но не каждое слово на -ion будет
        # обработано одинаково.
        if word.endswith("ion") and len(word) > 3:
            stem = word[:-3]
            if stem and stem[-1] in ("s", "t") and self._measure(stem) > 1:
                return stem
            return word

        # Остальные суффиксы просто пробуются по убыванию длины.
        for suffix in sorted(suffixes, key=len, reverse=True):
            if word.endswith(suffix):
                new_word, _ = self._replace_if(word, suffix, "", min_m=1)
                return new_word

        return word

    def _step5a(self, word: str) -> str:
        """Удаляет финальную букву e при выполнении условий по мере слова."""
        if word.endswith("e"):
            stem = word[:-1]
            m = self._measure(stem)

            # Если m > 1, e удаляется без дополнительных условий:
            # probate -> probat.
            if m > 1:
                return stem

            # Если m == 1, e удаляется только тогда, когда основа не
            # заканчивается на CVC. Поэтому rate остаётся rate.
            if m == 1 and not self._cvc(stem):
                return stem

        return word

    def _step5b(self, word: str) -> str:
        """Убирает одну l в конце, если слово заканчивается на ll и m > 1."""
        if self._measure(word) > 1 and word.endswith("ll"):
            return word[:-1]
        return word


# Набор примеров нужен для быстрой проверки реализации. Слева исходное слово,
# справа ожидаемый результат после применения всех шагов алгоритма.
TEST_CASES = {
    "caresses": "caress",
    "ponies": "poni",
    "ties": "ti",
    "cats": "cat",
    "feed": "feed",
    "agreed": "agre",
    "plastered": "plaster",
    "motoring": "motor",
    "sing": "sing",
    "hopping": "hop",
    "falling": "fall",
    "hissing": "hiss",
    "fizzed": "fizz",
    "happy": "happi",
    "sky": "sky",
    "relational": "relat",
    "conditional": "condit",
    "rational": "ration",
    "vietnamization": "vietnam",
    "operator": "oper",
    "feudalism": "feudal",
    "decisiveness": "decis",
    "hopefulness": "hope",
    "triplicate": "triplic",
    "formative": "form",
    "formalize": "formal",
    "electriciti": "electr",
    "goodness": "good",
    "revival": "reviv",
    "allowance": "allow",
    "inference": "infer",
    "adoption": "adopt",
    "effective": "effect",
    "probate": "probat",
    "rate": "rate",
    "cease": "ceas",
    "controll": "control",
    "roll": "roll",
    "feed": "feed",
    "agreed": "agre",
}


def run_tests() -> None:
    """Запускает встроенные тестовые примеры."""
    stemmer = PorterStemmer()
    passed = 0

    for word, expected in TEST_CASES.items():
        got = stemmer.stem(word)
        ok = got == expected

        if ok:
            passed += 1
            print(f"{word:>16} -> {got:<12} OK")
        else:
            print(f"{word:>16} -> {got:<12} FAIL, expected: {expected}")

    print(f"\nPassed: {passed}/{len(TEST_CASES)}")


def main() -> None:
    """Запускает интерактивный режим.

    Команды:
    - :q    выход из программы;
    - :test запуск встроенных тестов;
    - любая другая строка разбивается на слова и обрабатывается стеммером.
    """
    stemmer = PorterStemmer()

    while True:
        line = input("> ").strip()

        if line == ":q":
            break

        if line == ":test":
            run_tests()
            continue

        for token in line.split():
            print(f"{token:>16} -> {stemmer.stem(token)}")


if __name__ == "__main__":
    main()
