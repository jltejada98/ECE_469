#include "usertraps.h"
#include "misc.h"

#define part1 "part1.dlx.obj"
#define part2 "part2.dlx.obj"
#define part3 "part3.dlx.obj"
#define part4 "part4.dlx.obj"
#define part5 "part5.dlx.obj"
#define part6 "part6.dlx.obj"
#define HELLO_WORLD "hello_world.dlx.obj"

void main (int argc, char *argv[])
{
  int part_num = 0;             // Used to store number of processes to create
  int i;                               // Loop index variable
  int num_proccess = 0;
  sem_t s_procs_completed;             // Semaphore used to wait until all spawned processes have completed
  char s_procs_completed_str[10];      // Used as command-line argument to pass page_mapped handle to new processes
  int num_hello_world = 5;

  if (argc != 2) {
    Printf("Usage: %s <number of hello world processes to create>\n", argv[0]);
    Exit();
  }

  Printf("Makeprocs started!\n");

  // Convert string from ascii command line argument to integer number
  part_num = dstrtol(argv[1], NULL, 10); // the "10" means base 10
  Printf("makeprocs running part %d\n", part_num);

  switch(part_num) { //Number of processes to make depending in part
    case 0: num_proccess = 30;   break;
    case -1: num_proccess = 1;   break;
    case 1: num_proccess = 1;    break;
    case 2: num_proccess = 1;    break;
    case 3: num_proccess = 1;    break;
    case 4: num_proccess = 1;    break;
    case 5: num_proccess = 1;  break;
    case 6: num_proccess = 30;   break;
  }

  // Create semaphore to not exit this process until all other processes 
  // have signalled that they are complete.
  if ((s_procs_completed = sem_create(-(num_proccess-1))) == SYNC_FAIL) {
    Printf("makeprocs (%d): Bad sem_create\n", getpid());
    Exit();
  }

  // Setup the command-line arguments for the new processes.  We're going to
  // pass the handles to the semaphore as strings
  // on the command line, so we must first convert them from ints to strings.
  ditoa(s_procs_completed, s_procs_completed_str);

  if(part_num == 6 || part_num == 0) {
    Printf("-------------------------------------------------------------------------------------\n");
    Printf("makeprocs (%d): part %d\n", getpid(), part_num);
    for(i = 0; i < 30; i++) {
      process_create(part6, s_procs_completed_str, NULL);
    }

    if (sem_wait(s_procs_completed) != SYNC_SUCCESS)
    {
			Printf("Bad semaphore s_procs_completed (%d) in %s\n", s_procs_completed, argv[0]);
			Exit();
		}
  }
  if(part_num == -1){
    // Create Hello World processes
    Printf("-------------------------------------------------------------------------------------\n");
    Printf("makeprocs (%d): Creating %d hello world's in a row, but only one runs at a time\n", getpid(), num_hello_world);
    for(i=0; i<num_hello_world; i++) {
      Printf("makeprocs (%d): Creating hello world #%d\n", getpid(), i);
      process_create(HELLO_WORLD, s_procs_completed_str, NULL);
      if (sem_wait(s_procs_completed) != SYNC_SUCCESS) {
        Printf("Bad semaphore s_procs_completed (%d) in %s\n", s_procs_completed, argv[0]);
        Exit();
      }
    }
  }
  if(part_num == 1 || part_num == 0){
    Printf("-------------------------------------------------------------------------------------\n");
		Printf("makeprocs (%d): part %d\n", getpid(), part_num);
		process_create(part1, s_procs_completed_str, NULL);
    if (sem_wait(s_procs_completed) != SYNC_SUCCESS)
    {
			Printf("Bad semaphore s_procs_completed (%d) in %s\n", s_procs_completed, argv[0]);
			Exit();
		}
  }
  if(part_num == 0) {
		Printf("-------------------------------------------------------------------------------------\n");
		Printf("Cannot run part 2 with all other parts because this causes a memory access error which invokes interrupt handler and calls exitsim()\n");
  }
	if(part_num == 2) {
		Printf("-------------------------------------------------------------------------------------\n");
		Printf("makeprocs (%d): part %d\n", getpid(), part_num);
		process_create(part2, s_procs_completed_str, NULL);
		if (sem_wait(s_procs_completed) != SYNC_SUCCESS)
    {
			Printf("Bad semaphore s_procs_completed (%d) in %s\n", s_procs_completed, argv[0]);
			Exit();
		}
	} 
  if(part_num == 0) {
		Printf("-------------------------------------------------------------------------------------\n");
		Printf("Cannot run part 3 with all other parts because this causes a memory access error which invokes interrupt handler and calls exitsim()\n");
  }
  if(part_num == 3) {
    Printf("-------------------------------------------------------------------------------------\n");
    Printf("makeprocs (%d): part %d\n", getpid(), part_num);
    process_create(part3, s_procs_completed_str, NULL);
    if (sem_wait(s_procs_completed) != SYNC_SUCCESS)
    {
			Printf("Bad semaphore s_procs_completed (%d) in %s\n", s_procs_completed, argv[0]);
			Exit();
		}
  } 
  if(part_num == 4 || part_num == 0) {
    Printf("-------------------------------------------------------------------------------------\n");
    Printf("makeprocs (%d): part %d\n", getpid(), part_num);
    process_create(part4, s_procs_completed_str, NULL);
    if (sem_wait(s_procs_completed) != SYNC_SUCCESS)
    {
			Printf("Bad semaphore s_procs_completed (%d) in %s\n", s_procs_completed, argv[0]);
			Exit();
		}
  } 
  if(part_num == 5 || part_num == 0) {
    Printf("-------------------------------------------------------------------------------------\n");
    Printf("makeprocs (%d): part %d\n", getpid(), part_num);
    for(i = 0; i < 100; i++) {
      process_create(part5, s_procs_completed_str, NULL);

      if (sem_wait(s_procs_completed) != SYNC_SUCCESS)
      {
				Printf("Bad semaphore s_procs_completed (%d) in %s\n", s_procs_completed, argv[0]);
				Exit();
			}
    }
  } 
  Printf("-------------------------------------------------------------------------------------\n");
	Printf("makeprocs (%d): All other processes completed, exiting main process.\n", getpid());

}
