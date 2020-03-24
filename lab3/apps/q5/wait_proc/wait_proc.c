#include "usertraps.h"
#include "misc.h"
#include "utility.h"

int main(int argc, char const *argv[])
{
  sem_t sem_procs_completed;
  int num_sleep;
  int sleep_time;
  int i;

  if (argc < 3)
  {
    Printf("Incorrect number of arguments (%d) for %s, expecting at least 3.\n", argc, argv[0]);
    Exit();
  }

  sem_procs_completed = dstrtol(argv[1], NULL, 10);
  num_sleep = dstrtol(argv[2], NULL, 10);

  if (argc != 3 + num_sleep) { 
    Printf("Incorrect number of arguments (%d) for %s, expecting %d\n", argc, argv[0], 3 + num_sleep);
    Exit();
  } 

  for(i = 0; i < num_sleep; i++)
  {
    sleep_time = dstrtol(argv[3 + i], NULL, 10);
    sleep(sleep_time);
    //yield();

    Printf("Process %d slept %d time for %d seconds\n", getpid(), i+1, sleep_time);
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