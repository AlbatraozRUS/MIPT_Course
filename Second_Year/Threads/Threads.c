#include "Threads.h"

static const double STARTX = 0;
static const double FINISHX = 100;
static const double DELTAX = 0.0000001;

int main(int argc, char** argv)
{
    if (argc != 2)
        PRINTERROR("Incorrect input")

    int nThreads = ScanNum(argv[1]);

    int nProc = get_nprocs();

    int nEmptyThreads = 0;
    if (nProc > nThreads)
        nEmptyThreads = nProc - nThreads;

    size_t sizeThreadData = 0;
    void* threadsData = AllocThreadData(nThreads + nEmptyThreads, &sizeThreadData);
    if (threadsData == NULL)
        PRINTERROR("Can`t allocate memory for threadsData")

    pthread_t* threadsID = (pthread_t*)calloc(nThreads + nEmptyThreads, sizeof(pthread_t));
    if (threadsID == NULL)
        PRINTERROR("Can`t allocate memory for threadsID")

    PrepareThreads(threadsData, sizeThreadData, 
                   nThreads, nEmptyThreads, (FINISHX - STARTX) / nThreads);

    for (int iThread = 0; iThread < nThreads + nEmptyThreads; iThread++) {
        if (nEmptyThreads > 0)
            ((ThreadData*)(threadsData + iThread * sizeThreadData))->numCPU = iThread;

        if (pthread_create(threadsID + iThread, NULL, pthreadStartFunc,
                          (threadsData + iThread * sizeThreadData)) != 0)
            PRINTERROR("Can`t create thread")
    }

    double sum = 0;
    for (long iThread = 0; iThread < nThreads; iThread++) {
        if (pthread_join(threadsID[iThread], NULL) != 0)
            PRINTERROR("Something wrong with pthread_join")

        sum += ((ThreadData*)(threadsData + iThread * sizeThreadData))->sum;
    }

    printf("Result - %lg", sum);

    free(threadsID);
    free(threadsData);

    return 0;
}

void PrepareThreads(void* data, size_t sizeThreadData, 
                    int nThreads, int nEmptyThreads, double threadLength)
{
    for (int iThread = 0; iThread < nThreads; iThread++) {
        ((ThreadData*)(data + iThread * sizeThreadData))->startX = 
                STARTX + iThread * threadLength;

        ((ThreadData*)(data + iThread * sizeThreadData))->finishX =
                ((ThreadData *)(data + iThread * sizeThreadData))->startX + threadLength;

        ((ThreadData*)(data + iThread * sizeThreadData))->deltaX = DELTAX;
        ((ThreadData*)(data + iThread * sizeThreadData))->numCPU = -1;
    }

    for (int iThread = 0; iThread < nEmptyThreads; iThread++) {
        ((ThreadData*)(data + (nThreads + iThread) * sizeThreadData))->startX = STARTX;
        ((ThreadData*)(data + (nThreads + iThread) * sizeThreadData))->finishX = 
                STARTX + ((FINISHX - STARTX) / nThreads);

        ((ThreadData*)(data + (nThreads + iThread) * sizeThreadData))->deltaX = DELTAX;
        ((ThreadData*)(data + (nThreads + iThread) * sizeThreadData))->numCPU = -1;
    }
}


void* AllocThreadData(size_t nThreads, size_t *size)
{
    FILE* cacheInfo = fopen(CASHE_INFO_FILE, "r");
    if (cacheInfo == NULL)
        PRINTERROR("Can`t count cache")

    int lineSize = 0;
    int ret = fscanf(cacheInfo, "%d", &lineSize);
    if (ret != 1)
      PRINTERROR("Can`t scanf cache")

    size_t objectSize = (sizeof(ThreadData) / lineSize + 2) * lineSize;

    *size = objectSize;

    return malloc(nThreads * objectSize);
}

void* pthreadStartFunc(void* arg)
{
    ThreadData* threadsData = (ThreadData*) arg;

    cpu_set_t cpu = {};
    pthread_t id = pthread_self();
    int numCPU = ((ThreadData*)threadsData)->numCPU;

    if (numCPU > 0){
      CPU_ZERO(&cpu);
      CPU_SET(numCPU, &cpu);

      if (pthread_setaffinity_np(id, sizeof(cpu_set_t), &cpu) < 0)
        PRINTERROR("Error in setaffinity")
    }

    for (double x = threadsData->startX; x < threadsData->finishX; x += threadsData->deltaX)
        threadsData->sum += function(x) * threadsData->deltaX;

    return NULL;
}

double function(double x)
{
    return x;
}

size_t ScanNum(char* strToNumberise)
{
    char* endptr = (char*)calloc(10, 1);

    unsigned long long N = strtoul(strToNumberise, &endptr, 10);
    if (*endptr != '\0' || errno == ERANGE)
    {
        fprintf(stderr, "Something is wrong with your input\n");
        abort();
    }
    return N;
}
