#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>


typedef struct {
    int csock;
    struct sockaddr_in cliaddr;
    int running;
    pthread_mutex_t mutex;
} ClientArg;

void* client_handler(void* arg);

#endif