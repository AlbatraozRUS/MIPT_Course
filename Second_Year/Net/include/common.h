#pragma once

#include <stdio.h>

#define UDP_PORT 6666
#define TCP_PORT 8888

enum error {
    SUCCESS = 0,
    E_BADARGS,
    E_SOCKET,
    E_CONNECTION,
    E_MEMORY,
    E_THREAD,
};

typedef struct InfoCommon {
    int server;
    size_t nThreads;
    size_t nComputers;
} InfoCommon;

void RunServer(size_t n_threads, size_t nComputers);
void RunClient ();
int setKeepAlive (int socketID, int keepcnt, int keepidle, int keepintvl);
void p_error(enum error err);