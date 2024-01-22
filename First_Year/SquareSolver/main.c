//  Клянусь

#include "solve_square.h"

const int Infinite = -1;
const int NoSolutions = -2;
const float Inaccuracy = 0.000000000005;


//Проверка на погрешность
//Функция получает 2 числа, которые необходимо сравнить
//! @param[in] a First digit
//! @param[in] b Second Digit
//! @return If digits are equal

bool IsEqual(double a, double b){
    if (fabs(fabs(a) - fabs(b)) < Inaccuracy)
        return true;
    else return false;
}

//Решение линейного уравнения
//Функцие передаются коэффициенты "b" и "c"
//! @param[in] c c-coefficient
//! @param[in] b b-coefficient
//! @param[out] X1 The answer

double LinearEquation (double b, double c) {
    return -c / b;
}

//Главная функция
//! @file main.c
//! @mainpage
//! - main.c
//! Note If there is "Error" or "All bad" check your input

int main() {

    printf("Welcome to Square Solver\n");
    int nNumbers = 0;
    printf("Please, enter the number of coefficients\n");
    int check = 0;
    while (check == 0) {
        if (scanf("%d", &nNumbers) != 1) {
            printf("Error occured. Please check number of coefficients!\n");
            while (!feof(stdin)) {
                getchar();
            }
        }
        else check++;
    }

    double *coeff = inputNumbers("Input your coefficient", nNumbers);

    double x1 = 0, x2 = 0;
    int nRoots = SquareSolver(coeff[0], coeff[1], coeff[2], &x1, &x2);

    free(coeff);

    switch (nRoots) {
        case NoSolutions :
            printf("There are no solutions\n");
            break;
        case Infinite :
            printf("The number of solutions is infinite\n");
            break;
        case 1 :
            printf("Equation has 1 solution\n");
            printf("The answer is %lg\n", x1);
            break;
        case 2 :
            printf("There are two solutions\n");
            printf("One of them is %lg another is %lg\n", x1, x2);
            break;
        default:
            printf("All bad\n");
 }

    printf("Do you want to run tests?\n");
    printf("Yes - 1, No - 0\n");
    int answer = 0;
    scanf("%d", &answer);
    if (answer > 0){
        bool ResultofTests = Unit_Tests();
        if (ResultofTests == true)
            printf("Tests are failed");
        else
            printf("Test completed succesfully");
    }

    return 0;
}

