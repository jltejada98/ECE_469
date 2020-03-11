#include "usertraps.h"
#include "misc.h"
#include "utility.h"


//Arguments
//semaphore, shared_memory


int main(int argc, char const *argv[])
{
  sem_t sem_procs_completed;
  mbox_t mbox_CO, mbox_O2, mbox_C2;
  int msg;
  int txMsg;
  int numReact;
  int i;
  int j;


  if (argc != 6) { 
    Printf("Incorrect number of arguments (%d) for %s\n", argc, argv[0]);
    Exit();
  } 

  sem_procs_completed = dstrtol(argv[1], NULL, 10);
  mbox_CO = dstrtol(argv[2], NULL, 10);
  mbox_O2 = dstrtol(argv[3], NULL, 10);
  mbox_C2 = dstrtol(argv[4], NULL, 10);
  numReact = dstrtol(argv[5], NULL, 10);

    if(mbox_open(mbox_CO) == MBOX_FAIL){
    Printf("Could not open mbox_CO (%d) in", mbox_CO);
    Printf(argv[0]);
    Exit();
  }
  if(mbox_open(mbox_O2) == MBOX_FAIL){
    Printf("Could not open mbox_02 (%d) in", mbox_O2);
    Printf(argv[0]);
    Exit();
  }
  if(mbox_open(mbox_C2) == MBOX_FAIL){
    Printf("Could not open mbox_C2 (%d) in", mbox_C2);
    Printf(argv[0]);
    Exit();
  }

  txMsg = 1;
  i = 0;

  while(i < numReact)
  {
    msg = 0;
    for(j = 0; j < 4; j++)
    {
      if(mbox_recv(mbox_CO, sizeof(int), &msg) == MBOX_FAIL)
      {
        Printf("Bad mailbox recv in %s, PID: %d\nExiting...\n", argv[0], getpid());
        Exit();
      }
      if (msg != 1)
      {
        Printf("Error, incorrect message rx\nExiting...\n");
        Exit();
      }
    }

    for(j = 0; j < 2; j++)
    {
      if(mbox_send(mbox_O2, sizeof(int), &txMsg) == MBOX_FAIL)
      {
        Printf("Bad mailbox recv in %s, PID: %d\nExiting...\n", argv[0], getpid());
        Exit();
      }
    }

    for(j = 0; j < 2; j++)
    {
      if(mbox_send(mbox_C2, sizeof(int), &txMsg) == MBOX_FAIL)
      {
        Printf("Bad mailbox recv in %s, PID: %d\nExiting...\n", argv[0], getpid());
        Exit();
      }
    }
    Printf("(%d) 4 CO -> 2 O2 + 2 C2 Reacted, PID: %d\n", i+1, getpid());
    i++;
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