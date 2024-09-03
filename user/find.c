#include "kernel/types.h" // Do not remove this!
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#define TRUE 1
#define FALSE 0

int match(char* buf, char* pattern, int pos)
{
  char* bufPos = buf+pos;
  int match = FALSE;

  // Slide through the buffer (sliding window)
  for(int i = 0; strlen(pattern) <= strlen(bufPos) - i; i++)
  {
    char* bufPtr = bufPos+i;

    int charMatch = TRUE;
    for(int j = 0; j < strlen(pattern); j++)
    {
      if(pattern[j] != bufPtr[j])
      {
        charMatch = FALSE;
        break;
      }
    }

    if(charMatch == TRUE)
      return TRUE;
  }

  return match;
}
void
find(char *path, char *pattern)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  // Open fd to path
  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    exit(1);
  }

  // Close and bail if we can't 'stat' the fd
  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  // Check for too long a path
  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
    printf("find: path too long\n");
    exit(1);
  }

  strcpy(buf, path);
  p = buf+strlen(buf);
  if (strcmp(path, "/") != 0)
  {
    *p++ = '/';
  }


  while(read(fd, &de, sizeof(de)) == sizeof(de)){

    if(de.inum == 0 || strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0)
      continue;

    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    if(stat(buf, &st) < 0){
      printf("find: cannot stat %s\n", buf);
      continue;
    }

    if(match(path, pattern, 0) == TRUE || match(de.name, pattern, 0) == TRUE)
      printf("%s\n", buf);

    if (st.type == T_DIR)
      find(buf, pattern);
  }

  close(fd);
}


int
main(int argc, char *argv[])
{

  if(argc != 3){
    printf("find: requires 2 args\n");
    exit(1);
  }
  find(argv[1], argv[2]);

  exit(0);
}

