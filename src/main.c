#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "fbDraw.h"
#include "ball.h"
#include "ball_list.h"

char parseCommand(const char* cmdStr, int* count) {
    if (!cmdStr || !count) return 0;

    char op;
    int num;

    if (sscanf(cmdStr, "%c:%d", &op, &num) == 2) {
        *count = num;
        return op;  // 'a' 또는 'd'
    }

    return 0;  // 실패 시
}

int main(int argc, char **argv)
{

  dev_fb fb;
  char input[100];
  int count;
  char cmd;
  int ball_count = 0;

  srand(time(NULL)); // randDir() 사용을 위한 초기화

  if(fb_init(&fb) > 0)
  {
    perror("framebuffer init failed");
    return -1;
  }

  BallNode* ballListHead = NULL;
  BallNode* ballListTail = NULL;

  //head = loadBall(&tail, FILE_NAME);

  // 공 5개 생성해서 리스트에 추가
  for (int i = 0; i < 5; i++) {
    Ball b = createBall(ball_count++, fb.vinfo.xres, fb.vinfo.yres ,20);
    ballListHead = appendBall(ballListHead, &ballListTail ,b);
  }

  printInfoBall(ballListHead);

  // while (1) {
  //     fb_fillScr(&fb, 0, 0, 0); // 화면 초기화

  //     moveBallList(ballListHead, fb.vinfo.xres, fb.vinfo.yres);
  //     drawBallList(&fb, ballListHead);

  //     usleep(1000000 / 60); // 60 FPS
  // }

// 사용자 입력 시뮬레이션

strcpy(input, "a:3");

cmd = parseCommand(input, &count);

if (cmd == 'a') {
    for (int i = 0; i < count; i++) {
        Ball b = createBall(ball_count++, fb.vinfo.xres, fb.vinfo.yres, 20);
        ballListHead = appendBall(ballListHead, &ballListTail, b);
    }
} else if (cmd == 'd') {
    for (int i = 0; i < count; i++) {
      ball_count--;
      deleteLastBall(&ballListHead, &ballListTail); // 뒤에서 삭제하는 함수 
    }
}

printInfoBall(ballListHead);

strcpy(input, "d:2");

cmd = parseCommand(input, &count);

if (cmd == 'a') {
    for (int i = 0; i < count; i++) {
        Ball b = createBall(ball_count++, fb.vinfo.xres, fb.vinfo.yres, 20);
        ballListHead = appendBall(ballListHead, &ballListTail, b);
    }
} else if (cmd == 'd') {
    for (int i = 0; i < count; i++) {
      ball_count--;
      deleteLastBall(&ballListHead, &ballListTail); // 뒤에서 삭제하는 함수 
    }
}

printInfoBall(ballListHead);
	// 프레임 버퍼 장치 닫기
	close(fb.fbfd);

	return 0;
}
