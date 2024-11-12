#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void echo(int connfd) {
  char rbuf[64] = {};
  ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
  if (n < 0) {
    printf("read error, connfg = %d\n", connfd);
    return;
  }
  printf("client says: %s\n", rbuf);

  char msg[] = "echo: ";
  char wbuf[n + sizeof(msg)];
  sprintf(wbuf, "%s%s", msg, rbuf);
  write(connfd, wbuf, sizeof(wbuf));
}

int main() {
  int fd = socket(AF_INET, SOCK_STREAM, 0);

  int val = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = ntohs(1234);
  addr.sin_addr.s_addr = ntohl(0);

  if (bind(fd, (const struct sockaddr *)&addr, sizeof(addr)) < 0) {
    printf("cannot bind\n");
    return 1;
  }

  if (listen(fd, SOMAXCONN) < 0) {
    printf("cannot listen\n");
    return 1;
  }

  while (1) {
    struct sockaddr_in client_addr = {};
    socklen_t socklen = sizeof(client_addr);
    int connfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
    if (connfd < 0) {
      printf("error while accept");
      continue;
    }

    echo(connfd);
    close(connfd);
  }
}