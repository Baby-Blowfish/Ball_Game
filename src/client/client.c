
#include "client.h"


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

void* socket_recv_echo_thread(void* arg) {
    
    SocketContext* t_arg = (SocketContext*)arg;
    char recv_buf[BUFSIZ];
    int n;

    while(1)
    {
      pthread_mutex_lock(&t_arg->mutex_socket);
      if (!t_arg->is_running) {
        pthread_mutex_unlock(&t_arg->mutex_socket);
        break;
      }
      pthread_mutex_unlock(&t_arg->mutex_socket);

      n = recv(t_arg->socket_fd, recv_buf, BUFSIZ - 1, 0);
      if (n <= 0) {
          if (n == 0) printf("The server has terminated the connection.\n");
          else perror("recv()");
          break;
      }
      recv_buf[n] = '\0';
      printf("[Client] Received from server: %s\n", recv_buf);
      fflush(stdout);
    }

    pthread_mutex_lock(&t_arg->mutex_socket);
    t_arg->is_running = 0;
    pthread_mutex_unlock(&t_arg->mutex_socket);

    pthread_exit(NULL);
}


void* socket_send_echo_thread(void* arg) {
    SocketContext* t_arg = (SocketContext*)arg;
    char send_buf[BUFSIZ];

    while (1) {
      pthread_mutex_lock(&t_arg->mutex_socket);
      if (!t_arg->is_running) {
        pthread_mutex_unlock(&t_arg->mutex_socket);
        break;
      }
      pthread_mutex_unlock(&t_arg->mutex_socket);

      printf("[Client] Enter a message (enter 'q' to quit): ");
      if (fgets(send_buf, BUFSIZ, stdin) == NULL)
          continue;

      send(t_arg->socket_fd, send_buf, strlen(send_buf), 0);

      if (strncmp(send_buf, "q", 1) == 0) {
        pthread_mutex_lock(&t_arg->mutex_socket);
        t_arg->is_running = 0;
        pthread_mutex_unlock(&t_arg->mutex_socket);

        shutdown(t_arg->socket_fd, SHUT_WR);
        break;
      }
    }

    pthread_exit(NULL);
}

// 클라이언트 ->  서버 송신 스레드 : 입력  명령 받기 및 전송
void* socket_send_thread(void* arg) {

    GameSharedContext *ctx = (GameSharedContext*)arg;

    dev_fb* framebuffer = ctx->framebuffer;
    BallListManager* ball_list_manager = ctx->ball_list_manager;
    SocketContext* socket_ctx = ctx->socket_ctx;

    char input[MAX_INPUT];

    while (1) {

        pthread_mutex_lock(&socket_ctx->mutex_socket);
        if (!socket_ctx->is_running) {
            pthread_mutex_unlock(&socket_ctx->mutex_socket);
            break;
        }
        pthread_mutex_unlock(&socket_ctx->mutex_socket);

        printf("\n[ Command Guide ]\n"
        "- Create a ball           : a        (e.g., a)\n"
        "- Delete a ball           : d        (e.g., d)\n"
        "- Increase speed          : w\n"
        "- Decrease speed          : s\n"
        "- Create multiple balls   : a:<count> (e.g., a:3)\n"
        "- Delete multiple balls   : d:<count> (e.g., d:2)\n"
        "- Exit the program        : x\n"
        ">> Enter command: ");

        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) continue;
        input[strcspn(input, "\n")] = '\0';

        int ball_count = 0; // 몇개나?
        char cmd = parseCommand(input, &ball_count);

        
        send(socket_ctx->socket_fd, input, strlen(input), 0);

        pthread_mutex_lock(&ball_list_manager->mutex_ball_list);

        if (cmd == CMD_EXIT) {
            pthread_mutex_lock(&socket_ctx->mutex_socket);
            socket_ctx->is_running = 0;
            pthread_mutex_unlock(&socket_ctx->mutex_socket);
            pthread_mutex_unlock(&ball_list_manager->mutex_ball_list);
            shutdown(socket_ctx->socket_fd, SHUT_WR);
            break;
        }

        switch (cmd) {
            case CMD_ADD:
                ball_list_manager->flag_add_remove = 1;  // 공 증가
                if (ball_count == 0) ball_count = 1;
                for (int i = 0; i < ball_count; i++) {
                    BallObject b = createBall(ball_list_manager->total_count++, framebuffer->vinfo.xres, framebuffer->vinfo.yres, 20);
                    ball_list_manager->head = appendBall(ball_list_manager->head, &ball_list_manager->tail, b);
                }
                break;

            case CMD_DEL:
                ball_list_manager->flag_add_remove = -1; // 공 감소
                if (ball_count == 0) ball_count = 1;
                for (int i = 0; i < ball_count; i++) {
                  if( deleteLastBall(&ball_list_manager->head, &ball_list_manager->tail) == NULL)   printf("There are no balls.\n");
                  else ball_list_manager->total_count--;
                }
                break;

            case CMD_SPEED_UP:
                ball_list_manager->flag_speed_change = 1; // 속도 증가
                speedUpBalls(ball_list_manager->head);
                break;

            case CMD_SPEED_DOWN:
                ball_list_manager->flag_speed_change = -1; // 속도 감소
                slowDownBalls(ball_list_manager->head);
                break;

            default:
                printf("[!] Unknown command.\n");
        }

        if (cmd == CMD_ADD || cmd == CMD_DEL || cmd == CMD_SPEED_UP || cmd == CMD_SPEED_DOWN) {
          printInfoBall(ball_list_manager->head);
        }
        pthread_mutex_unlock(&ball_list_manager->mutex_ball_list);
    }
    return NULL;
}


GameSharedContext* ball_client_arg_init() {
    GameSharedContext* arg = malloc(sizeof(GameSharedContext));
    if (!arg) {
        perror("malloc() : BallClientArg");
        return NULL;
    }

    arg->framebuffer = malloc(sizeof(dev_fb));
    arg->ball_list_manager = malloc(sizeof(BallListManager));
    arg->socket_ctx = malloc(sizeof(SocketContext));

    if (!arg->framebuffer || !arg->ball_list_manager || !arg->socket_ctx) {
        perror("malloc() : internal");
        free(arg->framebuffer); free(arg->ball_list_manager); free(arg->socket_ctx); free(arg);
        return NULL;
    }

    client_ball_manager_init(arg->ball_list_manager, START_BALL_COUNT, arg->framebuffer->vinfo.xres,arg->framebuffer->vinfo.yres);

    // 초기화: socket_ctx
    memset(arg->socket_ctx, 0, sizeof(SocketContext));
    pthread_mutex_init(&arg->socket_ctx->mutex_socket, NULL);
    arg->socket_ctx->is_running = 1;

    // framebuffer 초기화
    if(fb_init(arg->framebuffer) > 0)
    {
        perror("framebuffer init failed");
        return NULL;
    }

    return arg;
}

void ball_client_arg_destroy(GameSharedContext* arg) {
    if (!arg) return;

    client_ball_manager_destroy(arg->ball_list_manager);

    if (arg->socket_ctx) {
        pthread_mutex_destroy(&arg->socket_ctx->mutex_socket);
        free(arg->socket_ctx);
    }

    if (arg->framebuffer) {
        fb_close(arg->framebuffer);
        free(arg->framebuffer);
    }

    free(arg);
}