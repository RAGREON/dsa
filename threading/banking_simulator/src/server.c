#include "../include/server.h"

Server* create_server(int port) {
  Server* server = (Server*)malloc(sizeof(Server));

  server->port = port;

  server->winsock = initialize_winsock();
  if (server->winsock == NULL) {
    printf("winsock initialization failed\n");
    return NULL;
  }

  server->socket = create_socket(port);
  bind_socket(server->socket);

  return server;
}

int accept_connection(Socket* server_socket, Socket* client_socket) {
  int client_sock_length = sizeof(client_socket->address);

  client_socket->descriptor = accept(
    server_socket->descriptor,
    (struct sockaddr*)&client_socket->address,
    &client_sock_length
  ); 
 
  if (client_socket->descriptor == INVALID_SOCKET) {
    WSA_ERROR("client socket accept failed");
    return 0;
  }

  printf("connection accepted\n");
  return 1;
}

void server_listen(Server* server, int max_connection) {
  listen_socket(server->socket, max_connection);
  
  printf("server is listening on port: %d\n", server->port);
  printf("max connections: %d\n\n", max_connection);
  
  printf("listening for incoming connections:\n");
  while (1) {
    Socket* client_socket = (Socket*)malloc(sizeof(Socket));

    if (!accept_connection(server->socket, client_socket)) {
      free(client_socket);
      continue;
    }

    pthread_t client_thread;
    pthread_create(&client_thread, NULL, handle_client, client_socket);
    pthread_detach(client_thread);
  }
}

void* handle_client(void* arg) {
  Socket* client_socket = (Socket*)arg;

  Credentials credentials;

  while (1) {
    int bytes = recv(client_socket->descriptor, (char*)&credentials, sizeof(credentials), 0);

    if (bytes == SOCKET_ERROR) {
      WSA_ERROR("recv failed\n");
      closesocket(client_socket->descriptor);
      break;
    }
    else if (bytes == 0) {
      printf("client disconnected\n");
      closesocket(client_socket->descriptor);
      break;
    }
    else {
      printf("username: %s\n", credentials.username);
      printf("password: %s\n", credentials.password);
    }
  } 

  free(client_socket);

  printf("connection closed to client\n");
}