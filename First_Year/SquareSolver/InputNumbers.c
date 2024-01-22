#include "solve_square.h"

double *inputNumbers(char * Welcome, int nNumbers) {

    printf("%s\n", Welcome);

    double *data = (double *) (calloc(nNumbers, sizeof(data[0])));

    int check = 0;
    for (int index = 0; index < nNumbers; index++) {
        while (check == 0) {
            if (scanf("%lg", &data[index]) != 1) {
                printf("Error occured, try to input numbers again");
                while (!feof(stdin)) {
                    getchar();
                }
            } else check++;
        }
        check = 0;
    }
    return data;
}
