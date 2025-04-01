#ifndef BALL_H
#define BALL_H

#include"common.h"

// @brief Ball 객체를 정의하는 구조체
typedef struct ball
{
    int id;         ///< 고유 식별자
    int x, y;       ///< 현재 위치 좌표
    int dx, dy;     ///< 이동 방향 및 속도
    int radius;     ///< 반지름
    int color;      ///< 프레임버퍼 출력용 RGB565 색상
}Ball;

// @brief Ball을 생성하는 함수
// @param id Ball 고유 ID
// @param x 초기 x 위치
// @param y 초기 y 위치
// @param dx 이동 방향 x
// @param dy 이동 방향 y
// @param radius 반지름
// @param color 색상
Ball createBall(int id, int x, int y, int dx, int dy, int radius, int color);

// @brief Ball을 이동시키는 함수
// @param b 이동시킬 Ball (참조)
// @param maxX 화면의 최대 너비
// @param maxY 화면의 최대 높이
void moveBall(Ball *b, int maxX, int maxY);

#endif // BALL_H
