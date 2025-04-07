#ifndef CLIENT_BALL_MANAGER_H
#define CLIENT_BALL_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/epoll.h>

#include "localballmanager.h"
#include "client_list_manager.h"
#include "task.h"
#include "log.h"

#define SERVER_PORT 5100
#define NUM_WORKERS 4

//sig_atomic_t 원자적으로 읽기/쓰기 보장
extern volatile sig_atomic_t keep_running;
extern TaskQueue* global_task_queue;
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
    int epoll_fd;
} SharedContext;



// 서버
SharedContext* manager_init();
void manager_destroy(SharedContext* arg);

char parseCommand(const char* cmdStr, int* ball_count, int* radius);

// 공 리스트를 문자열로 직렬화하여 모든 클라이언트에 전송
void broadcast_ball_state(ClientListManager* client_mgr, BallListManager* ball_mgr);

void log_client_connect(int fd, struct sockaddr_in* cliaddr);

void log_client_disconnect(int fd, const char* reason);

void log_ball_memory_usage(const char* action, int fd, int count);

void* worker_thread(void* arg);

void* cycle_broadcast_ball_state(void* arg);



#endif