import math

import pytest
from hypothesis import given, strategies as st

from crypto_math.number_theory import (
    crt,
    divides,
    extended_gcd,
    garner_crt,
    gcd,
    mod_inverse,
    mod_pow,
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
