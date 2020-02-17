#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"
#include "spawn.h"
#include "utility.h"


int main (int argc, char *argv[]){
    int numprocs = 5;
    lock_t atm_lock; //Lock for atmospehere
    sem_t sem_procs_sleeping; // Semaphore used to wait until all spawned processes have completed
    atm a; //Atmosphere data structure
    int num_water = 0;
    int num_sulfate = 0;
    uint32 h_mem;                   // Used to hold handle to shared memory page
    char atm_lock_str[10];   //Used as command-line argument for lock
    char h_mem_str[10];             // Used as command-line argument to pass mem_handle to new processes
    char sem_procs_sleeping_str[10]; // Used as command-line argument to pass page_mapped handle to new processes
    int i;

    if (argc != 2) {
        Printf("Usage: "); Printf(argv[0]); Printf(" <number of Producers/Consumers to create>\n");
        Exit();
    }

    // Convert string from ascii command line argument to integer number
    num_water = dstrtol(argv[1], NULL, 10); // the "10" means base 10
    num_sulfate = dstrtol(argv[2], NULL, 10); // the "10" means base 10


     // Allocate space for a shared memory page, which is exactly 64KB
    // Note that it doesn't matter how much memory we actually need: we
    // always get 64KB
    if ((h_mem = shmget()) == 0) {
        Printf("ERROR: could not allocate shared memory page in "); Printf(argv[0]); Printf(", exiting...\n");
        Exit();
    }

    a = (atm *)shmat(h_mem);
    if(bp == NULL){
      Printf("Could not map the shared page to virtual address in "); Printf(argv[0]); Printf(", exiting..\n");
      Exit();
    }
    a->water = 0;
    a->h = 0;
    a->o = 0;
    a->sulfate = 0;
    a->h_sulfate = 0;


    sem_procs_sleeping = sem_create(-(numprocs - 1));
    if(sem_procs_sleeping == SYNC_FAIL){
      Printf("Bad sem_create in ");
      Printf(argv[0]);
      Printf("\n");
      Exit();
    }

    atm_lock = lock_create();
    if(atm_lock == SYNC_FAIL)
    {
        Printf("Bad LockCreate in ");
        Printf(argv[0]);
        Printf("\n");
        Exit();
    }

    ditoa(h_mem, h_mem_str);
    ditoa(sem_procs_sleeping, sem_procs_sleeping_str);
    ditoa(atm_lock, atm_lock_str);


    process_create(PRODUCER_FILENAME,sem_procs_sleeping_str, h_mem_str, atm_lock_str, NULL);
    process_create(CONSUMER_FILENAME,sem_procs_sleeping_str, h_mem_str, atm_lock_str, NULL);
    

    if (sem_wait(sem_procs_sleeping) != SYNC_SUCCESS) {
      Printf("Bad semaphore sem_procs_sleeping (%d) in ", sem_procs_sleeping); Printf(argv[0]); Printf("\n");
      Exit();
    }

    return 0;
}
