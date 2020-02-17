#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"
#include "utility.h"

int main(int argc, char const *argv[])
{
  
  sem_t sem_procs_sleeping;
  atm *a; 
  lock_t atm_lock;
  uint32 h_mem;                   // Used to hold handle to shared memory page
  int num_molecules;
  int i;

  if (argc != 4) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_shared_memory_page> <handle_to_page_mapped_semaphore>\n"); 
    Exit();
  } 

  sem_procs_sleeping = dstrtol(argv[1], NULL, 10);
  h_mem = dstrtol(argv[2], NULL, 10);
  atm_lock = dstrtol(argv[3], NULL, 10);
  num_molecules = dstrtol(argv[4], NULL, 10);


  a = (atm*) shmat(h_mem);
  if(a == NULL)
  {
    Printf("Could not map virtual address to memory in ");
    Printf(argv[0]);
    Exit();
  }

  i = 0;
  //Consider checking start pointer
  while(i < num_molecules){
    lock_acquire(atm_lock); //Changed lock outside of for loop.
    if(!((cb->start + 1) % BUFFER_SIZE == cb->end)) //Buffer not full
    {
      cb->data[cb->end] = resource[i];
      Printf("Producer %d inserted: %c\n", getpid(), resource[i]);
      cb->end = (cb->end + 1) % BUFFER_SIZE;
      i++;
    }
    lock_release(atm_lock);
  }

  //Signal semaphore
  if(sem_signal(sem_procs_sleeping) != SYNC_SUCCESS){
    Printf("Bad semaphore sem_procs_sleeping (%d) in ", sem_procs_sleeping); 
    Printf(argv[0]); 
    Printf(", exiting...\n");
    Exit();
  }



  return 0;
}