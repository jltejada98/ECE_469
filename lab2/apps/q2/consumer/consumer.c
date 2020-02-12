#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"
#include "spawn.h"
#include "utility.h"


//Arguments
//semaphore, shared_memory


int int main(int argc, char const *argv[])
{
  //circular_buffer *cb
  sem_t s_procs_completed; 

  // if (argc != 3) { 
  //   Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_shared_memory_page> <handle_to_page_mapped_semaphore>\n"); 
  //   Exit();
  // } 

  s_procs_completed = dstrtol(argv[1], NULL, 10);


  Printf("Consumer Created\n");

  // for(int i = 0; i < 11; i++)
  // {
  //   //Get lock
  //   //Consume resource
  //   //Release lock
  // }

  //Signal semaphore
  if(sem_signal(s_procs_completed) != SYNC_SUCCESS){
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); 
    Printf(argv[0]); 
    Printf(", exiting...\n");
    Exit();
  }



  return 0;
}