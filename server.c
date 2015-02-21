#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>

void handle_connection(int listen_sock) {
  struct sockaddr_in peer_addr;
  socklen_t addrlen = sizeof(peer_addr);
  int conn_sock;
  char buffer[100];
  ssize_t len_received;

  if((conn_sock = accept(listen_sock, (struct sockaddr *) &peer_addr, &addrlen)) == -1) {
    perror("accept");
    exit(1);
  }

  while(len_received = recv(conn_sock, buffer, sizeof(buffer)-1, 0)) {
    if(len_received == -1) {
      perror("recv");
      exit(1);
    }

    buffer[len_received] = 0;

    printf("%ld bytes: \"%s\"\n", len_received, buffer);
  }

  if(close(conn_sock)) {
    perror("close");
  }
}

int main() {
  int listen_sock;
  struct sockaddr_in addr;

  if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  addr.sin_addr.s_addr = INADDR_ANY;
  if (bind(listen_sock, (struct sockaddr *) &addr, sizeof(addr))) {
    perror("bind");
    exit(1);
  }

  if (listen(listen_sock, 5)) {
    perror("listen");
    exit(1);
  }

  printf("Server running. Interrupt to exit.\n");

  while(1) {
    handle_connection(listen_sock);
  }
}
