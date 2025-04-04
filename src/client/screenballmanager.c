#include "screenballmanager.h"

void drawBall(dev_fb* fb, ScreenBall* b) {
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


void ball_manager_init(BallListManager* manager) {

    memset(manager, 0,sizeof(BallListManager));
    manager->head = NULL;
    manager->tail = NULL;
    pthread_mutex_init(&manager->mutex_ball, NULL);
}

void ball_manager_destroy(BallListManager* manager) {
    pthread_mutex_destroy(&manager->mutex_ball);
    printf( COLOR_GREEN "Mutex 'mutex_ball' has been destroyed." COLOR_RESET);
    freeBallList(&manager->head);
}

void add_ball(BallListManager* manager, int count, int width, int height, int radius) {
    for (int i = 0; i < count; i++) {
        ScreenBall b = create_screen_ball(manager->total_count++, width, height, radius);
        manager->head = appendBall(manager->head, &manager->tail, b);
    }
}

void delete_ball(BallListManager* manager, int count) {
    for (int i = 0; i < count; i++) {
        if (deleteLastBall(&manager->head, &manager->tail) != NULL)
            manager->total_count--;
        else printf(COLOR_BLUE  "There are no balls." COLOR_RESET);
    }
}

void move_all_ball(dev_fb* fb, BallListManager* manager) {
    moveBallList(manager->head, fb->vinfo.xres, fb->vinfo.yres);
}

BallListNode* deserialize_ball_list(const char* str)
{
    
}