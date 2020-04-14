#include "usertraps.h"
#include "misc.h"

void main (int argc, char *argv[])
{
  sem_t s_procs_completed; // Semaphore to signal the original process that we're done
  int i;
  
  if (argc != 2) { 
    Printf("Usage: pid: %d, %s <handle_to_procs_completed_semaphore>\n"); 
    Exit();
  } 

  s_procs_completed = dstrtol(argv[1], NULL, 10);

  Printf("part6 : pid: %d, Looping to a large number  \n", getpid());
  Printf("part6 : pid: %d, START! \n", getpid());

  for(i =0; i<50000; i++) {}

  Printf("part6 : pid: %d, looping complete  \n", getpid());

  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("part6 : pid: %d, Bad semaphore %d s_procs_completed !  \n", getpid(), s_procs_completed);
    Exit();
  }

  Printf("part6 : pid: %d, done \n", getpid());

}