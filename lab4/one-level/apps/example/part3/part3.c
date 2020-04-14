#include "usertraps.h"
#include "misc.h"
#include "os/memory_constants.h"

void main (int argc, char *argv[])
{
  sem_t s_procs_completed; 
  int * address;

  if (argc != 2) { 
    Printf("Usage: %s <handle_to_procs_completed_semaphore>\n"); 
    Exit();
  } 

  s_procs_completed = dstrtol(argv[1], NULL, 10);

  Printf("part3 : Access memory inside the virtual address space, but outside of currently allocated pages.\n");

  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("part3 : Bad semaphore s_procs_completed !\n", s_procs_completed);
    Exit();
  }

  //Attempt to access memory location that is outside the set of allocated pages
  address = (MEM_MAX_VIRTUAL_ADDRESS + 1 - MEM_PAGESIZE) - 4;
  Printf("part3 : address to be accessed: %d\n", address);
  Printf("part3 : Accessing Memory Location: %d \n", *address);
  Printf("part3 : DONE!\n");

}