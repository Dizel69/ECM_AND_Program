import math

import pytest
from hypothesis import given, strategies as st

from crypto_math.number_theory import (
    crt,
    decompose,
    divides,
    euler_phi,
    extended_gcd,
    find_g_safe,
    find_p_g,
    garner_crt,
    gcd,
    generate_safe_prime,
    is_prime_fast,
    is_prime_mr,
    miller_rabin,
    miller_rabin_round,
    mod_inverse,
    mod_pow,
    multiplicative_order,
    next_prime,
    primes,
    random_exact_bits,
    solve_linear,
    solve_linear_diophantine,
)

nonzero_pairs = st.tuples(st.integers(), st.integers()).filter(lambda p: p != (0, 0))


@given(st.integers(), st.integers())
def test_divides_matches_definition(a, b):
    assert divides(a, b) == (b == 0 if a == 0 else b % a == 0)


@given(nonzero_pairs)
def test_gcd_matches_reference(pair):
    a, b = pair
    assert gcd(a, b) == math.gcd(a, b)


@given(nonzero_pairs)
def test_extended_gcd_bezout(pair):
    a, b = pair
    g, x, y = extended_gcd(a, b)
    assert g == math.gcd(a, b)
    assert a * x + b * y == g


def test_gcd_rejects_zero_zero():
    with pytest.raises(ValueError):
        gcd(0, 0)


@given(st.integers(), st.integers(min_value=2))
def test_mod_inverse_contract(a, n):
    if math.gcd(a, n) == 1:
        inv = mod_inverse(a, n)
        assert 0 <= inv < n and (a * inv) % n == 1
    else:
        with pytest.raises(ValueError):
            mod_inverse(a, n)


@given(
    st.integers(),
    st.integers(min_value=0, max_value=2**64),
    st.integers(min_value=1),
)
def test_mod_pow_matches_builtin(a, e, n):
    assert mod_pow(a, e, n) == pow(a, e, n)


MODULI = [3, 5, 7, 11, 13, 17]


@given(st.integers())
def test_crt_roundtrip(x):
    N = math.prod(MODULI)
    residues = [x % m for m in MODULI]
    assert crt(residues, MODULI) == garner_crt(residues, MODULI) == x % N


