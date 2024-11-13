#include "fdtools.h"

#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static int32_t query(int fd, const char *text) {
  uint32_t len = (uint32_t)strlen(text);
  if (len > k_max_msg) {
    return -1;
  }

  char wbuf[4 + k_max_msg];
  memcpy(wbuf, &len, 4);
  memcpy(&wbuf[4], text, len);
  if (int32_t err = write_all(fd, wbuf, 4 + len)) {
    return err;
  }

  // 4 bytes header
  char rbuf[4 + k_max_msg + 1];
  errno = 0;
  int32_t err = read_full(fd, rbuf, 4);
  if (err) {
    if (errno == 0) {
      printf("EOF");
    } else {
      printf("read() error");
    }
    return err;
  }

  memcpy(&len, rbuf, 4); // assume little endian
  if (len > k_max_msg) {
    printf("too long");
    return -1;
  }

  // reply body
  err = read_full(fd, &rbuf[4], len);
  if (err) {
    printf("read() error");
    return err;
  }

  // do smth
  rbuf[4 + len] = '\0';
  printf("server says: %s\n", &rbuf[4]);
  return 0;
}

int main() {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    printf("error open socket\n");
    return 1;
  }

  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(1234);
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  if (connect(fd, (const struct sockaddr *)&addr, sizeof(addr)) < 0) {
    printf("cannot connect\n");
    return 1;
  }

  // if (query(fd, "Hello!")) {
  //   goto L_DONE;
  // }
  // if (query(fd, "Don't worry, be happy!")) {
  //   goto L_DONE;
  // }
  // if (query(fd, "42")) {
  //   goto L_DONE;
  // }

  // L_DONE:
  query(fd, "Hello");
  query(fd, "Hello");
  query(fd, "Hello");
  query(fd, "Hello");
  query(fd, "Hello");
  close(fd);
}
