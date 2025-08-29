#pragma once

#include "./shared.h"
#include "./socket.h"

Socket* create_server_socket(const char* address, int port);
int client_connect(Socket* server_socket);
int send_message(Socket* server_socket, const void* data, size_t length);
int send_credentials(Socket* server_socket, Credentials* credentials);