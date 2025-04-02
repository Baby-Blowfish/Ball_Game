#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

/**
 * @brief 소켓 통신 상태를 관리하는 구조체
 */
typedef struct {
    int socket_fd;                 ///< 연결된 서버 소켓의 파일 디스크립터
    int is_running;               ///< 실행 상태 플래그 (1: 실행 중, 0: 종료 요청됨)
    pthread_mutex_t mutex_socket; ///< 소켓과 실행 상태 보호를 위한 뮤텍스
} SocketContext;

void* socket_recv_echo_thread(void* arg);
void* socket_send_echo_thread(void* arg);

#endif