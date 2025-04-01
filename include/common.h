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

#include "fbDraw.h"  // pixel 사용
#include "ball.h"    // Ball 구조체 사용

#define CLIENT_NUM 1

typedef unsigned char uchar;


/// @brief 속도 방향 구조체
typedef struct {
    int dx; // -1, 1
    int dy; // -1, 1
} Speed;

/// @brief 클라이언트 → 서버 명령 패킷
typedef struct {
    char cmd[100];     // "a:3", "d:1", "s:fast" 등
    int opt_num;       // 옵션 값
    int client_num;    // 클라이언트 번호
} packet;

/// @brief 서버 → 클라이언트 공 동기화 패킷
typedef struct {
    int pkt_type;     // 0: size, 1: data
    int list_size;
    Ball ball;
} sync_packet;

#endif // COMMON_H
