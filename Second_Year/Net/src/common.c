#include "common.h"
#include "Network/Server.h"
#include "Network/Worker.h"

const double begin = 0.0;
const double end   = 100.0;

static void* SendHello (void* arg) {

    int error = SUCCESS;
    while (1) {
        error = SendHelloMessage();

        if (error != SUCCESS) {
            p_error (error);
            pthread_exit (NULL);
        }
    }
}

static void* GetTCPs (void* arg) {

    TasksWorkers* tasks = arg;

    int error = GetConnectionsTCP(tasks);

    if (error != SUCCESS) {
        p_error (error);
        fprintf (stderr, "Failed connection\n");
    }

    return NULL;
}

void RunServer(size_t nThreads, size_t nComputers) {

    int error = SUCCESS;

    TasksWorkers* tasks = DivideWork(nComputers, nThreads, begin, end, &error);
    if (error != SUCCESS) {
        p_error (error);
        fprintf (stderr, "Failed\n");
        return;
    }

    pthread_t thSender, thGetter;

    if (pthread_create(&thGetter, NULL, GetTCPs, tasks) != 0) {
        perror ("pthread_create getter:");
        return;
    }

    if (pthread_create(&thSender, NULL, SendHello, NULL) != 0) {
        perror ("pthread_create sender:");
        return;
    }

    if (pthread_join(thGetter, NULL) != 0) {
        perror ("pthread_join getter:");
        goto exit;
    }

    pthread_cancel(thSender);

    double res = 0;
    if (GetResult(tasks, &res) != SUCCESS){
        p_error (error);
        fprintf (stderr, "Failed\n");
        goto exit;
    }

    printf ("Result: %f\n", res);

exit:
    for (int i = 0; i < tasks->size; i++)
        close (tasks->task[i].socket);
    
}

void RunClient() {

    int error = SUCCESS;

    struct sockaddr_in AddrServer = GetHelloMessage(&error);

    if (error != SUCCESS) {
        p_error (error);
        return;
    }

    int socketServer = ConnectServer(AddrServer, &error);

    if (error != SUCCESS) {
        p_error (error);
        close (socketServer);
        return;
    }

    error = Compute(socketServer);

    if (error != SUCCESS) {
        close (socketServer);
        p_error (error);
        return;
    }

    printf ("[SUCCESS:Finished]\n");

    close (socketServer);
}

int setKeepAlive (int socketID, int keepcnt, int keepidle, int keepintvl) {

    int ret;
    int true = 1;

    if (setsockopt (socketID, SOL_SOCKET, SO_KEEPALIVE, &true, sizeof true) != 0){
        perror ("setsockopt (timeout) for server socket:");
        return -1;
    }

    if (setsockopt(socketID, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(int)) != 0){
        perror ("tcp_keepcnt");
        return -1;
    }

    if (setsockopt (socketID, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(int)) != 0){
        perror ("tcp_keepcnt");
        return -1;
    }

    if (setsockopt(socketID, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(int)) != 0){
        perror ("tcp_keepcnt");
        return -1;
    }

    return 0;
}

void p_error (enum error err) {

    switch (err) {

        case SUCCESS:
            fprintf (stderr, "SUCCESS\n");
            break;
        case E_BADARGS:
            fprintf (stderr, "Bad arguments");
            break;
        case E_SOCKET:
            fprintf (stderr, "Error with socket\n");
            break;
        case E_CONNECTION:
            fprintf (stderr, "Error while connecting\n");
            break;
        case E_MEMORY:
            fprintf (stderr, "Error allocating memory\n");
            break;
        case E_THREAD:
            fprintf (stderr, "Error with thread\n");
            break;
        default:
            fprintf (stderr, "Unknown error\n");
    }
}