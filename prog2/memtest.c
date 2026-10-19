#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
showmem(char *label)
{
  struct meminfo mi;
printf("meminfo struct size: %d\n", (int)sizeof(mi));
  if(getmeminfo(&mi) < 0) {
    printf("getmeminfo failed\n");
    exit(1);
  }

  printf("\n%s\n", label);
  printf("Total pages: %d\n", (int)mi.total_pages);
  printf("Free pages: %d\n", (int)mi.free_pages);
  printf("Process pages: %d\n", (int)mi.process_pages);
  printf("Page-table pages: %d\n", (int)mi.page_table_pages);
}

int
main(void)
{
  showmem("Initial memory");

  char *mem = sbrk(10 * 4096);
  if(mem == (char *)-1) {
    printf("sbrk failed\n");
    exit(1);
  }

  // Touch each page so it is actually allocated.
  for(int i = 0; i < 10; i++)
    mem[i * 4096] = 'A';

  showmem("After allocating 10 pages");

  int pid = fork();

  if(pid == 0) {
    showmem("Child after fork");
    exit(0);
  } else if(pid > 0) {
    wait(0);
    showmem("Parent after child exits");
  } else {
    printf("fork failed\n");
  }

  sbrk(-10 * 4096);
  showmem("After freeing allocated memory");

  exit(0);
}