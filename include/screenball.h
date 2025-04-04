#ifndef SCREEN_BALL_H
#define SCREEN_BALL_H

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
#include "console_color.h"

/**
 * @brief RGB 색상 정보를 나타내는 구조체
 */
typedef struct {
    unsigned char r;  ///< Red 채널 (0 ~ 255)
    unsigned char g;  ///< Green 채널 (0 ~ 255)
    unsigned char b;  ///< Blue 채널 (0 ~ 255)
} RGBColor;

/**
 * @brief 클라이언트 해상도 기준의 화면 출력용 공 구조체
 * 서버에서 받은 BallObject를 픽셀 단위로 변환한 결과
 */
typedef struct {
    int id;
    int x, y;           ///< 실제 화면상 좌표 (픽셀)
    int dx, dy;         ///< 화면 속도 
    int radius;         ///< 반지름 (픽셀 기준)
    RGBColor color;
} ScreenBall;

// @brief Ball을 생성하는 함수
// @param id Ball 고유 ID
// @param radius 반지름
ScreenBall create_screen_ball(int id, int width, int height, int radius);

// @brief Ball을 이동시키는 함수 (논리좌표계 기준)
// @param b 이동시킬 Ball (참조)
void move_screen_ball(ScreenBall *b, int width, int height);
#endif // SCREEN_BALL_H
