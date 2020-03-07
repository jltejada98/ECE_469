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

	int numS, numO2;

	if (argc != 6) { 
	  Printf("Incorrect Arguments for %s", argv[0]);
	  Exit();
	} 

	sem_procs_completed = dstrtol(argv[1], NULL, 10);
	mbox_S = dstrtol(argv[2], NULL, 10);
	mbox_O2 = dstrtol(argv[3], NULL, 10);
	mbox_SO4 = dstrtol(argv[4], NULL, 10);
	numReact = dstrtol(argv[6], NULL, 10);

	mbox_open(mbox_S);
	mbox_open(mbox_O2);
	mbox_open(mbox_SO4);

	i = 0;
	numS = 0;
	numO2 = 0;
	txMsg = 1;

	while(i < numReact)
	{
		msg = 0;
		if(mbox_recv(mbox_S, sizeof(int), &msg) == MBOX_FAIL)
		{
			Printf("Bad mailbox recv in %s, PID: %d\nExiting...\n" argv[0], getpid());
			Exit();
		}
		if (msg != 1)
		{
			Printf("Error, incorrect message rx\nExiting...\n");
			Exit();
		}
		numS++;

		msg = 0;
		if(mbox_recv(mbox_O2, sizeof int, &msg) == MBOX_FAIL)
		{
			Printf("Bad mailbox recv in %s, PID: %d\nExiting...\n" argv[0], getpid());
			Exit();
		}
		if (msg != 1)
		{
			Printf("Error, incorrect message rx\nExiting...\n");
			Exit();
		}
		numO2++;

		if(numO2 >= 2 && numS >= 1)
		{
			if(mbox_send(mbox_SO4, sizeof int, &txMsg) == MBOX_FAIL)
			{
				Printf("Bad mailbox recv in %s, PID: %d\nExiting...\n" argv[0], getpid());
				Exit();
			}
			Printf("(%d) S + 2 O2 -> SO4 Reacted, PID: %d", i, getpid());
			i++;
		}

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