# Частное решение уравнения Безу не единственно, поэтому тест не сверяет
# конкретную пару (x0, y0). Проверяется само уравнение, канонический шаг
# (b/g, -a/g) и отказ, когда g не делит c.
@given(st.integers(), st.integers(), st.integers())
def test_solve_linear_diophantine_property(a, b, c):
    if a == 0 and b == 0:
        with pytest.raises(ValueError):
            solve_linear_diophantine(a, b, c)
        return

    g = math.gcd(a, b)
    solution = solve_linear_diophantine(a, b, c)
    if c % g != 0:
        assert solution is None
        return

    x0, y0, dx, dy = solution
    assert a * x0 + b * y0 == c
    assert a * dx + b * dy == 0
    assert dx == b // g
    assert dy == -(a // g)
    assert a * (x0 + dx) + b * (y0 + dy) == c


# Полный список решений сравнения конечен, поэтому его можно сверить
# с определением: перебор x = 0, ..., n-1 допустим в тесте.
# Модуль ограничен, иначе при g = n список решений имеет длину n.
@given(
    st.integers(),
    st.integers(),
    st.integers(min_value=2, max_value=2**12),
)
def test_solve_linear_matches_definition(a, b, n):
    found = solve_linear(a, b, n)
    expected = [x for x in range(n) if (a * x - b) % n == 0]
    assert found == expected


def test_solve_linear_rejects_small_modulus():
    with pytest.raises(ValueError):
        solve_linear(1, 1, 1)


def test_random_exact_bits_has_requested_length():
    for bits in (1, 2, 8, 64, 512):
        value = random_exact_bits(bits)
        assert value > 0
        assert value.bit_length() == bits


def test_random_exact_bits_rejects_non_positive():
    with pytest.raises(ValueError):
        random_exact_bits(0)


def test_is_prime_fast_agrees_with_primes_up_to_10000():
    # Одно и то же множество простых двумя алгоритмами: перебор делителей и решето.
    # Граница 10^4 включена, primes(limit) — это простые p <= limit.
    limit = 10_000
    assert primes(limit) == [n for n in range(limit + 1) if is_prime_fast(n)]


@pytest.mark.parametrize("n", list(range(-3, 400)) + [10**4, 10**5])
def test_next_prime_is_the_immediate_successor(n):
    # next_prime(n) простое, строго больше n, и между ними простых нет.
    # Для n < 2 ответ равен 2: отрицательные и 0, 1 тоже попадают в этот контракт.
    prime = next_prime(n)
    assert prime > n
    assert is_prime_fast(prime)
    assert not any(is_prime_fast(candidate) for candidate in range(n + 1, prime))


def _recompose(factors: dict[int, int]) -> int:
    # Обратная операция к decompose: пустой словарь даёт 1, иначе произведение p^e.
    value = 1
    for prime, exponent in factors.items():
        value *= prime**exponent
    return value


def _divisors(n: int) -> list[int]:
    # Все делители по разложению. На каждый простой p^e список умножается
    # на 1, p, ..., p^e. Для взаимно простых множителей это даёт каждый делитель один раз.
    divisors = [1]
    for prime, exponent in decompose(n).items():
        divisors = [
            divisor * prime**power
            for divisor in divisors
            for power in range(exponent + 1)
        ]
    return divisors


@given(st.integers(min_value=1, max_value=4000))
def test_recompose_inverts_decompose(n):
    # Разложение и сборка взаимно обратны: канонический вид не теряет множители.
    assert _recompose(decompose(n)) == n


@given(
    st.integers(min_value=2, max_value=3000),
    st.integers(min_value=0, max_value=3000),
)
def test_euler_theorem_for_units(n, a):
    # Теорема Эйлера: если gcd(a, n) = 1, то a^{φ(n)} ≡ 1 (mod n).
    # Необратимый a в проверку не входит: для него степень единицы не обещана.
    residue = a % n
    if math.gcd(residue, n) != 1:
        return
    assert pow(residue, euler_phi(n), n) == 1


@given(
    st.integers(min_value=2, max_value=2000),
    st.integers(min_value=0, max_value=2000),
)
def test_multiplicative_order_divides_group_order(n, a):
    # Порядок элемента делит порядок группы, и a^{порядок} ≡ 1.
    # Второе равенство ловит ответ, который делит φ(n), но слишком большой.
    residue = a % n
    if math.gcd(residue, n) != 1:
        return
    order = multiplicative_order(residue, n)
    assert euler_phi(n) % order == 0
    assert pow(residue, order, n) == 1


@given(st.integers(min_value=1, max_value=1500))
def test_euler_phi_sums_to_n_over_divisors(n):
    # Тождество (3.18): сумма φ(d) по всем d | n равна n.
    assert sum(euler_phi(d) for d in _divisors(n)) == n


def test_miller_rabin_rejects_bad_rounds_and_handles_small_n():
    # t — число раундов. Ноль, отрицательное и bool не являются положительным целым.
    for bad_t in (0, -1, True):
        with pytest.raises(ValueError):
            miller_rabin(5, bad_t)
        with pytest.raises(ValueError):
            is_prime_mr(5, bad_t)
    # Малые и чётные значения не зависят от случайных оснований.
    # -2..1 не простые, 2, 3 и 5 простые, 4 и 6 чётные составные.
    assert [miller_rabin(n, 1) for n in range(-2, 7)] == [
        False,
        False,
        False,
        False,
        True,
        True,
        False,
        True,
        False,
    ]
    assert [is_prime_mr(n) for n in range(-2, 7)] == [
        False,
        False,
        False,
        False,
        True,
        True,
        False,
        True,
        False,
    ]


def test_single_round_is_not_a_primality_test():
    # 2047 сильно псевдопросто по основанию 2, но составное.
    # Один пройденный раунд и ответ теста простоты здесь расходятся.
    assert miller_rabin_round(2047, 2) is True
    assert is_prime_mr(2047) is False
    # 91: основание 2 — свидетель, основание 9 — сильный лжец.
    assert miller_rabin_round(91, 2) is False
    assert miller_rabin_round(91, 9) is True
    # В диапазоне 2..7 у числа 9 сильных лжецов нет, поэтому даже один раунд отвергает его.
    assert miller_rabin(9, 1) is False


def test_is_prime_mr_matches_exact_test_on_a_small_range():
    # Сравнение в обе стороны: и простые принимаются, и составные отклоняются.
    # Диапазон маленький, точный перебор делителей здесь ещё практичен.
    for n in range(-5, 2001):
        assert is_prime_mr(n) is is_prime_fast(n)


def test_deterministic_branch_catches_pseudoprime_without_small_factor():
    # 25 326 001 = 2251 * 11251. Оба множителя больше 997, пробное деление их не видит.
    # Число сильно псевдопросто по основаниям 2, 3 и 5, но не по всему набору из 12.
    assert miller_rabin_round(25326001, 2) is True
    assert is_prime_mr(25326001) is False
    # Произведение двух простых больше 997 тоже доходит до детерминированной ветви.
    assert is_prime_mr(1009 * 1013) is False
    assert is_prime_mr(1000000000061) is True
    assert is_prime_mr(1000000000063) is True


def test_generate_safe_prime_contract():
    for bad_bits in (3, 0, -4, True):
        with pytest.raises(ValueError):
            generate_safe_prime(bad_bits)
    with pytest.raises(ValueError):
        generate_safe_prime(8, 0)
    # При 4 и 5 битах подходящая пара единственна, случайный перебор обязан её найти.
    assert generate_safe_prime(4) == (11, 5)
    assert generate_safe_prime(5) == (23, 11)
    for bits in (8, 12, 16):
        p, q = generate_safe_prime(bits)
        assert p == 2 * q + 1
        assert p.bit_length() == bits
        assert q.bit_length() == bits - 1
        assert is_prime_fast(p) and is_prime_fast(q)


def test_find_g_safe_returns_least_primitive_root():
    assert find_g_safe(23, 11) == 5
    assert pow(5, 11, 23) == 22
    assert multiplicative_order(5, 23, 22) == 22
    assert find_g_safe(11, 5) == 2
    with pytest.raises(ValueError):
        find_g_safe(23, 10)
    # 15 = 2*7+1, но 15 составное: ни один g не даёт остаток 14, это ошибка предпосылки.
    with pytest.raises(ValueError):
        find_g_safe(15, 7)


def test_find_p_g_matches_safe_prime_and_root():
    p, q, g = find_p_g(12)
    assert p == 2 * q + 1
    assert g == find_g_safe(p, q)
    assert pow(g, q, p) == p - 1
    assert is_prime_fast(p) and is_prime_fast(q)
    assert multiplicative_order(g, p, p - 1) == p - 1
