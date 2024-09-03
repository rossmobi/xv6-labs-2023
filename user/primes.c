#include "kernel/types.h"
#include "user/user.h"

void crunch_numbers(int READ);
void read_and_print(int READ, int *num);

int READ=0, WRITE=1;

int
main(int argc, char **argv)
{
  // Parent setup
  int p[2];
  pipe(p);
  
  if (fork() == 0) {
    for (int i = 2; i < 36; i++) {
      sleep(5);
      write(p[WRITE], &i, 4);
    }
    sleep(10);
    close(p[WRITE]);
    wait(0);
    exit(0);
    
  } else {
    close(p[WRITE]);
    crunch_numbers(p[READ]);
  }
}

void
crunch_numbers(int read_fd)
{
  int prime = 0, num = 0, forked = 0;
  // p = get a number from left neighbour
  read(read_fd, &prime, 4);
  // print p
  printf("prime %d\n", prime);

  int p[2];
  pipe(p);
    

  // loop:
  while (read(read_fd, &num, 4) > 0)  {
    // n = get a number from left neighbour
    // if (p does not divide n)
    if (num % prime != 0) {
      // send n to right neighbour
      write(p[WRITE], &num, 4);
      if (forked != 1) {
        forked = 1;
        if (fork() != 0) {
          close(p[WRITE]);
          crunch_numbers(p[READ]);
          return;
        }
      }
    }
  
  }

  close(read_fd);
  close(p[WRITE]);

  wait(0);
}

