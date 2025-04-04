// client_list_manager.h
#ifndef CLIENT_LIST_MANAGER_H
#define CLIENT_LIST_MANAGER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "console_color.h"

#define MAX_CLIENTS 10

// 소켓
typedef struct {
    int csock;
    int is_running;
    struct sockaddr_in cliaddr;
} SocketContext;

// 클라이언트 노드 구조체
typedef struct ClientNode {
    SocketContext ctx;
    struct ClientNode* next;
} ClientNode;

typedef struct {
    ClientNode* head;  
    ClientNode* tail;  
    pthread_mutex_t mutex_client;
    int client_count;
} ClientListManager;

SocketContext create_client(int csock, struct sockaddr_in cliaddr);

ClientNode* create_client_node(SocketContext ctx);

ClientNode* append_client(ClientNode* head, ClientNode** tail, SocketContext ctx);

void print_clients(ClientNode* head);

/**
 * @brief 특정 소켓을 가진 클라이언트를 연결 리스트에서 제거합니다.
 * @param socket_fd 제거할 클라이언트의 소켓 FD
 * @param head 리스트의 헤드 포인터 주소
 * @param tail 리스트의 테일 포인터 주소
 * @return 제거된 노드 포인터 (호출자가 free 해야 함), 없으면 NULL
 */
ClientNode* remove_client_by_socket(int socket_fd, ClientNode** head, ClientNode** tail);

void free_client_list(ClientNode** head);

void client_list_manager_init(ClientListManager* manager);

void client_list_manager_destroy(ClientListManager* manager);

void add_client(ClientListManager* manager, int csock, struct sockaddr_in cliaddr);


#endif // CLIENT_LIST_MANAGER_H
