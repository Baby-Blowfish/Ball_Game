#include "server.h"


SharedContext* manager_init() {
    SharedContext* arg = malloc(sizeof(SharedContext));
    if (!arg) {
        perror("malloc() : BallServerArg");
        return NULL;
    }

    arg->ball_list_manager = malloc(sizeof(BallListManager));
    arg->client_list_manager = malloc(sizeof(ClientListManager));
    arg->task_queue = malloc(sizeof(TaskQueue));


    if (!arg->ball_list_manager || !arg->client_list_manager || !arg->task_queue) {
        perror("malloc() : internal");
        free(arg->ball_list_manager); free(arg->client_list_manager);
        return NULL;
    }

    ball_manager_init(arg->ball_list_manager);
    client_list_manager_init(arg->client_list_manager);
    task_queue_init(arg->task_queue);

    return arg;
}

void manager_destroy(SharedContext* arg) {
    if (!arg) return;
    ball_manager_destroy(arg->ball_list_manager);
    client_list_manager_destroy(arg->client_list_manager);
    task_queue_destroy(arg->task_queue);
    free(arg);
    printf(COLOR_GREEN "Freed memory of the SharedContext." COLOR_RESET);
}

// 명령 파싱 함수: a:3:30, d:2, w, s, x 등 다양한 형태 지원
char parseCommand(const char* cmdStr, int* ball_count, int* radius) {
    if (!cmdStr || !ball_count || !radius) return 0;

    char op;
    int n1 = 0, n2 = 0;

    // 전체 파라미터 있는 경우: a:3:30 (공 3개, 반지름 30)
    if (sscanf(cmdStr, "%c:%d:%d", &op, &n1, &n2) == 3) {
        *ball_count = n1;
        *radius = n2;
        return op;
    }

    // 기본 형태: a:3
    if (sscanf(cmdStr, "%c:%d", &op, &n1) == 2) {
        *ball_count = n1;
        *radius = START_BALL_RADIUS; // 기본값
        return op;
    }

    // 단일 명령어: a, d, w, s, x
    if (strlen(cmdStr) == 1) {
        *ball_count = 0;
        *radius = START_BALL_RADIUS; // 기본값
        return cmdStr[0];
    }

    return 0;
}

// 공 리스트를 문자열로 직렬화하여 모든 클라이언트에 전송
void broadcast_ball_state(ClientListManager* client_mgr, BallListManager* ball_mgr) {
   

    pthread_mutex_lock(&ball_mgr->mutex_ball);
    char* buffer  = serialize_ball_list(ball_mgr);
    pthread_mutex_unlock(&ball_mgr->mutex_ball);

    if(!buffer) return;

    pthread_mutex_lock(&client_mgr->mutex_client);
    ClientNode* curr = client_mgr->head;
    while (curr) {
        send(curr->ctx.csock, buffer, strlen(buffer), 0);
        curr = curr->next;
    }
    pthread_mutex_unlock(&client_mgr->mutex_client);

    free(buffer);
}

// Worker thread 루프
void* worker_thread(void* arg) {

    int count = 0, radius = 0;
    SharedContext* ctx = (SharedContext*)arg;

    while (keep_running) {

        Task task = dequeue_task(ctx->task_queue);
        task.data[task.length] = '\0'; // 수신한 데이터 null-terminate 보장

        printf(COLOR_BLUE "[Worker] Processing task from fd %d: %s" COLOR_RESET, task.fd, task.data);

        char cmd = parseCommand(task.data, &count, &radius);

        if(cmd == CMD_EXIT)
        {
            // 클라이언트 종료 처리
            printf(COLOR_YELLOW "[Server] Client requested disconnect (fd=%d)" COLOR_RESET, task.fd);

            pthread_mutex_lock(&ctx->client_list_manager->mutex_client);
            ClientNode* removed = remove_client_by_socket(task.fd, &ctx->client_list_manager->head, &ctx->client_list_manager->tail);
            if (removed) {
                ctx->client_list_manager->client_count--;
                epoll_ctl(ctx->epoll_fd, EPOLL_CTL_DEL, task.fd, NULL);
                shutdown(removed->ctx.csock, SHUT_RDWR);
                close(removed->ctx.csock);
                free(removed);
            }
            pthread_mutex_unlock(&ctx->client_list_manager->mutex_client);

            continue; // 다음 작업 처리
        }

        // 다른 명령이 들어왔을때 처리 필요함!

        pthread_mutex_lock(&ctx->ball_list_manager->mutex_ball);
        dispatch_command(cmd, count, radius, ctx->ball_list_manager);  // 해상도는 임의 지정
        pthread_mutex_unlock(&ctx->ball_list_manager->mutex_ball);

        char response[64];
        snprintf(response, sizeof(response), "OK %c : %d\n", cmd, count);
        send(task.fd, response, strlen(response), 0);

        broadcast_ball_state(ctx->client_list_manager, ctx->ball_list_manager);

    }

    return NULL;
}
