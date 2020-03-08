  #include "usertraps.h"
  #include "misc.h"
  #include "utility.h"

  int main(int argc, char const *argv[])
  {
  sem_t sem_procs_completed; 
  int num_to_create;
  mbox_t mbox_S;
  int msg;
  int i;

  if (argc != 4) { 
    Printf("Incorrect args (%d) for: ", argc); Printf(argv[0]); Printf("\n");
    Printf("Arguments included were...\n");
    for(i = 0; i < argc; i++)
    {
      Printf("%d) %d\n", i, argv[i][0]);
    }
    Exit();
  } 

  sem_procs_completed = dstrtol(argv[1], NULL, 10);
  num_to_create = dstrtol(argv[2], NULL, 10);
  mbox_S = dstrtol(argv[3], NULL, 10);

  mbox_open(mbox_S);

  msg = 1;
  for(i = 0; i < num_to_create; i++)
  {
    mbox_send(mbox_S, sizeof(int), &msg);
  }

  mbox_close(mbox_S);

  //Signal complete semaphore
  if(sem_signal(sem_procs_completed) != SYNC_SUCCESS){
    Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); 
    Printf(argv[0]); 
    Printf(", exiting...\n");
    Exit();
  }



  return 0;
  }