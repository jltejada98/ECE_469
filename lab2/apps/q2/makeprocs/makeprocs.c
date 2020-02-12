#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"
#include "spawn.h"
#include "utility.h"

int main (int argc, char *argv[]){
    int numprocs = 0;
    buffer *bp;
    uint32 h_mem;                   // Used to hold handle to shared memory page
    sem_t s_procs_completed; // Semaphore used to wait until all spawned processes have completed
    char h_mem_str[10];             // Used as command-line argument to pass mem_handle to new processes
    char s_procs_completed_str[10]; // Used as command-line argument to pass page_mapped handle to new processes

    if (argc != 2) {
        Printf("Usage: "); Printf(argv[0]); Printf(" <number of Producers/Consumers to create>\n");
        Exit();
    }

    // Convert string from ascii command line argument to integer number
    numprocs = dstrtol(argv[1], NULL, 10); // the "10" means base 10
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

    // ditoa(h_mem, h_mem_str);

    process_create() //Add agruments


    Printf("Sucess\n");

    return 0;
}
