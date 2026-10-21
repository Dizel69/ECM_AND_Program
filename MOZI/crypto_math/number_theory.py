"""Контракты ноутбуков 1–3.

В этом модуле нельзя подменять изучаемые алгоритмы готовыми
`math.gcd`, `pow(a, -1, n)`, SymPy или Sage: они допустимы только в тестах.
"""

import math
import secrets


def random_exact_bits(bits: int) -> int:
    """Возвращает положительное целое ровно заданной битовой длины."""
    if bits < 1:
        raise ValueError("bits должно быть положительным")

    n = secrets.randbits(bits)
    n |= 1 << (bits - 1)
    return n


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


def is_prime_fast(n: int) -> bool:
    """Простое ли n: перебор делителей до floor(sqrt(n)).

    Составное n = a*b, 1 < a <= b, обязательно имеет делитель a <= sqrt(n).
    Если такого делителя нет и n > 1, число простое. После двойки достаточно
    нечётных кандидатов: чётное d > 2 само делится на 2.
    """
    # 0, 1 и отрицательные простыми не бывают.
    if n < 2:
        return False
    # Единственное чётное простое — это 2.
    if n % 2 == 0:
        return n == 2
    # math.isqrt — точный floor(sqrt(n)). Возведение n ** 0.5 для больших n врёт.
    limit = math.isqrt(n)
    for d in range(3, limit + 1, 2):
        if n % d == 0:
            return False
    return True


def primes(n: int) -> list[int]:
    """Все простые p <= n. Решето из ноутбука 2; при n < 2 список пуст.

    В списке кандидатов остаются только нечётные. Пока очередное p не больше
    sqrt(n), вычёркиваем его кратные. Когда p уже больше sqrt(n), всё, что
    осталось в списке, простое: меньший простой делитель уже вычеркнул бы его.
    """
    if n < 2:
        return []

    # Двойку храним отдельно, чтобы дальше шагать по нечётным.
    candidates = list(range(3, n + 1, 2))
    found = [2]
    limit = math.isqrt(n)
    while candidates:
        p = candidates[0]
        if p > limit:
            found.extend(candidates)
            break
        found.append(p)
        # Кратные p составные. Само p в новый список не попадает: p % p == 0.
        candidates = [x for x in candidates if x % p != 0]
    return found


def next_prime(n: int) -> int:
    """Наименьшее простое число, строго большее n. При n < 2 это 2.

    После 2 все простые нечётны, поэтому чётный кандидат пропускается
    и дальше шаг равен 2.
    """
    if n < 2:
        return 2
    if n == 2:
        return 3
    candidate = n + 1
    if candidate % 2 == 0:
        candidate += 1
    while not is_prime_fast(candidate):
        candidate += 2
    return candidate


def smallest_factor(n: int) -> int | None:
    """Наименьший простой делитель n >= 2. Для простого n возвращает само n.

    Наименьший делитель d > 1 всегда прост: иначе у d был бы делитель ещё меньше,
    и он делил бы n. Для n < 2 простого делителя нет, возвращается None.
    """
    if n < 2:
        return None
    if n % 2 == 0:
        return 2
    for d in range(3, math.isqrt(n) + 1, 2):
        if n % d == 0:
            return d
    # Делителя до sqrt(n) нет, значит n само простое.
    return n


def decompose(n: int) -> dict[int, int]:
    """Каноническое разложение n >= 1: словарь {простое: показатель}.

    Для n = 1 простых множителей нет, словарь пустой. Показатель копится,
    пока smallest_factor возвращает одно и то же p.
    """
    if n < 1:
        raise ValueError("n must be >= 1")

    value = n
    factors: dict[int, int] = {}
    while value > 1:
        p = smallest_factor(value)
        # Одно и то же p может делить value несколько раз: 12 = 2^2 * 3.
        factors[p] = factors.get(p, 0) + 1
        value //= p
    return factors


def euler_phi(n: int) -> int:
    """Функция Эйлера φ(n) = n * ∏_{p|n} (1 - 1/p).

    Для каждого различного простого p | n вычитается n/p, затем (n - n/p)/q
    и так далее. Это и есть формула (3.8): на степень p^e множитель один,
    φ(p^e) = p^e - p^{e-1}.
    """
    if n < 1:
        raise ValueError("n must be >= 1")

    result = n
    for p in decompose(n):
        result -= result // p
    return result


def multiplicative_order(a: int, n: int, t: int | None = None) -> int:
    """Порядок a по модулю n: наименьшее k >= 1 с a^k ≡ 1 (mod n).

    По теореме Лагранжа порядок делит φ(n), поэтому искать его можно среди
    делителей известного кратного t. По умолчанию t = φ(n). Если разложение
    порядка группы уже известно (для простого p это p - 1), его передают в t
    и само n заново не раскладывают.

    Алгоритм: для каждого простого q | t, пока q делит t и a^{t/q} ≡ 1,
    делим t на q. Уменьшать t можно только тогда, когда порядок делит t/q.
    В конце ни один простой множитель убрать нельзя, значит t и есть порядок.
    """
    if n < 2:
        raise ValueError("n must be >= 2")
    if gcd(a, n) != 1:
        raise ValueError("a and n must be coprime")
    if t is None:
        t = euler_phi(n)
    if t < 1:
        raise ValueError("t must be a positive multiple of the order")

    a %= n
    # Простые множители фиксируем до того, как начнём уменьшать t.
    for q in decompose(t):
        while t % q == 0 and mod_pow(a, t // q, n) == 1:
            t //= q
    return t


def _primitive_root_modulus(n: int) -> bool:
    """Теорема 3.2: первообразный корень есть ровно при n = 2, 4, p^k, 2p^k.

    p — нечётное простое, k >= 1. Сначала отбрасываем лишнюю двойку:
    после одного деления на 2 остаток должен быть нечётным. Затем этот
    нечётный остаток обязан быть степенью одного простого.
    """
    if n in (2, 4):
        return True
    odd_part = n
    if odd_part % 2 == 0:
        odd_part //= 2
        # n делится на 4 и больше 4: это не 4 и не 2p^k.
        if odd_part % 2 == 0:
            return False
    if odd_part == 1:
        return False
    p = smallest_factor(odd_part)
    while odd_part % p == 0:
        odd_part //= p
    # Если после снятия p что-то осталось, простых множителей больше одного.
    return odd_part == 1


def is_primitive_root(g: int, n: int, t: int | None = None) -> bool:
    """Критерий (3.20): g — первообразный корень по модулю n.

    Порядок g равен φ(n) тогда и только тогда, когда g^{φ(n)/q} ≢ 1
    ни для одного простого q | φ(n). Любой меньший делитель φ(n) делит
    φ(n)/q хотя бы для одного такого q. t — порядок группы, по умолчанию φ(n).
    """
    if n < 2 or gcd(g, n) != 1:
        return False
    if t is None:
        t = euler_phi(n)
    g %= n
    for q in decompose(t):
        if mod_pow(g, t // q, n) == 1:
            return False
    return True


def primitive_root(n: int) -> int | None:
    """Наименьший первообразный корень по модулю n, либо None, если его нет.

    Сначала теорема 3.2 отсекает модули, у которых корня нет вообще.
    Если форма модуля подходящая, кандидаты идут по возрастанию, поэтому
    первый успех критерия (3.20) — наименьший корень.
    """
    if n < 2 or not _primitive_root_modulus(n):
        return None
    group_order = euler_phi(n)
    for g in range(1, n):
        if gcd(g, n) == 1 and is_primitive_root(g, n, group_order):
            return g
    return None
