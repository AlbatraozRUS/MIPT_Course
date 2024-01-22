#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>


struct sockaddr_in
GetHelloMessage (int *error);

int ConnectServer (struct sockaddr_in servAddr, int* error);

int Compute (int socketID);
