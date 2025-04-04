#include "task.h"

// 1. TaskQueue 초기화
void task_queue_init(TaskQueue* q) {
    memset(q, 0,sizeof(TaskQueue));
    q->front = 0;
    q->rear = 0;
    q->count = 0;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->cond, NULL);
}

// 2. 작업 추가 (enqueue)
// main thread (epoll 루프)에서 작업을 넣을 때 호출
void enqueue_task(TaskQueue* q, Task task) {
    pthread_mutex_lock(&q->mutex);

    // 큐가 가득 찼으면 대기 (단순 구현: wait → 버퍼가 비면 signal 받음)
    while (q->count == TASK_QUEUE_CAPACITY) {
        pthread_cond_wait(&q->cond, &q->mutex);
    }

    q->queue[q->rear] = task;
    q->rear = (q->rear + 1) % TASK_QUEUE_CAPACITY;
    q->count++;

    pthread_cond_signal(&q->cond);  // 대기 중인 worker thread 깨움
    pthread_mutex_unlock(&q->mutex);
}

// 3. 작업 꺼내기 (dequeue)
// worker thread가 작업을 받아 처리할 때 호출
Task dequeue_task(TaskQueue* q) {
    pthread_mutex_lock(&q->mutex);

    // 큐가 비었으면 대기
    while (q->count == 0) {
        pthread_cond_wait(&q->cond, &q->mutex);
    }

    Task task = q->queue[q->front];
    q->front = (q->front + 1) % TASK_QUEUE_CAPACITY;
    q->count--;

    pthread_cond_signal(&q->cond);  // enqueue 대기 중일 수 있음
    pthread_mutex_unlock(&q->mutex);

    return task;
}

void task_queue_destroy(TaskQueue* q) {
    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->cond);
    printf( COLOR_GREEN "Mutex 'mutex, cond' has been destroyed." COLOR_RESET);
    free(q);
}