#include "screenballmanager.h"

void draw_ball(dev_fb* fb, ScreenBall* b) {
    pixel center = fb_toPixel(b->x, b->y);
    fb_drawFilledCircle(fb, center, b->color.r, b->color.g, b->color.b);
}

void draw_ball_list(dev_fb* fb, BallListNode* head) {
    BallListNode* cur = head;
    while (cur != NULL) {
        draw_ball(fb,&(cur->data));
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

int logic_to_pixel(float logic, int screen_size) {
    return (int)((logic / 1000.0f) * screen_size);
}

/**
 * @brief 논리 좌표계(LogicalBall)를 화면 좌표계(ScreenBall)로 변환
 * 
 * @param l 서버에서 받은 논리 공 정보
 * @param width 클라이언트 해상도 너비
 * @param height 클라이언트 해상도 높이
 * @return 변환된 화면 좌표계 공 객체
 */
ScreenBall logical_to_screen_ball(LogicalBall l, int width, int height) {
    ScreenBall s;
    s.id = l.id;
    s.x = logic_to_pixel(l.x, width);
    s.y = logic_to_pixel(l.y, height);
    s.dx = logic_to_pixel(l.dx, width);
    s.dy = logic_to_pixel(l.dy, height);

    int min_res = (width < height) ? width : height;
    s.radius = logic_to_pixel(l.radius, min_res);

    s.color = l.color; // RGB 복사
    return s;
}

/**
 * @brief 화면 좌표계(ScreenBall)를 논리 좌표계(LogicalBall)로 변환
 * 
 * @param s 화면 좌표계 공 정보
 * @param width 클라이언트 해상도 너비
 * @param height 클라이언트 해상도 높이
 * @return 변환된 논리 좌표계 공 객체
 */
LogicalBall screen_to_logical_ball(ScreenBall s, int width, int height) {
    LogicalBall l;
    l.id = s.id;
    
    // 픽셀 좌표를 논리 좌표로 변환 (0~1000 범위)
    l.x = (float)s.x / width * 1000.0f;
    l.y = (float)s.y / height * 1000.0f;
    l.dx = (float)s.dx / width * 1000.0f;
    l.dy = (float)s.dy / height * 1000.0f;

    int min_res = (width < height) ? width : height;
    l.radius = (float)s.radius / min_res * 1000.0f;

    l.color = s.color; // RGB 복사
    return l;
}

void updateBallListFromSerialized(BallListManager* manager, const char* str, int width, int height) {
    delete_ball(manager, manager->total_count); // 전체 삭제

    char* input = strdup(str);
    if(input == NULL) return;   // 복사할 공간이 없을때

    char* token = strtok(input, "|");
    while (token != NULL) {
        LogicalBall l;
        sscanf(token, "%d,%f,%f,%f,%f,%d,%hhu,%hhu,%hhu",
               &l.id,
               &l.x, &l.y,
               &l.dx, &l.dy,
               &l.radius,
               &l.color.r, &l.color.g, &l.color.b);
        ScreenBall ball = logical_to_screen_ball(l, width, height);
        manager->head = appendBall(manager->head, &manager->tail, ball);
        manager->total_count++;
    }
    free(input);
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
