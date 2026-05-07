from __future__ import annotations

from typing import Optional, Tuple


class PorterStemmer:
    """
    Стихеммер для английских слов по алгоритму Портера.

    Алгоритм поэтапно отбрасывает/заменяет суффиксы, ориентируясь на:
    - наличие гласных;
    - меру слова (m);
    - двойные согласные;
    - окончание вида CVC.
    """

    VOWELS = set("aeiou")

    def stem(self, word: str) -> str:
        """
        Возвращает основу слова.
        """
        if not word:
            return word

        word = word.lower()

        # Оставляем только латиницу a-z.
        if any(not ("a" <= ch <= "z") for ch in word):
            return word

        if len(word) <= 2:
            return word

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

        if ch in self.VOWELS:
            return False

        if ch == "y":
            if index == 0:
                return True
            return not self._is_consonant(word, index - 1)

        return True

    def _measure(self, word: str) -> int:
        """
        Вычисляет меру слова m:
        число чередований VC в представлении [C](VC)^m[V].
        """
        length = len(word)
        i = 0
        measure = 0

        while i < length and self._is_consonant(word, i):
            i += 1

        while i < length:
            while i < length and not self._is_consonant(word, i):
                i += 1

            if i >= length:
                break

            while i < length and self._is_consonant(word, i):
                i += 1

            measure += 1

        return measure

    def _has_vowel(self, word: str) -> bool:
        """
        Проверка условия *v*: есть ли хотя бы одна гласная.
        """
        return any(not self._is_consonant(word, i) for i in range(len(word)))

    def _double_consonant(self, word: str) -> bool:
        """
        Проверка условия *d: слово оканчивается на двойную согласную.
        """
        if len(word) < 2:
            return False

        if word[-1] != word[-2]:
            return False

        return self._is_consonant(word, len(word) - 1)

    def _cvc(self, word: str) -> bool:
        """
        Проверка условия *o:
        слово оканчивается на CVC, а последняя согласная не w/x/y.
        """
        if len(word) < 3:
            return False

        last = len(word) - 1

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
        if not word.endswith(suffix):
            return word, False

        stem = word[:-len(suffix)]

        if require_vowel and not self._has_vowel(stem):
            return word, False

        m = self._measure(stem)

        if min_m is not None and not (m > min_m):
            return word, False

        if eq_m is not None and not (m == eq_m):
            return word, False

        return stem + replacement, True

    # ---------------------------
    # Шаги алгоритма Портера
    # ---------------------------

    def _step1a(self, word: str) -> str:
        if word.endswith("sses"):
            return word[:-2]
        if word.endswith("ies"):
            return word[:-2]
        if word.endswith("ss"):
            return word
        if word.endswith("s"):
            return word[:-1]
        return word

    def _step1b(self, word: str) -> str:
        if word.endswith("eed"):
            stem = word[:-3]
            if self._measure(stem) > 0:
                return stem + "ee"
            return word

        if word.endswith("ed"):
            stem = word[:-2]
            if self._has_vowel(stem):
                return self._step1b_after_drop(stem)
            return word

        if word.endswith("ing"):
            stem = word[:-3]
            if self._has_vowel(stem):
                return self._step1b_after_drop(stem)
            return word

        return word

    def _step1b_after_drop(self, word: str) -> str:
        """
        Постобработка после удаления -ed или -ing.
        """
        for suffix, replacement in (("at", "ate"), ("bl", "ble"), ("iz", "ize")):
            if word.endswith(suffix):
                return word[:-len(suffix)] + replacement

        if self._double_consonant(word) and word[-1] not in ("l", "s", "z"):
            return word[:-1]

        if self._measure(word) == 1 and self._cvc(word):
            return word + "e"

        return word

    def _step1c(self, word: str) -> str:
        if word.endswith("y"):
            stem = word[:-1]
            if self._has_vowel(stem):
                return stem + "i"
        return word

    def _step2(self, word: str) -> str:
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

        for suffix, replacement in sorted(replacements, key=lambda item: len(item[0]), reverse=True):
            if word.endswith(suffix):
                new_word, _ = self._replace_if(word, suffix, replacement, min_m=0)
                return new_word

        return word

    def _step3(self, word: str) -> str:
        replacements = [
            ("icate", "ic"),
            ("ative", ""),
            ("alize", "al"),
            ("iciti", "ic"),
            ("ical", "ic"),
            ("ful", ""),
            ("ness", ""),
        ]

        for suffix, replacement in sorted(replacements, key=lambda item: len(item[0]), reverse=True):
            if word.endswith(suffix):
                new_word, _ = self._replace_if(word, suffix, replacement, min_m=0)
                return new_word

        return word

    def _step4(self, word: str) -> str:
        """
        Удаляет часть суффиксов на основе меры m.
        Отдельно обработан случай -ion: он удаляется только после s или t.
        """
        suffixes = [
            "al", "ance", "ence", "er", "ic", "able", "ible", "ant",
            "ement", "ment", "ent", "ou", "ism", "ate", "iti", "ous", "ive", "ize",
        ]

        if word.endswith("ion") and len(word) > 3:
            stem = word[:-3]
            if stem and stem[-1] in ("s", "t") and self._measure(stem) > 1:
                return stem
            return word

        for suffix in sorted(suffixes, key=len, reverse=True):
            if word.endswith(suffix):
                new_word, _ = self._replace_if(word, suffix, "", min_m=1)
                return new_word

        return word

    def _step5a(self, word: str) -> str:
        if word.endswith("e"):
            stem = word[:-1]
            m = self._measure(stem)

            if m > 1:
                return stem
            if m == 1 and not self._cvc(stem):
                return stem

        return word

    def _step5b(self, word: str) -> str:
        if self._measure(word) > 1 and word.endswith("ll"):
            return word[:-1]
        return word


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
