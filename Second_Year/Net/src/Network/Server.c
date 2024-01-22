#include "common.h"
#include "Network/Server.h"

static int CreateServerSocket (int* serverSocket);

void* HandlerWork (void* arg) {

    double* result = (double*) calloc (1, sizeof (double));

    if (result == NULL) {
        goto exit_without_free;
    }

    InfoHandler* infoHandler = arg;

    if (send (infoHandler->socket, 
              &infoHandler->infoWorker, 
              sizeof infoHandler->infoWorker,
               0) != sizeof infoHandler->infoWorker)
        goto exit_free_res;

    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(infoHandler->socket, &fdset);

    if (select (infoHandler->socket + 1, &fdset, NULL, NULL, NULL) != 1)
        goto exit_free_res;

    if (recv (infoHandler->socket, result, sizeof (double), 0) != sizeof (double))
        goto exit_free_res;

    goto exit_without_free;

exit_free_res:

    free (result);
    result = NULL;

exit_without_free:

    return result;
}

TasksWorkers *
DivideWork (unsigned nComputers, unsigned nThreads,
             double begin, double end, int *error) {

    int err = SUCCESS;
    if (nComputers == 0 || nThreads == 0) {
        err = E_BADARGS;
        goto exit_without_free;
    }

    const unsigned nCPUs = get_nprocs ();
    const double intervalThread = (end - begin) / nThreads;

    unsigned tmp = nThreads / nCPUs;
    unsigned nWorkers = nThreads % nCPUs == 0 ? tmp : tmp + 1;
    nWorkers = nComputers < nThreads ? nComputers : nThreads;

    TasksWorkers*
            result = (TasksWorkers*) calloc (1, sizeof (TasksWorkers));

    if (!result) {
        err = E_MEMORY;
        goto exit_without_free;
    }

    result->size = nWorkers;
    result->task = (InfoHandler*) calloc(nWorkers, sizeof(InfoHandler));
    if (!result->task) {
        err = E_MEMORY;
        goto exit_free_res;
    }

    for (int i = 0; i < nWorkers - 1; i++) {

        result->task[i].infoWorker.nThreads = nThreads / nWorkers;
        result->task[i].infoWorker.begin = begin;

        begin += intervalThread * (double)result->task[i].infoWorker.nThreads;

        result->task[i].infoWorker.end = begin;
    }

    result->task[nWorkers - 1].infoWorker.nThreads = nThreads - (nWorkers - 1) *
                                                     result->task[0].infoWorker.nThreads;
    result->task[nWorkers - 1].infoWorker.begin = begin;
    result->task[nWorkers - 1].infoWorker.end = end;

    goto exit_without_free;

exit_free_res:
    free (result);
    result = NULL;

exit_without_free:
    if (error)
        *error = err;

    if (err != SUCCESS)
        result = NULL;

    return result;
}

void DumpTasks (TasksWorkers* tasks) {

    if (tasks == NULL) {
        fprintf (stderr, "tasks is NULL\n");
        return;
    }

    for (int i = 0; i < tasks->size; i++) {
        printf ("[%d]:\nNumber of Threads: %zu\nBegin: %f\nEnd: %f\n",
                i, tasks->task[i].infoWorker.nThreads,
                tasks->task[i].infoWorker.begin, tasks->task[i].infoWorker.end);
    }
}

int SendHelloMessage () {

    struct sockaddr_in bc_addr;

    int socketID = socket (AF_INET, SOCK_DGRAM, 0);
    if (socketID == -1)
        return E_SOCKET;

    int true = 1;
    if (setsockopt (socketID, SOL_SOCKET, SO_BROADCAST, &true, sizeof true) < 0) {
        close (socketID);
        return E_SOCKET;
    }

    memset (&bc_addr, 0, sizeof bc_addr);

    bc_addr.sin_family = AF_INET;
    bc_addr.sin_port = htons(UDP_PORT);
    bc_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    int msg = TCP_PORT;
    if (sendto (socketID, &msg, sizeof msg, 0, (struct sockaddr *) &bc_addr, sizeof bc_addr) < 0) {
        close (socketID);
        return E_SOCKET;
    }

    close (socketID);
    return SUCCESS;
}

