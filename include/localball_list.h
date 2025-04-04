#ifndef LOCAL_BALL_NODE_H
#define LOCAL_BALL_NODE_H

#include "localball.h"

/**
 * @brief 공 객체를 연결 리스트로 저장하는 노드 구조체
 */
typedef struct BallListNode {
    LogicalBall data;           ///< 저장된 공 객체
    struct BallListNode* next; ///< 다음 노드 포인터 (단일 연결 리스트)
} BallListNode;

BallListNode* createNode(LogicalBall ball);

BallListNode* appendBall(BallListNode* head, BallListNode** tail, LogicalBall ball);

void printInfoBall(BallListNode *head);

void moveBallList(BallListNode* head);

BallListNode* deleteLastBall(BallListNode** head, BallListNode** tail);

void speedUpBalls(BallListNode* head);

void slowDownBalls(BallListNode* head);

void freeBallList(BallListNode** head);

#endif //LOCAL_BALL_NODE_H