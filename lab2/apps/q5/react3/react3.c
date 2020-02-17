#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"
#include "utility.h"


//Arguments
//semaphore, shared_memory


int main(int argc, char const *argv[])
{
  sem_t sem_procs_completed, sem_h, sem_o, sem_s02, sem_h_sulfate;
  int numReact;
  int i;

  if (argc != 7) { 
    Printf("Incorrect Arguments for %s", argv[0]);
    Exit();
  } 

  sem_procs_completed = dstrtol(argv[1], NULL, 10);
  sem_h = dstrtol(argv[2], NULL, 10);
  sem_o = dstrtol(argv[3], NULL, 10);
  sem_s02 = dstrtol(argv[4], NULL, 10);
  sem_h_sulfate = dstrtol(argv[5], NULL, 10);
  numReact = dstrtol(argv[6], NULL, 10);

  for(i = 0; i < numReact; i++)
  {
    //Wait for resources
    if(sem_wait(sem_h) != SYNC_SUCCESS)
    {
      Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); 
      Printf(argv[0]); 
      Printf(", exiting...\n");
      Exit();
    }
    if(sem_wait(sem_o) != SYNC_SUCCESS)
    {
      Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); 
      Printf(argv[0]); 
      Printf(", exiting...\n");
      Exit();
    }
    if(sem_wait(sem_s02) != SYNC_SUCCESS)
    {
      Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); 
      Printf(argv[0]); 
      Printf(", exiting...\n");
      Exit();
    }

    //Create resources
    if(sem_signal(sem_h_sulfate) != SYNC_SUCCESS)
    {
      Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); 
      Printf(argv[0]); 
      Printf(", exiting...\n");
      Exit();
    }

    Printf("(3) H2 + O2 + SO2 -> H2SO4 reacted, PID: %d", getpid());
  }

  //Signal semaphore
  if(sem_signal(sem_procs_completed) != SYNC_SUCCESS){
    Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); 
    Printf(argv[0]); 
    Printf(", exiting...\n");
    Exit();
  }



  return 0;
}