#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"
#include "utility.h"


//Arguments
//semaphore, shared_memory


int main(int argc, char const *argv[])
{
  sem_t sem_procs_completed;
  sem_t sem_water;
  sem_t sem_h;
  sem_t sem_o;
  int numReact;
  int i;
  int count;

  count = 0;
	Printf("CP: %d\n", count++);


  if (argc != 5) { 
    Printf("Incorrect Arguments for %s", argv[0]);
    Exit();
  } 


  sem_procs_completed = dstrtol(argv[1], NULL, 10);
  sem_water = dstrtol(argv[2], NULL, 10);
  sem_h = dstrtol(argv[3], NULL, 10);
  sem_o = dstrtol(argv[4], NULL, 10);
  numReact = dstrtol(argv[5], NULL, 10);

  Printf("Sem H: %d\n", sem_h);
  Printf("Sem O: %d\n", sem_o);
  Printf("")

  for(i = 0; i < numReact; i++)
  {
	Printf("In loop\n");

  	//Wait for resources
    if(sem_wait(sem_water) != SYNC_SUCCESS)
    {
			Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); 
			Printf(argv[0]); 
			Printf(", exiting...\n");
			Exit();
    }
    if(sem_wait(sem_water) != SYNC_SUCCESS)
    {
			Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); 
			Printf(argv[0]); 
			Printf(", exiting...\n");
			Exit();
    }



    //Generate resources
    if(sem_signal(sem_h) != SYNC_SUCCESS)
    {
			Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); 
			Printf(argv[0]); 
			Printf(", exiting...\n");
			Exit();
    }
    if(sem_signal(sem_h) != SYNC_SUCCESS)
    {
			Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); 
			Printf(argv[0]); 
			Printf(", exiting...\n");
			Exit();
    }
    if(sem_signal(sem_o) != SYNC_SUCCESS)
    {
			Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); 
			Printf(argv[0]); 
			Printf(", exiting...\n");
			Exit();
    }

    Printf("(1) H2O -> 2 H2 + O2 reacted, PID: %d", getpid());
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