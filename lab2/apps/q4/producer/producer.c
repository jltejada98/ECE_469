#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"
#include "utility.h"
#include "spawn.h"

int main(int argc, char const *argv[])
{
  buffer *cb;
  sem_t sem_procs_completed; 
  lock_t buffer_lock;
  uint32 h_mem;                   // Used to hold handle to shared memory page
  cond_t cond_not_empty;
  cond_t cond_not_full;
  int i;
  char resource[11] = "Hello World";

  if (argc != 6) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_shared_memory_page> <handle_to_page_mapped_semaphore>\n"); 
    Exit();
  } 

  sem_procs_completed = dstrtol(argv[1], NULL, 10);
  h_mem = dstrtol(argv[2], NULL, 10);
  buffer_lock = dstrtol(argv[3], NULL, 10);
  cond_not_empty = dstrtol(argv[4], NULL, 10);
  cond_not_full = dstrtol(argv[5], NULL, 10);


  cb = (buffer*) shmat(h_mem);
  if(cb == NULL)
  {
    Printf("Could not map virtual address to memory in ");
    Printf(argv[0]);
    Exit();
  }

  for(i=0; i<11; i++){

    lock_acquire(buffer_lock); //Changed lock outside of for loop.
    while(cb->start == cb->end && !cb->empty) //Buffer full
    {
      cond_wait(cond_not_full);
    }  

    //Produce resource
    cb->data[cb->end] = resource[i];
    Printf("Producer %d inserted: %c\n", getpid(), resource[i]);
    cb->end = (cb->end + 1) % BUFFER_SIZE;
    cb->empty = 0;
    
    cond_signal(cond_not_empty);

    lock_release(buffer_lock);
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