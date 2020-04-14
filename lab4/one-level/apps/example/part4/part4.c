#include "usertraps.h"
#include "misc.h"

int stack(int x){
	if(x == 0){
		return 0;
	}
	return (stack(x-1) + 1);
}

void main (int argc, char *argv[])
{
  sem_t s_procs_completed; // Semaphore to signal the original process that we're done
  int num_recursive_calls = 100;
  int last_call;

  if (argc != 2) { 
    Printf("Usage: %s <handle_to_procs_completed_semaphore>\n"); 
    Exit();
  } 

  s_procs_completed = dstrtol(argv[1], NULL, 10);

 
  Printf("part4 : testing user function call stack to grow larger than 1 page\n");

  last_call = stack(num_recursive_calls);

  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("part4 : Bad semaphore s_procs_completed !\n", s_procs_completed);
    Exit();
  }
  Printf("part4 : done!\n");
}
