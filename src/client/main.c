#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <pthread.h>
#include "client.h"

#define THREAD_NUM 2

int main(int argc, char **argv)
{
  struct sockaddr_in servaddr;

  pthread_t tid[THREAD_NUM];

  GameSharedContext* arg = ball_client_arg_init();
  if(arg == NULL)
  {
    perror("malloc() : arg ");
    return -1;
  }

  // 서버 주소
  if (argc < 2) {
    printf("Usage : %s <SERVER_IP>\n", argv[0]);
    return -1;
  }

  // 소켓 초기화
  if ((arg->socket_ctx->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket()");
    return -1;
  }

  // rand() 초기화
  srand(time(NULL)); 

  // 서버 주소 설정
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERVER_PORT);
  inet_pton(AF_INET, argv[1], &servaddr.sin_addr.s_addr);

  // 서버에 연결
  if (connect(arg->socket_ctx->socket_fd , (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
      perror("connect()");
      return -1;
  }

  // 공 5개 속성 표시
  printInfoBall(arg->ball_list_manager->head);

  // 키보드 입력 스레드 생성
  if(pthread_create(&tid[0], NULL, socket_send_thread, arg) != 0)
  {
    ball_client_arg_destroy(arg);
    perror("pthread_create()");
    return -1;
  }

  // 서버 -> 클라이언트 수신용 스래드
  if(pthread_create(&tid[1], NULL, socket_recv_echo_thread, arg->socket_ctx) != 0)
  {
    ball_client_arg_destroy(arg);
    perror("pthread_create()");
    return -1;
  }

  while (1) {
    pthread_mutex_lock(&arg->ball_list_manager->mutex_ball);
    if (!arg->socket_ctx->is_running) {
      pthread_mutex_unlock(&arg->ball_list_manager->mutex_ball);
      break;
    }

    fb_fillScr(arg->framebuffer, 0, 0, 0);
    moveBallList(arg->ball_list_manager->head, arg->framebuffer->vinfo.xres, arg->framebuffer->vinfo.yres);
    draw_ball_list(arg->framebuffer, arg->ball_list_manager->head);
    pthread_mutex_unlock(&arg->ball_list_manager->mutex_ball);
    usleep(1000000 / 60); // (60 FPS)
  }

  for (int i = 0; i < THREAD_NUM; ++i) pthread_join(tid[i], NULL);  // 쓰래드 종료 대기
  shutdown(arg->socket_ctx->socket_fd, SHUT_RDWR); //  소켓 읽기 쓰기 종료 신호
  ball_client_arg_destroy(arg);

	return 0;
}
