  #include "usertraps.h"
  #include "misc.h"
  #include "utility.h"

  int main(int argc, char const *argv[])
  {
  sem_t sem_procs_completed; 
  int num_to_create;
  mbox_t mbox_S2;
  int msg;
  int i;

  if (argc != 4) { 
    Printf("Incorrect args (%d) for: ", argc); Printf(argv[0]); Printf("\n");
    Printf("Arguments included:\n");
    for(i = 0; i < argc; i++)
    {
      Printf("%d) %s\n", i, argv[i]);
    }
    Exit();
  } 

  sem_procs_completed = dstrtol(argv[1], NULL, 10);
  num_to_create = dstrtol(argv[2], NULL, 10);
  mbox_S2 = dstrtol(argv[3], NULL, 10);

  mbox_open(mbox_S2);

  msg = 1;
  for(i = 0; i < num_to_create; i++)
  {
    Printf("Sending mbox_S2: %d\n", mbox_S2);
    mbox_send(mbox_S2, sizeof(int), &msg);
    Printf("S2 injected into Radeon atmosphere, PID: %d \n", getpid());
  }

  mbox_close(mbox_S2);

  //Signal complete semaphore
  if(sem_signal(sem_procs_completed) != SYNC_SUCCESS){
    Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); 
    Printf(argv[0]); 
    Printf(", exiting...\n");
    Exit();
  }



  return 0;
  }