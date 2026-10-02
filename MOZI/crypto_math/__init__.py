"""Пакет арифметики курса. Реализации живут в number_theory."""

from crypto_math.number_theory import (
    crt,
    divides,
    extended_gcd,
    garner_crt,
    gcd,
    mod_inverse,
    mod_pow,
    solve_linear,
    solve_linear_diophantine,
)

__all__ = [
    "divides",
    "gcd",
    "extended_gcd",
    "solve_linear_diophantine",
    "mod_inverse",
    "solve_linear",
    "mod_pow",
    "crt",
    "garner_crt",
]
