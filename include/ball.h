#ifndef BALL_H
#define BALL_H

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
 * @brief 단일 공(Ball) 객체를 나타내는 구조체
 */
typedef struct {
    int id;           ///< 고유 식별자 (예: 공 번호)
    int x, y;         ///< 현재 중심 위치 좌표 (픽셀 단위)
    int dx, dy;       ///< 프레임당 이동 방향 및 속도
    int radius;       ///< 공의 반지름 (픽셀 단위)
    RGBColor color;   ///< 공의 색상 (RGBColor 구조체 사용)
} BallObject;


// @brief Ball을 생성하는 함수
// @param id Ball 고유 ID
// @param x 초기 x 위치
// @param y 초기 y 위치
// @param dx 이동 방향 x
// @param dy 이동 방향 y
// @param radius 반지름
// @param color 색상
BallObject createBall(int id, int maxX, int maxY, int radius);

// @brief Ball을 이동시키는 함수
// @param b 이동시킬 Ball (참조)
// @param maxX 화면의 최대 너비
// @param maxY 화면의 최대 높이

void moveBall(BallObject *b, int maxX, int maxY);


#endif // BALL_H
