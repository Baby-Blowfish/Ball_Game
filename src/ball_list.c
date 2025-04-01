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