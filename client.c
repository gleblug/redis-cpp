#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

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

  char buf[64];
  fgets(buf, sizeof(buf), stdin);
  write(fd, buf, sizeof(buf));

  char rbuf[64] = {};
  ssize_t n = read(fd, rbuf, sizeof(rbuf));
  if (n < 0) {
    printf("cannot read\n");
    return 1;
  }
  printf("%s\n", rbuf);
  close(fd);
}
