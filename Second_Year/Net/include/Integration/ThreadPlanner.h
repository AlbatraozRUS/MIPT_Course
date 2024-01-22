#pragma once

#define _GNU_SOURCE
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sched.h>
#include <sys/sysinfo.h>
#include <malloc.h>

typedef struct CPU {
    int* processors;
    size_t nProc;
    size_t capacity;
} CPU;

typedef struct InfoCPU {
    CPU* CPUs;
    size_t nCPU;
} InfoCPU;

typedef struct InfoThread {
    double begin;
    double end;
    double res;
    double (*Function) (double);
} InfoThread;

InfoCPU GetInfoCPU();
int SetAttrs (InfoCPU cpuInfo, pthread_attr_t* attrs, size_t size);
InfoThread** BuildCacheAlignedInfoThread (size_t nThreads);
int FillInfoThread (InfoThread** infosp, size_t info_size, InfoThread init, size_t nThreads);
