#include "solve_square.h"

//Решение квадратного уравнения
//Функцие передаются коэффициенты квадратного уравнения и адреса на корни уравнения
//! Solves a square equation ax2 + bx + c = 0
//! @param[in] a a-coefficient
//! @param[in] b b-coefficient
//! @param[in] c c-coefficient
//! @param[out] x1 Pointer to the first root
//! @param[out] x2 Pointer to the second root
//!
//! @return Number of Roots
int SquareSolver(double a, double b, double c, double* x1, double* x2) {

    assert(isfinite (a));
    assert(isfinite (b));
    assert(isfinite (c));

    assert(x1 != x2);
    assert(x1 != NULL);
    assert(x2 != NULL);

    if (IsEqual(a, 0)) {
        if (IsEqual(b, 0)) {
            if (IsEqual(c,0))
                return Infinite;
            else
                return NoSolutions;
        } else {
            *x1 = LinearEquation(b,c);
            return 1;
        }
    } else {
        double DiscriminantTemp = 0;
        if ((b * b - 4 * a * c) < 0)
            return NoSolutions;
        else DiscriminantTemp= sqrt(b * b - 4 * a * c);
        if (IsEqual(DiscriminantTemp, 0)) {
            *x1 = -b / (2 * a);
            return 1;
        }
        if (DiscriminantTemp < 0) {
            return NoSolutions;
        }
        if (DiscriminantTemp > 0) {
            *x1 = (-b + DiscriminantTemp) / (2 * a);
            *x2 = (-b - DiscriminantTemp) / (2 * a);
            return 2;
        }
    }
}