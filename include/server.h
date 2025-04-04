#ifndef CLIENT_BALL_MANAGER_H
#define CLIENT_BALL_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "server_ball_manager.h"
#include "client_list_manager.h"
#include "task.h"


/**
 * @brief 게임 전체에서 공유되는 전역 상태 컨텍스트 구조체
 *
 * 여러 스레드에서 공통으로 접근하는 자원들(공 리스트, 클라이언트 리스트 등)을 포인터로 포함하고 있음.
 * 스레드들은 이 구조체 하나를 인자로 받아 각각 필요한 자원에 접근한다.
 */
typedef struct {
    BallListManager* ball_list_manager;     ///< 공 리스트 관리자
    ClientListManager* client_list_manager; ///< 클라이언트 리스트 관리자
    TaskQueue *task_queue;
} SharedContext;



// 서버
SharedContext* manager_init();
void manager_destroy(SharedContext* arg);

char parseCommand(const char* cmdStr, int* ball_count, int* width, int* height, int* radius);

void* worker_thread(void* arg);

#endif