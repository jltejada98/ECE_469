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
	mbox_t mbox_S, mbox_O2, mbox_SO4;
	int msg;
	int txMsg;


	if (argc != 6) { 
		Printf("Incorrect number of arguments (%d) for %s\n", argc, argv[0]);
	  Exit();
	} 

	sem_procs_completed = dstrtol(argv[1], NULL, 10);
	mbox_S = dstrtol(argv[2], NULL, 10);
	mbox_O2 = dstrtol(argv[3], NULL, 10);
	mbox_SO4 = dstrtol(argv[4], NULL, 10);
	numReact = dstrtol(argv[5], NULL, 10);

	if(mbox_open(mbox_S) == MBOX_FAIL){
		Printf("Could not open mbox_S (%d) in", mbox_S);
		Printf(argv[0]);
		Exit();
	}
	if(mbox_open(mbox_O2) == MBOX_FAIL){
		Printf("Could not open mbox_02 (%d) in", mbox_O2);
		Printf(argv[0]);
		Exit();
	}
	if(mbox_open(mbox_SO4) == MBOX_FAIL){
		Printf("Could not open mbox_S04 (%d) in", mbox_SO4);
		Printf(argv[0]);
		Exit();
	}

	i = 0;
	txMsg = 1;

	while(i < numReact)
	{

		msg = 0;
		if(mbox_recv(mbox_O2, sizeof(int), &msg) == MBOX_FAIL)
		{
			Printf("Bad mailbox recv in %s, PID: %d\nExiting...\n", argv[0], getpid());
			Exit();
		}
		if (msg != 1)
		{
			Printf("Error, incorrect message rx\nExiting...\n");
			Exit();
		}

		msg = 0;
		if(mbox_recv(mbox_S, sizeof(int), &msg) == MBOX_FAIL)
		{
			Printf("Bad mailbox recv in %s, PID: %d\nExiting...\n", argv[0], getpid());
			Exit();
		}
		if (msg != 1)
		{
			Printf("Error, incorrect message rx\nExiting...\n");
			Exit();
		}
		

		if(mbox_send(mbox_SO4, sizeof(int), &txMsg) == MBOX_FAIL)
		{
			Printf("Bad mailbox recv in %s, PID: %d\nExiting...\n", argv[0], getpid());
			Exit();
		}
		Printf("(%d) S + 2 O2 -> SO4 Reacted, PID: %d\n", i+1, getpid());
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