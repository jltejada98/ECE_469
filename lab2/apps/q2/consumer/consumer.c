#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"
#include "spawn.h"
#include "utility.h"


//Arguments
//semaphore, shared_memory


int main(int argc, char const *argv[])
{
  //circular_buffer *cb
  sem_t sem_procs_completed; 

  if (argc != 3) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_shared_memory_page> <handle_to_page_mapped_semaphore>\n"); 
    Exit();
  } 

  sem_procs_completed = dstrtol(argv[1], NULL, 10);
  h_mem = dstrtol(argv[2], NULL, 10);

  cb = (buffer*) shmat(h_mem);
  if(cb == NULL)
  {
    Printf("Could not map virtual address to memory in ");
    Printf(argv[0]);
    Exit();
  }

  // for(int i = 0; i < 11; i++)
  // {
  //   //Get lock
  //   //Consume resource
  //   //Release lock
  // }

  //Signal semaphore
  Printf("Consumer Signaling Complete\n");
  if(sem_signal(sem_procs_completed) != SYNC_SUCCESS){
    Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); 
    Printf(argv[0]); 
    Printf(", exiting...\n");
    Exit();
  }



  return 0;
}