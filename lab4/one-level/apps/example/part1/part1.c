#include "usertraps.h"
#include "misc.h"

void main (int argc, char *argv[])
{
  sem_t s_procs_completed; // Semaphore to signal the original process

  if (argc != 2) { 
    Printf("Usage: %s <handle_to_procs_completed_semaphore>\n"); 
    Exit();
  } 
  //Convert command line arguments
  s_procs_completed = dstrtol(argv[1], NULL, 10);

  Printf("part1: testing hello world\n");
  // Signal semaphore
  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("part1: Bad semaphore s_procs_completed (%d)!\n", getpid(), s_procs_completed);
    Exit();
  }
  Printf("part1: done!\n");
}
