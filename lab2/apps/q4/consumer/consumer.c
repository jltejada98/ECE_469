#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"
#include "spawn.h"
#include "utility.h"


//Arguments
//semaphore, shared_memory


int main(int argc, char const *argv[])
{
  buffer *cb;
  sem_t sem_procs_completed; 
  lock_t buffer_lock;
  uint32 h_mem;                   // Used to hold handle to shared memory page
  cond_t cond_not_empty;
  cond_t cond_not_full;
  int bufferWasFull;
  int i;

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

  bufferWasFull = 0;
  for(i = 0; i < 11; i++)
  {
    Printf("Consumer Requesting Lock\n");
    lock_acquire(buffer_lock);
    Printf("Consumer Aquired Lock\n");
    if(cb->start == cb->end) //Buffer empty
    {
      Printf("Consumer waiting for not empty\n");
      cond_wait(cond_not_empty);
      Printf("Consumer rx not empty\n");
    }

    if((cb->start + 1) % BUFFER_SIZE == cb->end) //Buffer full
      bufferWasFull = 1;
    else
      bufferWasFull = 0;
    
    //Consume resource
    Printf("Consumer %d removed: %c\n", getpid(), cb->data[cb->start]);
    cb->start = (cb->start + 1) % BUFFER_SIZE;

    if(bufferWasFull)
      cond_signal(cond_not_full);
    
    Printf("Consumer Releasing Lock\n");
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