int GetConnectionsTCP (TasksWorkers* tasks) {

    if (tasks == NULL)
        return E_BADARGS;

    int error = SUCCESS;

    int serverSocket;
    int newSocket;

    struct sockaddr_in newAddr;
    socklen_t newAddrLen = sizeof newAddr;

    error = CreateServerSocket (&serverSocket);

    if (error != SUCCESS)
        goto exit_without_closing;
    

    if (listen (serverSocket, 10) < 0) {
        error = E_SOCKET;
        goto exit_without_closing;
    }

    int nConnectedSockets = 0;
    for (; nConnectedSockets < tasks->size; nConnectedSockets++) {

        newSocket = accept (serverSocket, (struct sockaddr*) &newAddr, &newAddrLen);
        if (newSocket < 0) {
            error = E_CONNECTION;
            goto exit_close_sockets;
        }

        tasks->task[nConnectedSockets].socket = newSocket;
        printf("[Success: Server is connected]\n");
    }


    goto exit_without_closing;

exit_close_sockets:

    for (int j = 0; j < nConnectedSockets; ++j)
        close (tasks->task[j].socket);


exit_without_closing:

    close (serverSocket);
    return error;
}

int GetResult (TasksWorkers *tasks, double *res) {

    if (!tasks || !res)
        return E_BADARGS;

    int error = SUCCESS;

    pthread_t* threadIDs = (pthread_t*) calloc (tasks->size, sizeof (pthread_t));

    if (!threadIDs) {
        error = E_MEMORY;
        goto exit_without_free;
    }

    int i = 0;
    for (; i < tasks->size; i++) {

        if (pthread_create (&threadIDs[i], NULL, HandlerWork, (void *) &tasks->task[i]) != 0){
            perror ("pthread_create HandlerWork:");
            error = E_THREAD;
            goto exit_free_all;
        }
    }

    double result = 0;
    double *part = NULL;

    for (int j = 0; j < tasks->size; ++j) {

        int check = pthread_join (threadIDs[j], (void **) &part);

        if (check != 0 || !part) {

            if (check != 0)
                fprintf (stderr, "Output of pthread_join() = %d\n", check);
            error = E_THREAD;
            goto exit_free_all;
        }

        result += *part;
        free (part);
    }

    *res = result;

    goto exit_free_threads;

exit_free_all:
    for (int j = 0; j < i; ++j)
        pthread_cancel (threadIDs[j]);
    
exit_free_threads:
    free (threadIDs);

exit_without_free:
    return error;
}

static int CreateServerSocket (int* serverSocket) {

    int true = 1;
    int servSock = -1;

    struct sockaddr_in servAddr;

    servSock = socket(AF_INET, SOCK_STREAM, 0);
    if (servSock == -1) {
        perror ("creating server socket:");
        return E_SOCKET;
    }

    if (setsockopt (servSock, SOL_SOCKET, SO_REUSEADDR, &true, sizeof true) != 0){
        perror ("setsockopt for server socket:");
        return E_SOCKET;
    }

    struct timeval timeout = {
            .tv_sec = 10,
            .tv_usec = 0
    };

    if (setsockopt (servSock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) != 0){
        perror ("setsockopt (timeout) for server socket:");
        return E_SOCKET;
    }

    if (setKeepAlive (servSock, 4, 3, 1) != 0)
        return E_SOCKET;

    memset (&servAddr, 0, sizeof servAddr);

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl (INADDR_ANY);
    servAddr.sin_port = htons (TCP_PORT);

    if (bind (servSock, (struct sockaddr*) &servAddr, sizeof servAddr) < 0) {
        perror ("binding server socket");
        return E_SOCKET;
    }

    *serverSocket = servSock;
    return SUCCESS;
}
