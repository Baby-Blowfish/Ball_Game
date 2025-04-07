#ifndef SCREEN_BALL_NODE_H
#define SCREEN_BALL_NODE_H

#include "screenball.h"

/**
 * @brief 공 객체를 연결 리스트로 저장하는 노드 구조체
 */
typedef struct BallListNode {
    ScreenBall data;           ///< 저장된 공 객체
    struct BallListNode* next; ///< 다음 노드 포인터 (단일 연결 리스트)
} BallListNode;

BallListNode* createNode(ScreenBall ball);

BallListNode* appendBall(BallListNode* head, BallListNode** tail, ScreenBall ball);

void printInfoBall(BallListNode *head);


BallListNode* deleteLastBall(BallListNode** head, BallListNode** tail);

BallListNode* deep_copy_ball_list(BallListNode* head);

void freeBallList(BallListNode** head);

#endif  //SCREEN_BALL_NODE_H