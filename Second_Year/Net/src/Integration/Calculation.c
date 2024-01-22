#include "common.h"
#include "Integration/Calculation.h"

const double step = 1e-7;

typedef double (*func) (double);

double CalcIntegral (double begin, double end, func function) {

    double res = 0;
    for (double x = begin; x < end; x += step)
        res += function(x) * step;

    return res;
}

void* StartCalculation (void* arg) {

    InfoThread* params = arg;
    params->res = CalcIntegral(params->begin, params->end, params->Function);
    return NULL;
}

int Integrate (size_t nThreads, double begin, double end, func function, double *const result) {

    int error = SUCCESS;

    if (nThreads == 0 || function == NULL || result == NULL)
        return E_BADARGS;

    size_t nCPUs = (size_t) get_nprocs();
    size_t nThreadCreate = nThreads > nCPUs ? nThreads : nCPUs;

    InfoCPU cpuInfo = GetInfoCPU();
    if (cpuInfo.CPUs == NULL) {
        perror("get_cpu_info");
        return E_MEMORY;
    }

    pthread_t *threadIDs = (pthread_t*) calloc(nThreadCreate, sizeof(pthread_t));
    if (threadIDs == NULL) {
        perror("threadIDs allocation");
        error = E_MEMORY;
        goto exit_threads;
    }

    InfoThread** infosp = BuildCacheAlignedInfoThread(nThreadCreate);

    if (infosp == NULL) {
        perror("infosp allocation");
        error = E_MEMORY;
        goto exit_infosp;
    }

    InfoThread init = {
            .begin = begin,
            .end = end,
            .Function = function
    };

    if (FillInfoThread(infosp, nThreadCreate, init, nThreads) != 0) {
        error = E_BADARGS;
        goto exit_infosp;
    }

    pthread_attr_t* attrs = (pthread_attr_t*) calloc(nThreadCreate, sizeof(pthread_attr_t));

    if (attrs == NULL) {
        perror("attrs allocation");
        error = E_MEMORY;
        goto exit_attrs;
    }

    for (int i = 0; i < nThreadCreate; i++) {

        if (pthread_attr_init(attrs + i) != 0){
            perror("pthread_attr_init");
            error = E_THREAD;
            goto exit_attrs;
        }
    }

    if (nThreads <= nThreadCreate) {
        if (SetAttrs(cpuInfo, attrs, nThreadCreate) != 0) {
            perror("SetAttrs");
            error = E_THREAD;
            goto exit_all;
        }
    }

    for (int i = 0; i < nThreadCreate; i++) {

        if (pthread_create(&threadIDs[i], &attrs[i], StartCalculation, (void*) infosp[i]) != 0) {
            perror("pthread_create");
            error = E_THREAD;
            goto exit_all;
        }
    }

    double res = 0;
    for (int i = 0; i < nThreads; i++) {

        if (pthread_join(threadIDs[i], NULL) != 0){
            perror("pthread_join calc:");
            error = E_THREAD;
            goto exit_all;
        }

        res += infosp[i]->res;
    }

    *result = res;

exit_all:
    for (int i = 0; i < nThreadCreate; i++)
        pthread_attr_destroy(&attrs[i]);

    free(attrs);

exit_attrs:
    free(infosp[0]);
    free(infosp);

exit_infosp:
    free(threadIDs);

exit_threads:
    for (int i = 0; i < cpuInfo.nCPU; i++)
        free(cpuInfo.CPUs[i].processors);

    free(cpuInfo.CPUs);

    return error;
}
