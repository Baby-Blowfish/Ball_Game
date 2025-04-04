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

    server_ball_manager_init(arg->ball_list_manager);
    client_list_manager_init(arg->client_list_manager);
    task_queue_init(arg->task_queue);

    return arg;
}

void manager_destroy(SharedContext* arg) {
    if (!arg) return;
    server_ball_manager_destroy(arg->ball_list_manager);
    client_list_manager_destroy(arg->client_list_manager);
    task_queue_destroy(arg->task_queue);
    free(arg);
    printf(COLOR_GREEN "Freed memory of the SharedContext." COLOR_RESET);
}


char parseCommand(const char* cmdStr, int* ball_count, int* width, int* height, int* radius) {
    if (!cmdStr || !ball_count || !width || !height || !radius) return 0;

    char op;

    // 전체 파라미터 있는 경우: a:3:800:600:30
    if (sscanf(cmdStr, "%c:%d:%d:%d:%d", &op, ball_count, width, height, radius) == 5) {
        return op;
    }

    // 단순 숫자만 있는 경우: a:3
    if (sscanf(cmdStr, "%c:%d", &op, ball_count) == 2) {
        *width = 800;
        *height = 600;
        *radius = 30;
        return op;
    }

    // 단일 명령어만 있는 경우: a, d, w, s, x
    if (strlen(cmdStr) == 1) {
        *ball_count = 0;
        *width = 800;
        *height = 600;
        *radius = 30;
        return cmdStr[0];
    }

    return 0;
}


// Worker thread 루프
void* worker_thread(void* arg) {
    SharedContext* ctx = (SharedContext*)arg;

    while (1) {
        Task task = dequeue_task(ctx->task_queue);

        // 수신한 데이터 null-terminate 보장
        task.data[task.length] = '\0';

        printf("[Worker] Processing task from fd %d: %s\n", task.fd, task.data);

        int count = 0, width = 0, height = 0, radius = 0;
        char cmd = parseCommand(task.data, &count, &width, &height, &radius);

        pthread_mutex_lock(&ctx->ball_list_manager->mutex_ball);
        dispatch_command(cmd, count, width, height, radius, ctx->ball_list_manager);  // 해상도는 임의 지정
        pthread_mutex_unlock(&ctx->ball_list_manager->mutex_ball);

        // 필요 시 클라이언트에 결과를 전송 (예: ack, 상태, 메시지 등)
        // send(task.fd, "OK", 2, 0);
    }

    return NULL;
}
