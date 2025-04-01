#include "ball.h"


Ball createBall(int id, int maxX, int maxY, int radius) {
    
    Ball b;
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

void drawBall(dev_fb* fb, Ball* b) {
    pixel center = fb_toPixel(b->x, b->y);
    fb_drawFilledCircle(fb, center, b->color.r, b->color.g, b->color.b);
}


void moveBall(Ball* b, int maxX, int maxY) {
    b->x += b->dx;
    b->y += b->dy;

    if (b->x <= 0 || b->x >= (maxX - b->radius))
        b->dx *= -1;
    if (b->y <= 0 || b->y >= (maxY - b->radius))
        b->dy *= -1;
}

