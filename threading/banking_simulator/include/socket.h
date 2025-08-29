#pragma once

#include "./shared.h"

typedef struct {
  WSADATA             winsock_data;
} WinSock;

typedef struct {
  SOCKET              descriptor;
  struct sockaddr_in  address; 
} Socket;

WinSock* initialize_winsock();

Socket* create_socket(int port);
Socket* set_address(Socket* _socket, const char* address);
Socket* bind_socket(Socket* _socket);
void listen_socket(Socket* _socket, int max_connection);