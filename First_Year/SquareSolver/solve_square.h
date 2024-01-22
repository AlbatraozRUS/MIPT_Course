//
// Created by Искандер Шакирзянов on 16/09/2019.
//

#ifndef DED1_SOLVE_SQUARE_H
#define DED1_SOLVE_SQUARE_H



#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>


extern const int Infinite;
extern const int NoSolutions;
extern const float Inaccuracy;


double *inputNumbers(char * Welcome, int nNumbers);
bool IsEqual(double a, double b);
double LinearEquation (double b, double c);
int SquareSolver(double a, double b, double c, double* x1, double* x2);
bool Unit_Tests ();



#endif //DED1_SOLVE_SQUARE_H
