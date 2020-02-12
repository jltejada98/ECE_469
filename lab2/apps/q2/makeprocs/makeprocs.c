#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"
#include "spawn.h"
#include "utility.h"

// Jose and Josh make procs


int main (int argc, char *argv[]){
    int numprocs = 0;
    buffer *bp;
    uint32 h_mem;                   // Used to hold handle to shared memory page
    sem_t sem_procs_completed; // Semaphore used to wait until all spawned processes have completed
    lock_t buffer_lock; //Lock for buffer
    char h_mem_str[10];             // Used as command-line argument to pass mem_handle to new processes
    char sem_procs_completed_str[10]; // Used as command-line argument to pass page_mapped handle to new processes
    char buffer_lock_str[10];   //Used as command-line argument for lock
    int i;

    if (argc != 2) {
        Printf("Usage: "); Printf(argv[0]); Printf(" <number of Producers/Consumers to create>\n");
        Exit();
    }

    // Convert string from ascii command line argument to integer number
    numprocs = dstrtol(argv[1], NULL, 10) * 2; // the "10" means base 10, *2 because one producer and one consumer
    Printf("Creating %d processes\n", numprocs);


     // Allocate space for a shared memory page, which is exactly 64KB
    // Note that it doesn't matter how much memory we actually need: we
    // always get 64KB
    if ((h_mem = shmget()) == 0) {
        Printf("ERROR: could not allocate shared memory page in "); Printf(argv[0]); Printf(", exiting...\n");
        Exit();
    }

    bp = (buffer *)shmat(h_mem);
    if(bp == NULL){
      Printf("Could not map the shared page to virtual address in "); Printf(argv[0]); Printf(", exiting..\n");
      Exit();
    }

    sem_procs_completed = sem_create(-(numprocs - 1));
    if(sem_procs_completed == SYNC_FAIL){
      Printf("Bad sem_create in ");
      Printf(argv[0]);
      Printf("\n");
      Exit();
    }

    buffer_lock = lock_create();
    if(buffer_lock == SYNC_FAIL)
    {
        Printf("Bad LockCreate in ");
        Printf(argv[0]);
        Printf("\n");
        Exit();
    }

    ditoa(h_mem, h_mem_str);
    ditoa(sem_procs_completed, sem_procs_completed_str);
    ditoa(buffer_lock, buffer_lock_str);

    for(i = 0; i < (numprocs / 2); i++)
    {
        process_create(PRODUCER_FILENAME,sem_procs_completed_str, h_mem_str, buffer_lock_str, NULL);
        process_create(CONSUMER_FILENAME,sem_procs_completed_str, h_mem_str, buffer_lock_str, NULL);
    }

    if (sem_wait(sem_procs_completed) != SYNC_SUCCESS) {
      Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); Printf(argv[0]); Printf("\n");
      Exit();
    }

    Printf("Sucess\n");

    return 0;
}
