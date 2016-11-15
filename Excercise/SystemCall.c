#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>


char data[1024];

main() {
  int fdin = open("/dev/zero", O_RDONLY);
  int fdout = open("tmp1.bin", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  int i;

  printf("fdin = %d\n", fdin);
  printf("fdout = %d\n", fdout);
  for(i = 0; i < 2097162; i++) {
    read(fdin, data, 1);
    write(fdout, data, 1, 0);
  }
  close(fdout);
  close(fdin);
}
