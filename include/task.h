#ifndef TASK_H
#define TASK_H

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "console_color.h"

#define TASK_QUEUE_CAPACITY 128  // 큐 최대 크기

// 1. Task 구조체: 처리할 작업 단위
typedef struct {
    int fd;                      // 클라이언트 파일 디스크립터
    char data[BUFSIZ];          // 수신한 데이터
    int length;                 // 데이터 길이
} Task;

// 2. TaskQueue 구조체: thread-safe 작업 큐
typedef struct {
    Task queue[TASK_QUEUE_CAPACITY];  // Task 저장 공간 (원형 큐)
    int front;                        // 큐 front 인덱스
    int rear;                         // 큐 rear 인덱스
    int count;                        // 현재 저장된 Task 수

    pthread_mutex_t mutex;           // 큐 접근을 위한 mutex
    pthread_cond_t cond;             // 조건 변수 (큐에 작업 존재 시 signal)
} TaskQueue;

// 큐 초기화
void task_queue_init(TaskQueue* q);

// 작업 추가
void enqueue_task(TaskQueue* q, Task task);

// 작업 꺼내기
Task dequeue_task(TaskQueue* q);

// 큐 삭제
void task_queue_destroy(TaskQueue* q);
#endif
