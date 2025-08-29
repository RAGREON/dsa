#include "./client.h"

Socket* create_server_socket(const char* address, int port) {
  Socket* _socket = create_socket(port);
  set_address(_socket, address);
  return _socket;
}

int client_connect(Socket* server_socket) {
  int rc = connect(server_socket->descriptor, (struct sockaddr*)&server_socket->address, sizeof(server_socket->address));

  if (rc == SOCKET_ERROR) {
    WSA_ERROR("connection to server failed");
    return 0;
  }

  printf("connected to server:\n");
  return 1; 
}

int send_message(Socket* server_socket, const void* data, size_t length) {
  int bytes_sent = send(server_socket->descriptor, (const char*)data, length, 0);

  if (bytes_sent == SOCKET_ERROR) {
    WSA_ERROR("send failed");
    return -1;
  }

  return bytes_sent;
}

int send_credentials(Socket* server_socket, Credentials* credentials) {
  int bytes = send_message(server_socket, credentials, sizeof(*credentials));
  return bytes;
}