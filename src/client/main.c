#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <pthread.h>

#include "fbDraw.h"
#include "ball.h"
#include "ball_list.h"

#define MAX_INPUT 100

typedef struct {
    BallNode* head;
    BallNode* tail;
    int ball_total;   // 공 총 갯수
    int ball_flag;    // 갯수 증감 상태 플래그    0 : 유지, 1 : 증가, -1 : 감소
    int speed_flag;   // 속도 증감 상태 플래그    0 : 유지, 1 : 증가, -1 : 감소
    int running;      // 종료
    pthread_mutex_t mutex;
} BallManager;

// 쓰래드 인자 구조체
typedef struct {
    dev_fb* fb;
    BallManager* manager;
} ThreadArg;


char parseCommand(const char* cmdStr, int* ball_count) {
    if (!cmdStr || !ball_count) return 0;

    char op;
    int num;

    // "a:3", "d:5" 같은 경우 처리
    if (sscanf(cmdStr, "%c:%d", &op, &num) == 2) {
        *ball_count = num;
        return op;
    }

    // 단일 명령 문자 ("a", "d", "w", "s", "x")
    if (strlen(cmdStr) == 1) {
        *ball_count = 0;
        return cmdStr[0];
    }

    return 0;
}


// 입력 처리 스레드
void* input_thread(void* arg) {

    ThreadArg *ctx = (ThreadArg*)arg;

    dev_fb* fb = ctx->fb;
    BallManager* bm = ctx->manager;


    char input[MAX_INPUT];

    while (1) {
        printf("\n>> 명령 입력 (a, d, w, s, a:3, d:2, x): ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) continue;
        input[strcspn(input, "\n")] = '\0';

        int ball_count = 0; // 몇개나?
        char cmd = parseCommand(input, &ball_count);

        pthread_mutex_lock(&bm->mutex);

        if (cmd == 'x') {
            bm->running = 0;
            pthread_mutex_unlock(&bm->mutex);
            break;
        }

        switch (cmd) {
            case 'a':
                bm->ball_flag = 1;  // 공 증가
                if (ball_count == 0) ball_count = 1;
                for (int i = 0; i < ball_count; i++) {
                    Ball b = createBall(bm->ball_total++, fb->vinfo.xres, fb->vinfo.yres, 20);
                    bm->head = appendBall(bm->head, &bm->tail, b);
                }
                break;

            case 'd':
                bm->ball_flag = -1; // 공 감소
                if (ball_count == 0) ball_count = 1;
                for (int i = 0; i < ball_count; i++) {
                  if( deleteLastBall(&bm->head, &bm->tail) == NULL) printf(" 공이 0개 입니다\n");
                  else bm->ball_total--;
                }
                break;

            case 'w':
                bm->speed_flag = 1; // 속도 증가
                speedUpBalls(bm->head);
                break;

            case 's':
                bm->speed_flag = -1; // 속도 감소
                slowDownBalls(bm->head);
                break;

            default:
                printf("[!] 알 수 없는 명령입니다.\n");
        }

        if (cmd == 'a' || cmd == 'd' || cmd == 'w' || cmd == 's') {
          printInfoBall(bm->head);
        }
        pthread_mutex_unlock(&bm->mutex);
    }
    return ctx;
}


int main(int argc, char **argv)
{

  dev_fb fb;

  // 공 매니저 초기화
  BallManager manager = {
      .head = NULL,
      .tail = NULL,
      .ball_total = 0,  // 처음 공 갯수
      .ball_flag = 0,   // 갯수 -> 0 : 유지, 1 : 증가, -1 : 감소
      .speed_flag = 0,  // 속도 -> 0 : 유지, 1 : 증가, -1 : 감소
      .running = 1
  };
  pthread_mutex_init(&manager.mutex, NULL);

  srand(time(NULL)); // randDir() 사용을 위한 초기화

  if(fb_init(&fb) > 0)
  {
    perror("framebuffer init failed");
    return -1;
  }


  // 공 5개 생성해서 리스트에 추가
  for (int i = 0; i < 5; i++) {
    Ball b = createBall(manager.ball_total++, fb.vinfo.xres, fb.vinfo.yres ,20);
    manager.head = appendBall(manager.head, &manager.tail ,b);
  }

  printInfoBall(manager.head);

  ThreadArg* thread_arg = (ThreadArg* )malloc(sizeof(ThreadArg));
  if(thread_arg == NULL)
  {
    perror("malloc() : thread_arg");
    return -1;
  }

  thread_arg->fb = &fb;
  thread_arg->manager = &manager;

  pthread_t tid;
  if(pthread_create(&tid, NULL, input_thread, thread_arg) != 0)
  {
    free(thread_arg);
    perror("pthread_create()");
    return -1;
  }


  while (1) {
    pthread_mutex_lock(&manager.mutex);
    if (!manager.running) {
      pthread_mutex_unlock(&manager.mutex);
      break;
    }

    fb_fillScr(&fb, 0, 0, 0);
    moveBallList(manager.head, fb.vinfo.xres, fb.vinfo.yres);
    drawBallList(&fb, manager.head);

    pthread_mutex_unlock(&manager.mutex);
    usleep(1000000 / 60); // (60 FPS)
  }

  void* retval;
  pthread_join(tid, &retval);
  free(retval); // 매니저 메모리 해제

  pthread_mutex_destroy(&manager.mutex);
  freeBallList(&manager.head);  // 공 메모리 해제 함수
	close(fb.fbfd); // 프레임 버퍼 장치 닫기

	return 0;
}
