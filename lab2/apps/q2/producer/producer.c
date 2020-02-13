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
  int i;
  char resource[25] = "ABCDEFGHIJ";

  if (argc != 4) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_shared_memory_page> <handle_to_page_mapped_semaphore>\n"); 
    Exit();
  } 

  sem_procs_completed = dstrtol(argv[1], NULL, 10);
  h_mem = dstrtol(argv[2], NULL, 10);
  buffer_lock = dstrtol(argv[3], NULL, 10);


  cb = (buffer*) shmat(h_mem);
  if(cb == NULL)
  {
    Printf("Could not map virtual address to memory in ");
    Printf(argv[0]);
    Exit();
  }

  lock_acquire(buffer_lock); //Changed lock outside of for loop.
  for(i = 0; i < 11; i++)
  {
    if(!  ((cb->start + 1) % BUFFER_SIZE == cb->end) ) //Buffer not full
    {
      cb->end = (cb->end + 1) % BUFFER_SIZE;
      cb->data[cb->end] = resource[i];
      Printf("Producer %d inserted: %c %c\n", getpid(), resource[i], cb->data[cb->end]);
      Printf("CB:(%2d),(%2d)\n", cb->start, cb->end);
    }
  }
  lock_release(buffer_lock);

  //Signal semaphore
  if(sem_signal(sem_procs_completed) != SYNC_SUCCESS){
    Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); 
    Printf(argv[0]); 
    Printf(", exiting...\n");
    Exit();
  }



  return 0;
}