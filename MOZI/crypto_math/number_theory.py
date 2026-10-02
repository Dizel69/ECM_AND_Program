"""Контракты ноутбука 1.

В этом модуле нельзя подменять изучаемые алгоритмы готовыми
`math.gcd`, `pow(a, -1, n)`, SymPy или Sage: они допустимы только в тестах.
"""


def divides(a: int, b: int) -> bool:
    """True тогда и только тогда, когда a делит b.

    Крайний случай: 0 | b  <=>  b == 0.
    """
    if a == 0:
        return b == 0
    return b % a == 0


def gcd(a: int, b: int) -> int:
    """Положительный НОД a и b.

    gcd(0, 0) не определён и должен вызывать ValueError.
    Соглашения курса: gcd(a, b) >= 0 и gcd(a, 0) == abs(a).
    """
    if a == 0 and b == 0:
        raise ValueError("gcd(0, 0) is undefined by the course convention")

    a, b = abs(a), abs(b)
    while b:
        a, b = b, a % b
    return a


def extended_gcd(a: int, b: int) -> tuple[int, int, int]:
    """Возвращает (g, x, y), где g = gcd(a, b) и a*x + b*y == g.

    gcd(0, 0) не определён и должен вызывать ValueError.
    Инварианты (1.15) оставлены в цикле: они фиксируют доказательство,
    а не заменяют его готовой функцией.
    """
    if a == 0 and b == 0:
        raise ValueError("gcd(0, 0) is undefined by the course convention")

    # Знаки исходных чисел возвращаются только в конце.
    # В цикле оба остатка неотрицательны.
    original_a, original_b = a, b
    a, b = abs(a), abs(b)

    old_r, r = a, b
    old_s, s = 1, 0
    old_t, t = 0, 1

    while r != 0:
        assert old_r == old_s * a + old_t * b
        assert r == s * a + t * b

        quotient = old_r // r
        old_r, r = r, old_r - quotient * r
        old_s, s = s, old_s - quotient * s
        old_t, t = t, old_t - quotient * t

    assert old_r == old_s * a + old_t * b

    x = old_s if original_a >= 0 else -old_s
    y = old_t if original_b >= 0 else -old_t
    return old_r, x, y


def solve_linear_diophantine(
    a: int, b: int, c: int
) -> tuple[int, int, int, int] | None:
    """Частное решение ax + by = c и шаг общего решения.

    Возвращает (x0, y0, dx, dy) либо None, если gcd(a, b) не делит c.
    Все целые решения тогда имеют вид x = x0 + dx*t, y = y0 + dy*t.
    Шаг канонический: dx = b / g, dy = -a / g, где g = gcd(a, b).
    Случай a = b = 0 отклоняется через ValueError.
    """
    if a == 0 and b == 0:
        raise ValueError("equation 0*x + 0*y = c is degenerate")

    g, x, y = extended_gcd(a, b)
    if c % g != 0:
        return None

    factor = c // g
    return (x * factor, y * factor, b // g, -(a // g))


def mod_inverse(a: int, modulus: int) -> int:
    """Обратный элемент a по модулю modulus, в диапазоне 0 <= inv < modulus.

    modulus должен быть не меньше 2. Если gcd(a, modulus) != 1, ValueError.
    """
    if modulus < 2:
        raise ValueError("modulus must be >= 2")

    g, x, _ = extended_gcd(a, modulus)
    if g != 1:
        raise ValueError("inverse does not exist")
    return x % modulus


def solve_linear(a: int, b: int, n: int) -> list[int]:
    """Все решения ax ≡ b (mod n) в диапазоне 0 <= x < n, по возрастанию.

    Если решений нет, возвращает пустой список. n >= 2.
    Решений ровно gcd(a, n), когда этот gcd делит b. Шаг между ними равен n/g.
    """
    if n < 2:
        raise ValueError("n must be >= 2")

    g, coefficient, _ = extended_gcd(a, n)
    if b % g != 0:
        return []

    x0 = coefficient * (b // g)
    step = n // g
    return sorted((x0 + k * step) % n for k in range(g))


def mod_pow(base: int, exponent: int, modulus: int) -> int:
    """base**exponent mod modulus для exponent >= 0 и modulus > 0.

    Двоичный алгоритм справа налево. Число итераций равно битовой длине
    показателя, а не самому показателю.
    """
    if modulus <= 0:
        raise ValueError("modulus must be positive")
    if exponent < 0:
        raise ValueError("this implementation expects exponent >= 0")

    result = 1 % modulus
    base %= modulus
    e = exponent
    while e:
        if e & 1:
            result = (result * base) % modulus
        base = (base * base) % modulus
        e >>= 1
    return result


def _require_pairwise_coprime(moduli: list[int]) -> None:
    if any(modulus < 2 for modulus in moduli):
        raise ValueError("all moduli must be >= 2")
    for i, left in enumerate(moduli):
        for right in moduli[i + 1 :]:
            if gcd(left, right) != 1:
                raise ValueError("moduli must be pairwise coprime")


def crt(residues: list[int], moduli: list[int]) -> int:
    """Наименьший неотрицательный x по китайской теореме об остатках.

    Модули попарно взаимно просты и не меньше 2.
    """
    if len(residues) != len(moduli) or not residues:
        raise ValueError("residues and moduli must have equal non-zero length")
    _require_pairwise_coprime(moduli)

    product = 1
    for modulus in moduli:
        product *= modulus

    x = 0
    for residue, modulus in zip(residues, moduli):
        part = product // modulus
        x += residue * part * mod_inverse(part, modulus)
    return x % product


def garner_crt(residues: list[int], moduli: list[int]) -> int:
    """То же решение, что у crt, алгоритмом Гарнера."""
    if len(residues) != len(moduli) or not residues:
        raise ValueError("residues and moduli must have equal non-zero length")
    _require_pairwise_coprime(moduli)

    x = residues[0] % moduli[0]
    product = moduli[0]
    for residue, modulus in zip(residues[1:], moduli[1:]):
        coefficient = ((residue - x) * mod_inverse(product, modulus)) % modulus
        x += coefficient * product
        product *= modulus
    return x
