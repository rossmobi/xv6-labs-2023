#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char **argv) {
  int ping[2], pong[2], READ=0, WRITE=1;
  pipe(ping);
  pipe(pong);
  char *pingBuf=0, *pongBuf=0;
  
  write(ping[WRITE], "a", 1);

  if (fork() == 0) {
    read(pong[READ], pongBuf, 1);
    printf("%d: received pong\n", getpid());
  } else {
    read(ping[READ], pingBuf, 1);
    printf("%d: received ping\n", getpid());
    write(pong[WRITE], pingBuf, 1);
  }

  wait(0);
  exit(0);
}
