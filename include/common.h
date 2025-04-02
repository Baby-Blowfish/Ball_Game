#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

#include "console_color.h"
#include "fbDraw.h"
#include "ball_list.h"
#include "server.h"
#include "client.h"

#define FILE_NAME "data.bin"

// 명령어 정의
#define CMD_ADD 'a'
#define CMD_DEL 'd'
#define CMD_SPEED_UP 'w'
#define CMD_SPEED_DOWN 's'
#define CMD_EXIT 'x'

#define SERVER_PORT 5100
#define MAX_CLIENTS 10
#define MAX_INPUT 100

typedef unsigned char uchar;


/**
 * @brief 공 리스트 전체를 관리하는 매니저 구조체
 */
typedef struct {
    BallListNode* head;  ///< 공 리스트의 시작 노드
    BallListNode* tail;  ///< 공 리스트의 마지막 노드
    int total_count;     ///< 현재 리스트에 존재하는 공의 총 개수

    int flag_add_remove;   ///< 공의 개수 증감 플래그 (0: 유지, 1: 증가, -1: 감소)
    int flag_speed_change; ///< 공의 속도 증감 플래그 (0: 유지, 1: 증가, -1: 감소)

    pthread_mutex_t mutex_ball_list; ///< 공 리스트 전체에 대한 동기화를 위한 뮤텍스
} BallListManager;


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

void drawBall(dev_fb* fb, BallObject* b);
void drawBallList(dev_fb* fb, BallListNode* head);
char parseCommand(const char* cmdStr, int* ball_count);

// 클라이언트 ->  서버 송신 스레드 : 입력  명령 받기 및 전송
void* socket_send_thread(void* arg);

//  B
GameSharedContext* ball_server_arg_init();
void ball_server_arg_destroy(GameSharedContext* arg);

#endif // COMMON_H
