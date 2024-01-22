#pragma once

#define _GNU_SOURCE
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/sysinfo.h>
#include <sys/select.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

typedef struct InfoWorker {
    double begin;
    double end;
    size_t nThreads;
} InfoWorker;

typedef struct InfoHandler {
    InfoWorker infoWorker;
    int socket;
} InfoHandler;

typedef struct TasksWorkers {
    InfoHandler* task;
    size_t size;
} TasksWorkers;

void* HandlerWork (void*);

TasksWorkers*
DivideWork (unsigned nComputers, unsigned nThreads,
             double begin, double end, int* error);

void DumpTasks (TasksWorkers* tasks);

int SendHelloMessage();

int GetConnectionsTCP (TasksWorkers* tasks);

int GetResult (TasksWorkers *tasks, double *res);
