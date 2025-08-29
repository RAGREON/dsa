#include "./include/client.h"

int main(int argc, char** argv) {
  WinSock* winsock = initialize_winsock();

  Socket* server_socket = create_server_socket("127.0.0.1", 8080);

  client_connect(server_socket);

  char username[50];
  char password[50];

  printf("enter username: ");
  scanf("%s", username);

  printf("enter password: ");
  scanf("%s", password);

  printf("\nsending credentials to the server...");
 
  Credentials credentials;

  strcpy(credentials.username, username);
  strcpy(credentials.password, password);

  send_credentials(server_socket, &credentials);
 
  return 0;
}