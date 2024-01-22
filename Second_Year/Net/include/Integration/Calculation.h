#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "ThreadPlanner.h"

double CalcIntegral (double begin, double end, double (*Function) (double x));
int Integrate (size_t nThreads, double begin, double end, double (*Function) (double), double* const result);
