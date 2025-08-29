#include "../include/socket.h"

WinSock* initialize_winsock() {
  WinSock* winsock = (WinSock*)malloc(sizeof(WinSock));

  int rc = WSAStartup(MAKEWORD(2, 2), &winsock->winsock_data);

  if (rc != 0) {
    WSA_ERROR("winsock intialization");
    return NULL;
  }

  return winsock;
}

Socket* create_socket(int port) {
  Socket* _socket = (Socket*)malloc(sizeof(Socket));

  _socket->descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (_socket->descriptor == INVALID_SOCKET) {
    WSA_ERROR("socket creation");
    return NULL;
  }

  _socket->address.sin_family = AF_INET;
  _socket->address.sin_addr.s_addr = INADDR_ANY;  
  _socket->address.sin_port = htons(port);

  return _socket;
}

Socket* set_address(Socket* _socket, const char* address) {
  _socket->address.sin_addr.s_addr = inet_addr(address);

  return _socket;
}

Socket* bind_socket(Socket* _socket) {
  int rc = bind(_socket->descriptor, (struct sockaddr*)&_socket->address, sizeof(_socket->address));

  if (rc == SOCKET_ERROR) {
    WSA_ERROR("binding failed");
    return NULL;
  }

  return _socket;
}

void listen_socket(Socket* _socket, int max_connection) {
  listen(_socket->descriptor, max_connection);
}