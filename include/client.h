#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include "screenballmanager.h"


#define SERVER_PORT 5100
#define MAX_CLIENTS 10
#define MAX_INPUT 100
#define RADIUS 20
#define START_BALL_RADIUS 20

extern volatile sig_atomic_t keep_running;

/**
 * @brief 게임 전체에서 공유되는 전역 상태 컨텍스트 구조체
 *
 * 여러 스레드에서 공통으로 접근하는 자원들(프레임버퍼, 공 리스트, 서버 소켓 등)을 포인터로 포함하고 있음.
 * 스레드들은 이 구조체 하나를 인자로 받아 각각 필요한 자원에 접근한다.
 */
typedef struct {
    int socket_fd;
    dev_fb* framebuffer;             ///< 프레임버퍼 디바이스 정보 구조체 (디스플레이 제어용)
    BallListManager* ball_list_manager;     ///< 공 리스트 관리자
    pthread_mutex_t mutex_ball;
} SharedContext;

// 서버
SharedContext* manager_init();
void manager_destroy(SharedContext* arg);

// 렌더링 스레드
void* render_thread(void* arg);

// 서버 -> 클라이언트 수신용 스래드
void* socket_recv_thread(void* arg);
void* socket_send_thread(void* arg);

#endif