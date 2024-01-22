#include "Integration/ThreadPlanner.h"

static int Find (const int* arr, int size, int elem) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == elem)
            return i;
    }
    return -1;
}

static int AddCPUArray (CPU** arr, size_t* size, CPU elem, size_t position) {

    if (position >= *size) {
        *arr = (CPU*) realloc (*arr, (2 * position + 1) * sizeof(CPU));
        *size = 2 * position;
    }

    if (*arr == NULL)
        return -1;

    (*arr)[position] = elem;
    return 0;
}

static int AddProcArray (int** arr, size_t* size, int elem, size_t position) {

    if (position >= *size) {
        *arr = (int*) realloc (*arr, (2 * position + 1) * sizeof(int));
        *size = 2 * position;
    }

    if (*arr == NULL)
        return -1;

    (*arr)[position] = elem;
    return 0;
}

InfoCPU GetInfoCPU() {

    FILE* cmd = popen("grep \"core id\" /proc/cpuinfo", "r");
    char buffer[14] = {0};
    int coreIDs[256];
    int nCoreIDs = 0;
    short numReadBytes = 13;

    while(fread(buffer, sizeof(char), numReadBytes, cmd) > 0) {
        coreIDs[nCoreIDs++] = (int) strtol (buffer + 10, NULL, 10);

        if (coreIDs[nCoreIDs - 1] == 9)
            numReadBytes++;
    }

    InfoCPU res = {
            .CPUs = (CPU*) calloc(nCoreIDs, sizeof(CPU)),
            .nCPU = nCoreIDs
    };


    if (res.CPUs == NULL)
        return res;

    size_t nCPU = 0;
    for (int i = 0; i < nCoreIDs; i++) {

        if (Find(coreIDs, i, coreIDs[i]) == -1) {
            // if we haven't met this core_id yet
            CPU CPU = {};
            if (AddCPUArray(&res.CPUs, &res.nCPU, CPU, nCPU) != 0)
                goto exit_error;


            if (AddProcArray(&res.CPUs[nCPU].processors,
                                    &res.CPUs[nCPU].capacity,
                                    i,
                                    res.CPUs[nCPU].nProc++) != 0) 
                goto exit_error;

            nCPU++;
        }
        else {
            // if we have already met such core_id
            if (AddProcArray(&res.CPUs[coreIDs[i]].processors,
                                    &res.CPUs[coreIDs[i]].capacity,
                                    i,
                                    res.CPUs[coreIDs[i]].nProc++) != 0)
                goto exit_error;
        }
    }
    goto exit_success;

exit_error:

    for (int j = 0; j < nCPU; ++j)
        free (res.CPUs[j].processors);

    free (res.CPUs);
    res.CPUs = NULL;

exit_success:

    res.nCPU = nCPU;
    fclose(cmd);
    return res;
}

int SetAttrs (InfoCPU infoCPU, pthread_attr_t* attrs, size_t size) {

    for (int i = 0; i < size; i++) {
        size_t num_cpu = i % infoCPU.nCPU;
        size_t num_proc =(i / infoCPU.nCPU) % infoCPU.CPUs[num_cpu].nProc;

        cpu_set_t  cpu_set;
        CPU_ZERO(&cpu_set);
        CPU_SET(infoCPU.CPUs[num_cpu].processors[num_proc], &cpu_set);

        if (pthread_attr_setaffinity_np(&attrs[i], sizeof(cpu_set_t), &cpu_set) != 0)
            return -1;
    }

    return 0;
}

InfoThread** BuildCacheAlignedInfoThread (size_t nThreads) {

    long sizeCacheLine = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
    long sizePage = sysconf (_SC_PAGESIZE);
    size_t sizeOneInfo = (sizeof (InfoThread) / sizeCacheLine + 1) * sizeCacheLine;

    char* alignedMemory = (char*) memalign(sizePage, nThreads * sizeOneInfo);
    if (alignedMemory == NULL)
        return NULL;

    InfoThread** infosp = (InfoThread**) calloc(nThreads, sizeof(InfoThread*));
    if (infosp == NULL) {
        free(alignedMemory);
        return NULL;
    }

    for (int i = 0; i < nThreads; i++)
        infosp[i] = (InfoThread*) (alignedMemory + i * sizeOneInfo);

    return infosp;
}

int FillInfoThread (InfoThread** infosp, size_t info_size,
                       InfoThread init, size_t nThreads) {

    if (infosp == NULL || nThreads == 0)
        return -1;

    const double interval = (init.end - init.begin) / (double) nThreads;

    double th_begin = init.begin;
    double th_end   = init.begin + interval;

    const double c_begin = th_begin;
    const double c_end = th_end;

    double (*Function)(double) = init.Function;

    for (int i = 0; i < nThreads - 1; i++) {

        infosp[i]->begin    = th_begin;
        infosp[i]->end      = th_end;
        infosp[i]->Function = Function;
        th_begin = th_end;
        th_end  += interval;
    }

    infosp[nThreads - 1]->begin    = th_begin;
    infosp[nThreads - 1]->end      = init.end;
    infosp[nThreads - 1]->Function = Function;

    //for threads, which must work not for result
    for (size_t i = nThreads; i < info_size; i++) {
        infosp[i]->begin    = c_begin;
        infosp[i]->end      = c_end;
        infosp[i]->Function = Function;
    }

    return 0;
}