#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "fbDraw.h"


int main(int argc, char **argv)
{
  dev_fb  fb;

  switch (fb_init(&fb))
  {
    case FB_OPEN_FAIL :
    case FB_GET_FINFO_FAIL :
    case FB_GET_VINFO_FAIL :
    case FB_MMAP_FAIL :
      perror("framebuffer");
      return 1;
      break;
    default:
      break;
  }

  // 전체 화면 검정색으로 채우기
  fb_fillScr(&fb, 0, 0, 0);
  sleep(1);

  // 중심에 빨간 원 그리기
  pixel center = fb_toPixel(fb.vinfo.xres / 2, fb.vinfo.yres / 2);
  fb_drawFilledCircle(&fb, center, 255, 0, 0);
  sleep(1);

  // 좌측 상단에 하얀 사각형
  pixel topLeft = fb_toPixel(50, 50);
  fb_drawBox(&fb, topLeft, 100, 50, 255, 255, 255);
  sleep(1);

  // 한글자 출력 (숫자/영문/기호 한정)
  pixel textPos = fb_toPixel(100, 150);
  fb_drawChar(&fb, 'A', textPos, 30, 0, 255, 0);
  sleep(1);

  // 문자열 출력
  pixel strCursor = fb_toPixel(100, 200);
  fb_printStr(&fb, "HELLO", &strCursor, 30, 255, 255, 0);
  sleep(3);


	// 프레임 버퍼 장치 닫기
	close(fb.fbfd);

	return 0;
}
