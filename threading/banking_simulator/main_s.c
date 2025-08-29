#include "./include/server.h"

int main() {
  printf("creating server:\n");
  Server* server = create_server(8080);

  if (server) {
    printf("server created successfully\n");
  }
  else {
    printf("server creation failed\n");
    return 1;
  }

  printf("\n");

  server_listen(server, 100);

  return 0;
}