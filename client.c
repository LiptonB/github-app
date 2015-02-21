#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>

int main() {
  int conn_sock;
  struct sockaddr_in addr;
  char buffer[100];
  ssize_t sent_length;

  if ((conn_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  printf("Socket created, about to connect\n");
  getchar();

  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  if (connect(conn_sock, (struct sockaddr *) &addr, sizeof(addr))) {
    perror("connect");
    exit(1);
  }

  printf("Socket connected, about to send\n");
  getchar();

  strcpy(buffer, "A message.");
  sent_length = strlen(buffer);

  sent_length = send(conn_sock, buffer, sent_length, 0);

  printf("Message sent, about to close\n");
  getchar();

  close(conn_sock);

  printf("Socket closed\n");
}
