#include "usertraps.h"
#include "misc.h"
#include "utility.h"

//Arguments
//semaphore, shared_memory

int main(int argc, char const *argv[])
{
  sem_t sem_procs_completed;
  int numReact;
  int i;
  mbox_t mmbox_S2, mmbox_S;
  int msg;
  int txMsg;
  int numS;

  if (argc != 4) { 
    Printf("Incorrect Arguments for %s\n", argv[0]);
    Exit();
  } 


  sem_procs_completed = dstrtol(argv[1], NULL, 10);
  mmbox_S2 = dstrtol(argv[2], NULL, 10);
  mmbox_S = dstrtol(argv[3], NULL, 10);
  numReact = dstrtol(argv[4], NULL, 10);
  

  i = 0;
  txMsg = 1;

  while(i <  numReact)
  {
    msg = 0;
    if(mbox_recv(mmbox_S2, sizeof(int), &msg) == MBOX_FAIL)
    {
      Printf("Bad mailbox recv in %s, PID: %d\nExiting...\n" argv[0], getpid());
      Exit();
    }
    if (msg != 1)
    {
      Printf("Error, incorrect message rx\nExiting...\n");
      Exit();
    }

    if(mbox_send(mmbox_S, sizeof int, &txMsg) == MBOX_FAIL)
    {
      Printf("Bad mailbox recv in %s, PID: %d\nExiting...\n" argv[0], getpid());
      Exit();
    }
    if(mbox_send(mmbox_S, sizeof int, &txMsg) == MBOX_FAIL)
    {
      Printf("Bad mailbox recv in %s, PID: %d\nExiting...\n" argv[0], getpid());
      Exit();
    }

    Printf("S2 -> S + S reacted, PID: %d\n", getpid());
    ++i;
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