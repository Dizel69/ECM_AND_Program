import math

import pytest
from hypothesis import given, strategies as st

from crypto_math.number_theory import (
    crt,
    decompose,
    divides,
    euler_phi,
    extended_gcd,
    garner_crt,
    gcd,
    is_prime_fast,
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
