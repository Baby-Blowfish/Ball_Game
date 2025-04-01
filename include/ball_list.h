#ifndef BALL_NODE_H
#define BALL_NODE_H

#include "ball.h"
#include "common.h"

typedef struct BallNode {
    Ball data;
    struct BallNode* next;
} BallNode;



BallNode* createNode(Ball ball);

BallNode* appendBall(BallNode* head, BallNode** tail, Ball ball);

void printInfoBall(BallNode *head);

void drawBallList(dev_fb* fb, BallNode* head);

void moveBallList(BallNode* head, int maxX, int maxY);


#endif