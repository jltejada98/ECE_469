#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"
#include "utility.h"

int main(int argc, char const *argv[])
{
  sem_t sem_procs_completed; 
  int num_to_create;
  sem_t sem_water;

  int i;

  if (argc != 4) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_shared_memory_page> <handle_to_page_mapped_semaphore>\n"); 
    Exit();
  } 

  sem_procs_completed = dstrtol(argv[1], NULL, 10);
  num_to_create = dstrtol(argv[2], NULL, 10);
  sem_water = dstrtol(argv[3], NULL, 10);

  for(i = 0; i < num_to_create; i++)
  {
    if(sem_signal(sem_water) != SYNC_SUCCESS)
    {
      Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); 
      Printf(argv[0]); 
      Printf(", exiting...\n");
        Exit();
    }
    Printf("H2O injected into Radeon atmosphere, PID: %d \n", getpid());
  }

  //Signal complete semaphore
  if(sem_signal(sem_procs_completed) != SYNC_SUCCESS){
  Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); 
  Printf(argv[0]); 
  Printf(", exiting...\n");
  Exit();
  }



  return 0;
}