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

void add_client(ClientListManager* manager, int csock, struct sockaddr_in cliaddr);

ClientNode* remove_client_by_socket(int socket_fd, ClientNode** head, ClientNode** tail);

void free_client_list(ClientNode** head);

void print_clients(ClientNode* head);

void client_list_manager_init(ClientListManager* manager);

void client_list_manager_destroy(ClientListManager* manager);




#endif // CLIENT_LIST_MANAGER_H
