#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "client_ball_manager.h"


#define SERVER_PORT 5100
#define MAX_CLIENTS 10
#define MAX_INPUT 100
#define RADIUS 20
#define START_BALL_COUNT 5

/**
 * @brief 소켓 통신 상태를 관리하는 구조체
 */
typedef struct {
    int socket_fd;                 ///< 연결된 서버 소켓의 파일 디스크립터
    int is_running;               ///< 실행 상태 플래그 (1: 실행 중, 0: 종료 요청됨)
    pthread_mutex_t mutex_socket; ///< 소켓과 실행 상태 보호를 위한 뮤텍스
} SocketContext;

// // 클라이언트 → 서버: 명령 전송용 패킷
// typedef struct {
//     char cmd;       // CMD_ADD, CMD_DELETE, CMD_SPEEDUP, ...
//     int count;      // 추가/삭제할 개수 (속도 조절 시는 0 또는 의미값)
//     Ball ball;      // 공 1개의 정보 (CMD_ADD 전용)
// } BallCommandPacket;

// // 서버 → 클라이언트: 전체 공 리스트 동기화 전송용 패킷
// typedef struct {
//     int count;      // 포함된 공 개수
//     Ball balls[100]; // 공 리스트 (최대 100개까지 전송)
// } BallSyncPacket;

/**
 * @brief 게임 전체에서 공유되는 전역 상태 컨텍스트 구조체
 *
 * 여러 스레드에서 공통으로 접근하는 자원들(프레임버퍼, 공 리스트, 서버 소켓 등)을 포인터로 포함하고 있음.
 * 스레드들은 이 구조체 하나를 인자로 받아 각각 필요한 자원에 접근한다.
 */
typedef struct {
    dev_fb* framebuffer;             ///< 프레임버퍼 디바이스 정보 구조체 (디스플레이 제어용)
    BallListManager* ball_list_manager;     ///< 공 리스트 관리자
    SocketContext* socket_ctx;      ///< 소켓 통신 상태 관리자
} GameSharedContext;

char parseCommand(const char* cmdStr, int* ball_count);

// 클라이언트 ->  서버 송신 스레드 : 입력  명령 받기 및 전송
void* socket_send_thread(void* arg);

// 서버
GameSharedContext* ball_client_arg_init();
void ball_client_arg_destroy(GameSharedContext* arg);

void* socket_recv_echo_thread(void* arg);
void* socket_send_echo_thread(void* arg);

void parse_and_draw_balls(const char* recv_buf, dev_fb* fb);
#endif