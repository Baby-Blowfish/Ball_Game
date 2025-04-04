#ifndef SCREEN_BALL_MANAGER_H
#define SCREEN_BALL_MANAGER_H

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

#include "fbDraw.h"
#include "screenball_list.h"


#define FILE_NAME "data.bin"

// 명령어 정의
#define CMD_ADD 'a'
#define CMD_DEL 'd'
#define CMD_SPEED_UP 'w'
#define CMD_SPEED_DOWN 's'
#define CMD_EXIT 'x'


typedef unsigned char uchar;


typedef struct {
    BallListNode* head;  ///< 공 리스트의 시작 노드
    BallListNode* tail;  ///< 공 리스트의 마지막 노드
    pthread_mutex_t mutex_ball; ///< 공 리스트 전체에 대한 동기화를 위한 뮤텍스
    int total_count;     ///< 현재 리스트에 존재하는 공의 총 개수
} BallListManager;


void drawBall(dev_fb* fb, ScreenBall* b);

void drawBallList(dev_fb* fb, BallListNode* head);

void ball_manager_init(BallListManager* manager);

void ball_manager_destroy(BallListManager* manager);

void add_ball(BallListManager* manager, int count, int width, int height,int radius);

void delete_ball(BallListManager* manager, int count);

void move_all_ball(dev_fb* fb, BallListManager* manager);

BallListNode* deserialize_ball_list(const char* str);


#endif // SCREEN_BALL_MANAGER_H
