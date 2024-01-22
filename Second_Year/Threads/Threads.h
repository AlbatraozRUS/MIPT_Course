#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <pthread.h>
#include <stdbool.h>

#define CASHE_INFO_FILE "/sys/bus/cpu/devices/cpu0/cache/index0/coherency_line_size"

#define PRINTERROR(errorString)                         \
    {                                                   \
        fprintf(stderr, "ATTENTION!\nError occured: "); \
        perror("");                                     \
        fprintf(stderr, errorString);                   \
        exit(EXIT_FAILURE);                             \
    }


typedef struct ThreadData {
    double sum;
    double startX;
    double finishX;
    double deltaX;
    int numCPU;

} ThreadData;

void PrepareThreads(void* data, size_t sizeThreadData, 
                    int nThreads, int nEmptyThreads, double threadLength);
void *AllocThreadData(size_t num_threads, size_t *size);
void* pthreadStartFunc(void* arg);
double function(double x);
size_t ScanNum(char *number_str);
