
#include "client.h"


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


