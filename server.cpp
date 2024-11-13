#include "fdtools.h"

#include <assert.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static int32_t one_request(int connfd) {
  // 4 bytes header
  char rbuf[4 + k_max_msg + 1];
  errno = 0;
  int32_t err = read_full(connfd, rbuf, 4);
  if (err) {
    if (errno == 0) {
      printf("EOF");
    } else {
      printf("read() error");
    }
    return err;
  }

  uint32_t len = 0;
  memcpy(&len, rbuf, 4); // assume little endian
  if (len > k_max_msg) {
    printf("too long msg");
    return -1;
  }

  // request body
  err = read_full(connfd, &rbuf[4], len);
  if (err) {
    printf("read() error");
    return err;
  }

  // do smth
  rbuf[4 + len] = '\0';
  printf("client says: %s\n", &rbuf[4]);

  // reply using the same protocol
  const char reply[] = "world";
  char wbuf[4 + sizeof(reply)];
  len = (uint32_t)strlen(reply);

  memcpy(wbuf, &len, 4);
  memcpy(&wbuf[4], reply, len);
  return write_all(connfd, wbuf, 4 + len);
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
    printf("New connection! %d\n", connfd);
    if (connfd < 0) {
      printf("error while accept");
      continue;
    }

    while (1) {
      int32_t err = one_request(connfd);
      if (err) {
        break;
      }
    }

    close(connfd);
    printf("\nConnection closed! %d\n", connfd);
  }
}