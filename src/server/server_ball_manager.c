#include "server_ball_manager.h"

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

void server_ball_manager_init(BallListManager* manager) {

    memset(manager, 0,sizeof(BallListManager));
    manager->head = NULL;
    manager->tail = NULL;
    manager->total_count = 0;
    pthread_mutex_init(&manager->mutex_ball, NULL);
}

void server_ball_manager_destroy(BallListManager* manager) {
    pthread_mutex_destroy(&manager->mutex_ball);
    printf( COLOR_GREEN "Mutex 'mutex_ball' has been destroyed." COLOR_RESET);
    freeBallList(&manager->head);
}

void add_ball(BallListManager* manager, int count, int width, int height, int radius) {
    for (int i = 0; i < count; i++) {
        BallObject b = createBall(manager->total_count++, width, height, radius);
        manager->head = appendBall(manager->head, &manager->tail, b);
    }
}

void delete_ball(BallListManager* manager, int count) {
    for (int i = 0; i < count; i++) {
        if (deleteLastBall(&manager->head, &manager->tail) != NULL)
            manager->total_count--;
        else printf("There are no balls.\n");
    }
}

void move_all(dev_fb* fb, BallListManager* manager) {
    moveBallList(manager->head, fb->vinfo.xres, fb->vinfo.yres);
}


char* serialize_ball_list(BallListManager* manager, int count, int width, int height) {
    BallListNode* cur = manager->head;
    char* buffer = (char*)malloc(8192);  // 공이 많아질 경우 대비
    buffer[0] = '\0';

    while (cur) {
        char temp[256];
        snprintf(temp, sizeof(temp), "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d|",
                 cur->data.id,
                 cur->data.x, cur->data.y,
                 cur->data.dx, cur->data.dy,
                 cur->data.radius,
                 cur->data.color.r, cur->data.color.g, cur->data.color.b,
                 count, width, height);

        strcat(buffer, temp);
        cur = cur->next;
    }

    return buffer; // 호출자가 free 해야 함
}
















// 핸들러 함수 정의
void handle_add(BallListManager* m, int count, int width, int height,int radius) {
    if (count <= 0) count = 1;
    add_ball(m, count, width, height, radius); 
}

void handle_delete(BallListManager* m, int count, int width, int height,int radius) {
    if (count <= 0) count = 1;
    delete_ball(m, count);
}

void handle_speed_up(BallListManager* m, int count, int width, int height,int radius) {
    speedUpBalls(m->head);
}

void handle_speed_down(BallListManager* m, int count, int width, int height,int radius) {
    slowDownBalls(m->head);
}

// 테이블 매핑
typedef void (*CommandHandler)(BallListManager*, int, int, int, int);

typedef struct {
    char cmd;
    CommandHandler handler;
} CommandEntry;

static CommandEntry command_table[] = {
    {'a', handle_add},
    {'d', handle_delete},
    {'w', handle_speed_up},
    {'s', handle_speed_down}
};

void dispatch_command(char cmd, int count,int width, int height,int radius, BallListManager* m) {
    for (size_t i = 0; i < sizeof(command_table)/sizeof(CommandEntry); ++i) {
        if (command_table[i].cmd == cmd) {
            command_table[i].handler(m, count, width, height, radius);
            return;
        }
    }
    printf("[Server] Unknown command: %c\n", cmd);
}


