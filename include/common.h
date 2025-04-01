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

#define FILE_NAME "data.bin"

#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_CYAN    "\033[36m"

// 명령어 정의
#define CMD_ADD 'a'
#define CMD_DEL 'd'
#define CMD_SPEED_UP 'w'
#define CMD_SPEED_DOWN 's'
#define CMD_EXIT 'x'

#define CLIENT_NUM 1

typedef unsigned char uchar;



// /// @brief 클라이언트 → 서버 명령 패킷
// typedef struct {
//     char cmd[100];     // "a:3", "d:1", "s:fast" 등
//     int opt_num;       // 옵션 값
//     int client_num;    // 클라이언트 번호
// } packet;

// /// @brief 서버 → 클라이언트 공 동기화 패킷
// typedef struct {
//     int pkt_type;     // 0: size, 1: data
//     int list_size;
//     Ball ball;
// } sync_packet;

#endif // COMMON_H
