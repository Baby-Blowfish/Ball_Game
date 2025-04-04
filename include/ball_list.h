#ifndef BALL_NODE_H
#define BALL_NODE_H

#include "ball.h"

/**
 * @brief 공 객체를 연결 리스트로 저장하는 노드 구조체
 */
typedef struct BallListNode {
    BallObject data;           ///< 저장된 공 객체
    struct BallListNode* next; ///< 다음 노드 포인터 (단일 연결 리스트)
} BallListNode;

BallListNode* createNode(BallObject ball);

BallListNode* appendBall(BallListNode* head, BallListNode** tail, BallObject ball);

void printInfoBall(BallListNode *head);

void moveBallList(BallListNode* head, int width, int height);

BallListNode* deleteLastBall(BallListNode** head, BallListNode** tail);

void speedUpBalls(BallListNode* head);

void slowDownBalls(BallListNode* head);

void freeBallList(BallListNode** head);

#endif