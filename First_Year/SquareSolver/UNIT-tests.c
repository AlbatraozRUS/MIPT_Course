#include "solve_square.h"


//Считывание коэффициента
//!@param[in] Welcome - Introduction for user
//!@param[in]nNumbers - Number of coefficients
//!@return Array of coefficients
bool Unit_Tests () {

    bool Result = false;

    double coeff[3] = {0, 0, 0};
    double x1 = 0, x2 = 0;


    coeff[0] = 0;
    coeff[1] = 0;
    coeff[2] = 0;
    x1 = 0;
    x2 = 0;

    SquareSolver(coeff[0], coeff[1], coeff[2], &x1, &x2);
    if (coeff[0] * x1 * x1 + coeff[1] * x1 + coeff[2] != 0 || coeff[0] * x2 * x2 + coeff[1] * x2 + coeff[2] != 0) {
        printf("Error of test. Coefficients are %lg , %lg , %lg\n", coeff[0], coeff[1], coeff[2]);
        Result = true;
    }

    coeff[0] = 0;
    coeff[1] = 0;
    coeff[2] = 1;
    x1 = 0;
    x2 = 0;


    if (SquareSolver(coeff[0], coeff[1], coeff[2], &x1, &x2) != Infinite ){
        printf("Error of test. Coefficients are %lg , %lg , %lg\n", coeff[0], coeff[1], coeff[2]);
        Result = true;
    }

    coeff[0] = 0;
    coeff[1] = 1;
    coeff[2] = 1;
    x1 = 0;
    x2 = 0;

    SquareSolver(coeff[0], coeff[1], coeff[2], &x1, &x2);
    if ((coeff[1] * LinearEquation(coeff[1], coeff[2]) + coeff[2] != 0)) {
        printf("Error of test. Coefficients are %lg , %lg , %lg\n", coeff[0], coeff[1], coeff[2]);
        Result = true;
    }

    coeff[0] = 1;
    coeff[1] = 0;
    coeff[2] = 0;
    x1 = 0;
    x2 = 0;

    SquareSolver(coeff[0], coeff[1], coeff[2], &x1, &x2);
    if (coeff[0]*x1*x1+coeff[1]*x1+coeff[2] != 0 || coeff[0]*x2*x2+coeff[1]*x2+coeff[2] != 0) {
        printf("Error of test. Coefficients are %lg , %lg , %lg\n", coeff[0], coeff[1], coeff[2]);
        Result = true;
    }

    coeff[0] = 1;
    coeff[1] = 0;
    coeff[2] = 1;
    x1 = 0;
    x2 = 0;

    if (SquareSolver(coeff[0], coeff[1], coeff[2], &x1, &x2) != NoSolutions) {
        printf("Error of test. Coefficients are %lg , %lg , %lg\n", coeff[0], coeff[1], coeff[2]);
        Result = true;
    }

    coeff[0] = 1;
    coeff[1] = 1;
    coeff[2] = 1;
    x1 = 0;
    x2 = 0;

    if (SquareSolver(coeff[0], coeff[1], coeff[2], &x1, &x2) != NoSolutions) {
        printf("Error of test. Coefficients are %lg , %lg , %lg\n", coeff[0], coeff[1], coeff[2]);
        Result = true;
    }

    coeff[0] = 100;
    coeff[1] = 1;
    coeff[2] = 100;
    x1 = 0;
    x2 = 0;

    if (SquareSolver(coeff[0], coeff[1], coeff[2], &x1, &x2) != NoSolutions) {
        printf("Error of test. Coefficients are %lg , %lg , %lg\n", coeff[0], coeff[1], coeff[2]);
        Result = true;
    }

    return Result;

}