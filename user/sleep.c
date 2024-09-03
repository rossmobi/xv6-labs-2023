#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc != 2) {
    write(2, "Error!\n", 7);
    exit(1);
  }

  int sec = atoi(argv[1]);

  sleep(sec*10);
}
