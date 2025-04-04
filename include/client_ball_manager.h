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
#define RADIUS 20
#define START_BALL_COUNT 5

typedef unsigned char uchar;


/**
 * @brief 공 리스트 전체를 관리하는 매니저 구조체
 */
typedef struct {
    BallListNode* head;  ///< 공 리스트의 시작 노드
    BallListNode* tail;  ///< 공 리스트의 마지막 노드
    int flag_add_remove;
    int flag_speed_change;
    pthread_mutex_t mutex_ball; ///< 공 리스트 전체에 대한 동기화를 위한 뮤텍스
    int total_count;     ///< 현재 리스트에 존재하는 공의 총 개수
} BallListManager;



void drawBall(dev_fb* fb, BallObject* b);

void drawBallList(dev_fb* fb, BallListNode* head);


void move_all(dev_fb* fb, BallListManager* manager);

void server_ball_manager_init(BallListManager* manager);

void server_ball_manager_destroy(BallListManager* manager);

void add_ball(BallListManager* manager, int count, int width, int height,int radius);

void delete_ball(BallListManager* manager, int count);

BallListNode* deserialize_ball_list(const char* str);


#endif // COMMON_H
