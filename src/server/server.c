#include "server.h"

void* client_handler(void* arg) {
    ClientArg* carg = (ClientArg*)arg;
    char buf[BUFSIZ];
    int n;

    printf("[Server] Client connected from IP: %s\n", inet_ntoa(carg->cliaddr.sin_addr));

    while (1) {
        n = recv(carg->csock, buf, BUFSIZ - 1, 0);
        if (n <= 0)
            break;

        buf[n] = '\0';
        printf("[Server: %s] Received message: %s\n", inet_ntoa(carg->cliaddr.sin_addr), buf);
        fflush(stdout);
        
        if (strncmp(buf, "q", 1) == 0) {
            pthread_mutex_lock(&carg->mutex);
            carg->running = 0;
            pthread_mutex_unlock(&carg->mutex);
            break;
        }

        send(carg->csock, buf, n, 0);
    }

    printf("[Server] Connection with %s has been closed\n", inet_ntoa(carg->cliaddr.sin_addr));

    shutdown(carg->csock, SHUT_RDWR);
    close(carg->csock);

    pthread_mutex_destroy(&carg->mutex);
    free(carg);

    printf("  [Server] Thread terminated\n");


    pthread_exit(NULL);
}
