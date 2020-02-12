#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"
#include "utility.h"
#include "spawn.h"

int main(int argc, char const *argv[])
{
  //circular_buffer *cb
  sem_t sem_procs_completed; 

  // if (argc != 3) { 
  //   Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_shared_memory_page> <handle_to_page_mapped_semaphore>\n"); 
  //   Exit();
  // } 

  sem_procs_completed = dstrtol(argv[1], NULL, 10);

  Printf("Producer_Created\n");

  // int i;
  // for(i = 0; i < 11; i++)
  // {
  //   //Get lock
  //   //Produce resource
  //   //Release lock
  // }

  //Signal semaphore
  if(sem_signal(sem_procs_completed) != SYNC_SUCCESS){
    Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); 
    Printf(argv[0]); 
    Printf(", exiting...\n");
    Exit();
  }



  return 0;
}