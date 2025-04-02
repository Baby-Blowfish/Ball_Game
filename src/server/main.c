#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "common.h"

int main(int argc, char**argv)
{
    int ssock, csock;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clen = sizeof(cliaddr);

    ssock = socket(AF_INET, SOCK_STREAM, 0);
    if (ssock < 0) {
        perror("socket()");
        exit(1);
    }

    // 2. 포트 재사용 설정
    int optval = 1;
    setsockopt(ssock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // 2. 서버 주소 구조체 초기화 및 설정
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;                 // IPv4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  // 모든 IP에서의 접속 허용
    servaddr.sin_port = htons(SERVER_PORT);           // 포트 번호

    // 3. IP/포트 바인딩
    if(bind(ssock, (struct sockaddr *)&servaddr, sizeof(servaddr))<0)
    {
        perror("bind() :");
        return -1;
    }

    // 4. 클라이언트 연결 대기 상태 진입
    if((listen(ssock, 8) < 0))
    {
        perror("listen() : ");
        return -1;
    }

    clen = sizeof(cliaddr);

    printf("[Server] Waiting for a client to connect...\n");

    while (1) {
        csock = accept(ssock, (struct sockaddr*)&cliaddr, &clen);
        if (csock < 0) {
            perror("accept()");
            continue;
        }

        ClientArg* carg = (ClientArg*)malloc(sizeof(ClientArg));
        if (!carg) {
            perror("malloc()");
            close(csock);
            continue;
        }

        carg->csock = csock;
        carg->cliaddr = cliaddr;
        carg->running = 1;
        pthread_mutex_init(&carg->mutex, NULL);

        pthread_t tid;
        if (pthread_create(&tid, NULL, client_handler, (void*)carg) != 0) {
            perror("pthread_create()");
            close(csock);
            pthread_mutex_destroy(&carg->mutex);
            free(carg);
        } else {
            pthread_detach(tid);
        }
    }

    printf("  [Server] Main function terminated\n");

    close(ssock); // 서버 소켓 종료
    return 0;
}

