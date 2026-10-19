#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  printf("\n===== Initial page table =====\n");
  vmprint();

  printf("\n===== After sbrk(8 pages) =====\n");
  if(sbrk(8 * 4096) == (char *)-1) {
    printf("sbrk failed\n");
    exit(1);
  }
  vmprint();

  printf("\n===== After shrinking by 1 page =====\n");
  if(sbrk(-4096) == (char *)-1) {
    printf("shrink failed\n");
    exit(1);
  }
  vmprint();

  exit(0);
}