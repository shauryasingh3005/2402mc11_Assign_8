#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int global_var = 100;

int
main(void)
{
  int local_var = 200;
  char *heap_var = malloc(4096);

  printf("Global variable: VA=%p, PA=%p\n",
       (void *)&global_var,
       (void *)va2pa((uint64)&global_var));

printf("Local variable: VA=%p, PA=%p\n",
       (void *)&local_var,
       (void *)va2pa((uint64)&local_var));

printf("Heap variable: VA=%p, PA=%p\n",
       (void *)heap_var,
       (void *)va2pa((uint64)heap_var));

printf("Invalid address result: %d\n",
       (int)va2pa(0x7fffffff));

      int pid = fork();

  if(pid == 0){
    printf("Child:  VA=%p, PA=%p\n",
           (void *)&global_var,
           (void *)va2pa((uint64)&global_var));
    exit(0);
  } else if(pid > 0){
    printf("Parent: VA=%p, PA=%p\n",
           (void *)&global_var,
           (void *)va2pa((uint64)&global_var));
    wait(0);
  } else {
    printf("fork failed\n");
  }
  
  free(heap_var);
  exit(0);
}