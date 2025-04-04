#include "screenball.h"

ScreenBall create_screen_ball(int id, int width, int height, int radius) {
    
    ScreenBall b;
    b.id = id;
    b.radius = radius;

    // 경계를 벗어나지 않는 범위 내에서 랜덤 위치 생성
    b.x = rand() % (width - 2 * radius) + radius;
    b.y = rand() % (height - 2 * radius) + radius;

    b.dx = (rand() % 2 == 0) ? -1 : 1;
    b.dy = (rand() % 2 == 0) ? -1 : 1;

    b.color.r = rand() % 256;
    b.color.g = rand() % 256;
    b.color.b = rand() % 256;

    return b;
}


void move_screen_ball(ScreenBall* b, int width, int height) {
    b->x += b->dx;
    b->y += b->dy;

    // 좌우 벽에 닿으면 방향 반전
    if (b->x <= b->radius || b->x >= (width - b->radius)) {
        b->dx *= -1;
        b->x += b->dx; // 튕긴 뒤 한 칸 이동해줌
    }

    // 상하 벽에 닿으면 방향 반전
    if (b->y <= b->radius || b->y >= (height - b->radius)) {
        b->dy *= -1;
        b->y += b->dy;
    }
}

