	#include "usertraps.h"
	#include "misc.h"
	#include "utility.h"

	int main(int argc, char const *argv[])
	{
	sem_t sem_procs_completed; 
	int num_to_create;
	mbox_t mbox_CO;
	int msg;
	int i;

	if (argc != 4) { 
		Printf("Incorrect args (%d) for: ", argc); Printf(argv[0]); Printf("\n");
		Exit();
	} 

	sem_procs_completed = dstrtol(argv[1], NULL, 10);
	num_to_create = dstrtol(argv[2], NULL, 10);
	mbox_CO = dstrtol(argv[3], NULL, 10);

	mbox_open(mbox_CO);

	msg = 1;
	for(i = 0; i < num_to_create; i++)
	{
		mbox_send(mbox_CO, sizeof(int), &msg);
		Printf("CO injected into Radeon atmosphere, PID: %d \n", getpid());
	}

	mbox_close(mbox_CO);

	//Signal complete semaphore
	if(sem_signal(sem_procs_completed) != SYNC_SUCCESS){
		Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); 
		Printf(argv[0]); 
		Printf(", exiting...\n");
		Exit();
	}



	return 0;
	}