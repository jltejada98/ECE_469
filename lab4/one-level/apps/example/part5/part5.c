#include "usertraps.h"
#include "misc.h"


void main (int argc, char *argv[])
{
  sem_t s_procs_completed;

  if (argc != 2) { 
    Printf("Usage: %s <handle_to_procs_completed_semaphore>\n"); 
    Exit();
  } 

  s_procs_completed = dstrtol(argv[1], NULL, 10);

  Printf("part5: Hello world! %d \n", getpid());

  // Signal the semaphore to tell the original process that we're done
  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("part5 : Bad semaphore s_procs_completed !\n", s_procs_completed);
    Exit();
  }

  Printf("part5 : Done! %d\n", getpid());
}
