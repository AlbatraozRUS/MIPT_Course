#include "common.h"
#include <stdlib.h>
#include <string.h>

InfoCommon ParseArgs (int args, char *argv[]);

int main (int argc, char *argv[]) {

    InfoCommon start = ParseArgs (argc, argv);

    switch (start.server) {
        case -1:
            fprintf(stderr, "Error in parsing args.\n");
            return 0;

        case 1:
            RunServer(start.nThreads, start.nComputers);
            break;
        
        case 0:
            RunClient();
            break;

        default:
            fprintf(stderr, "Error in parsing args: [%d]\n", start.server);
            return -1;
        
    }


    return 0;
}

InfoCommon ParseArgs (int argc, char *argv[]) {

    InfoCommon infoCommon = {0, 0, 0};

    char* flagServer = "-server";

    switch (argc) {
        case 1:
            break;

        case 3:
            fprintf(stderr, "Bad arguments");
            infoCommon.server = -1;
            break;

        case 4:
            if (strcmp(argv[1], flagServer) != 0) {
                infoCommon.server = -1;
                return infoCommon;
            }

            infoCommon.server = 1;
            infoCommon.nComputers = (size_t)strtol(argv[2], NULL, 10);
            infoCommon.nThreads = (size_t)strtol(argv[3], NULL, 10);
            break; 

        default:
            infoCommon.server = -1;
            break;
    }


    return infoCommon;
}
