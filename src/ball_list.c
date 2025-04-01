#include "ball_list.h"




BallNode* createNode(Ball ball) {

  BallNode *newnode = (BallNode*)malloc(sizeof(BallNode));
  if (!newnode) {
      return NULL;
  }

  memset(newnode, 0, sizeof(BallNode));
  
  newnode->data = ball;
  newnode->next = NULL;

    return newnode;
}

BallNode* appendBall(BallNode* head, BallNode** tail, Ball ball)
{
  BallNode* newnode = NULL;
  if((newnode =createNode(ball)) == NULL) 
    perror("\n\033[31m[Error] Memory allocation failed\033[0m\n\n");

  if (head == NULL) {
    head = newnode;
    *tail = newnode;
  } else {
    (*tail)->next = newnode;
    *tail = newnode;
  }

  printf("\n\033[1;36m [Success] '%d' added successfully.\033[0m\n\n", ball.id);

  return head;

}


void printInfoBall(BallNode *head)
{
    if (!head)
    {
        printf(COLOR_YELLOW "\nNo data available.\n\n" COLOR_RESET);
        return;
    }

    BallNode *cur = head;

    printf("\n................................... \n");
    while (cur)
    {
        printf("ID: %d,  x : %d,  y : %d,  radius : %d,  RGB : (%d %d %d)\n",
               cur->data.id, cur->data.x, cur->data.y, cur->data.radius, cur->data.color.r,cur->data.color.g,cur->data.color.b);
        cur = cur->next;


    }
    printf("................................... \n\n");
}


void drawBallList(dev_fb* fb, BallNode* head) {
    BallNode* cur = head;
    while (cur != NULL) {
        drawBall(fb,&(cur->data));
        cur = cur->next;
    }
}

void moveBallList(BallNode* head, int maxX, int maxY) {
    BallNode* cur = head;
    while (cur != NULL) {
        moveBall(&cur->data, maxX, maxY);
        cur = cur->next;
    }
}



BallNode* deleteLastBall(BallNode** head, BallNode** tail) {
    if (*head == NULL) return NULL;

    BallNode* removed = NULL;

    // 한 개만 있을 때
    if (*head == *tail) {
        removed = *head;
        *head = NULL;
        *tail = NULL;
    } else {
        BallNode* cur = *head;
        while (cur->next != *tail) {
            cur = cur->next;
        }
        removed = *tail;
        cur->next = NULL;
        *tail = cur;
    }

    printf("\n\033[1;36m [Success] '%d' Deleted successfully.\033[0m\n\n", removed->data.id);

    free(removed);
    return NULL;
}

void speedUpBalls(BallNode* head) {
    BallNode* cur = head;
    while (cur != NULL) {
        cur->data.dx *= 2;
        cur->data.dy *= 2;
        cur = cur->next;
    }
}

void slowDownBalls(BallNode* head) {
    BallNode* cur = head;
    while (cur != NULL) {
        // 속도가 1보다 작아지면 멈추기 때문에 최소 1 유지
        if (cur->data.dx > 0)
            cur->data.dx = (cur->data.dx > 1) ? cur->data.dx / 2 : 1;
        else if (cur->data.dx < 0)
            cur->data.dx = (cur->data.dx < -1) ? cur->data.dx / 2 : -1;

        if (cur->data.dy > 0)
            cur->data.dy = (cur->data.dy > 1) ? cur->data.dy / 2 : 1;
        else if (cur->data.dy < 0)
            cur->data.dy = (cur->data.dy < -1) ? cur->data.dy / 2 : -1;

        cur = cur->next;
    }
}
