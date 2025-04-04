#include "client_ball_manager.h"

void drawBall(dev_fb* fb, BallObject* b) {
    pixel center = fb_toPixel(b->x, b->y);
    fb_drawFilledCircle(fb, center, b->color.r, b->color.g, b->color.b);
}

void drawBallList(dev_fb* fb, BallListNode* head) {
    BallListNode* cur = head;
    while (cur != NULL) {
        drawBall(fb,&(cur->data));
        cur = cur->next;
    }
}


void client_ball_manager_init(BallListManager* manager, int count, int width, int height) {

    memset(manager, 0,sizeof(BallListManager));
    manager->head = NULL;
    manager->tail = NULL;
    manager->total_count = 0;
    manager->flag_add_remove = 0;
    manager->flag_speed_change = 0;
    pthread_mutex_init(&manager->mutex_ball, NULL);

    // 테스트용 기본 공 count개 추가 가능
    client_add_ball(manager, count, width, height, RADIUS);
}

void client_ball_manager_destroy(BallListManager* manager) {
    pthread_mutex_destroy(&manager->mutex_ball);
    freeBallList(&manager->head);
}

void client_add_ball(BallListManager* manager, int count, int width, int height, int radius) {
    for (int i = 0; i < count; i++) {
        BallObject b = createBall(manager->total_count++, width, height, radius);
        manager->head = appendBall(manager->head, &manager->tail, b);
    }
}

void client_delete_ball(BallListManager* manager, int count) {
    for (int i = 0; i < count; i++) {
        if (deleteLastBall(&manager->head, &manager->tail) != NULL)
            manager->total_count--;
        else printf("There are no balls.\n");
    }
}

void move_all(dev_fb* fb, BallListManager* manager) {
    moveBallList(manager->head, fb->vinfo.xres, fb->vinfo.yres);
}

char* serialize_ball_list(BallListManager* manager) {
    BallListNode* cur = manager->head;
    char* buffer = (char*)malloc(8192);  // 공이 많아질 경우 대비
    buffer[0] = '\0';

    while (cur) {
        char temp[256];
        snprintf(temp, sizeof(temp), "%d,%d,%d,%d,%d,%d,%d,%d,%d|",
                 cur->data.id,
                 cur->data.x, cur->data.y,
                 cur->data.dx, cur->data.dy,
                 cur->data.radius,
                 cur->data.color.r, cur->data.color.g, cur->data.color.b);
        strcat(buffer, temp);
        cur = cur->next;
    }

    return buffer; // 호출자가 free 해야 함
}