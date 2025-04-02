#include "ball.h"


BallObject createBall(int id, int maxX, int maxY, int radius) {
    
    BallObject b;
    b.id = id;

    // 경계를 벗어나지 않는 범위 내에서 랜덤 위치 생성
    b.x = rand() % (maxX - 2 * radius) + radius;
    b.y = rand() % (maxY - 2 * radius) + radius;

    b.dx = (rand() % 2 == 0) ? -1 : 1;
    b.dy = (rand() % 2 == 0) ? -1 : 1;

    b.color.r = rand() % 256;
    b.color.g = rand() % 256;
    b.color.b = rand() % 256;

    b.radius = radius;

    return b;
}


void moveBall(BallObject* b, int maxX, int maxY) {
    b->x += b->dx;
    b->y += b->dy;

    // 좌우 벽에 닿으면 방향 반전
    if (b->x <= b->radius || b->x >= (maxX - b->radius)) {
        b->dx *= -1;
        b->x += b->dx; // 튕긴 뒤 한 칸 이동해줌
    }

    // 상하 벽에 닿으면 방향 반전
    if (b->y <= b->radius || b->y >= (maxY - b->radius)) {
        b->dy *= -1;
        b->y += b->dy;
    }
}

