#include "usertraps.h"
#include "misc.h"
#include "os/memory_constants.h"

void main (int argc, char *argv[])
{
  sem_t s_procs_completed; // Semaphore to signal original process
  int * address;  //Memory Location to be accessed

  if (argc != 2) { 
    Printf("Usage: %s <handle_to_procs_completed_semaphore>\n"); 
    Exit();
  } 

  // Convert the command-line arguments into ints
  s_procs_completed = dstrtol(argv[1], NULL, 10);
  Printf("part2: Attempting to Access memory beyond the maximum virtual address.\n");
  //Signal Semaphore
  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("part2 : Bad semaphore s_procs_completed !\n", s_procs_completed);
    Exit();
  }
  address = (MEM_MAX_VIRTUAL_ADDRESS + 1);
  Printf("part2 : address to be accessed: %d\n", address);
  Printf("part2 : accessing virtual memory location: %d \n", *address);
  Printf("part2 : done!\n");  //Should never reach here because line above is illegal memory address
}