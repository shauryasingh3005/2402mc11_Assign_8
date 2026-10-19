#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


static void
showmem(char *label)
{
  struct meminfo mi;

  if(getmeminfo(&mi) < 0){
    printf("getmeminfo failed\n");
    return;
  }

  printf("%s: free pages=%d, process pages=%d\n",
         label, (int)mi.free_pages, (int)mi.process_pages);
}

int
main(void)
{
  char *p;

  showmem("Before sbrk");

  // Reserve 32 MB without immediately allocating physical pages.
  p = sbrk(32 * 1024 * 1024);
  if(p == (char *)-1){
    printf("sbrk failed\n");
    exit(1);
  }

  showmem("After sbrk");

  // Touch only a few widely spaced pages.
  p[0] = 'A';
  p[4 * 1024 * 1024] = 'B';
  p[16 * 1024 * 1024] = 'C';
  p[31 * 1024 * 1024] = 'D';

  printf("Touched pages: %c %c %c %c\n",
         p[0], p[4 * 1024 * 1024],
         p[16 * 1024 * 1024], p[31 * 1024 * 1024]);

  showmem("After touching pages");

  int pid = fork();

  if(pid < 0){
    printf("fork failed\n");
    exit(1);
  }

  if(pid == 0){
    printf("Child: %c %c\n", p[0], p[31 * 1024 * 1024]);
    exit(0);
  }

  wait(0);

  // Test a system call writing into a reserved but untouched page.
  // Page 8 MB was never touched before this read.
  int fd[2];
  if(pipe(fd) == 0){
    char c = 'Z';
    write(fd[1], &c, 1);
    close(fd[1]);

    char *untouched = p + 8 * 1024 * 1024;
    if(read(fd[0], untouched, 1) == 1)
      printf("Untouched read buffer: %c\n", untouched[0]);

    close(fd[0]);
  }

  showmem("After fork and read");
  printf("Lazy allocation test completed\n");
  exit(0);
}