#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "fbDraw.h"
#include "ball.h"
#include "ball_list.h"

int main(int argc, char **argv)
{

  dev_fb fb;

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
    Ball b = createBall(i, fb.vinfo.xres, fb.vinfo.yres ,20);
    ballListHead = appendBall(ballListHead, &ballListTail ,b);
  }

  printInfoBall(ballListHead);

  fb_fillScr(&fb, 0, 0, 0); // 전체 검정으로 클리어
  drawBallList(&fb, ballListHead);


	// 프레임 버퍼 장치 닫기
	close(fb.fbfd);

	return 0;
}
