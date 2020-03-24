#include "usertraps.h"
#include "misc.h"
#include "utility.h"

int main(int argc, char const *argv[])
{
  sem_t sem_procs_completed;
  int num;
  int i;
  int j;
  int k;


  if (argc != 2) { 
    Printf("Incorrect number of arguments (%d) for %s\n", argc, argv[0]);
    Exit();
  } 

  sem_procs_completed = dstrtol(argv[1], NULL, 10);

  ProcessIdle();

  //Signal semaphore
  if(sem_signal(sem_procs_completed) != SYNC_SUCCESS){
    Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); 
    Printf(argv[0]); 
    Printf(", exiting...\n");
    Exit();
  }


  return 0;
}