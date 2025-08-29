#pragma once

#include "./shared.h"
#include "./socket.h"

#define BUFFER_SIZE 4028

typedef struct {
  WinSock*  winsock;
  Socket*   socket;
  int       port;
} Server;

Server* create_server(int port);
int accept_connection(Socket* server_socket, Socket* client_socket);
void server_listen(Server* server, int max_connection);
void* handle_client(void* arg);