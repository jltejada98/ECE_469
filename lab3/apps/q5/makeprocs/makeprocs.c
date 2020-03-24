#include "usertraps.h"
#include "misc.h"
#include "utility.h"


int main (int argc, char *argv[]){

    sem_t sem_procs_completed;
    char sem_procs_completed_str[10]; // Used as command-line argument to pass page_mapped handle to new processes

    if (argc != 1) {
        Printf(argv[0]); 
        Printf(" should not have any arguments.\n");
        Exit();
    }

    sem_procs_completed = sem_create(-1);

    //Error check semaphore
    if(sem_procs_completed == SYNC_FAIL)
    {
        Printf("Bad sem_create in ");
        Printf(argv[0]);
        Printf("\n");
        Exit();
    }

    ditoa(sem_procs_completed, sem_procs_completed_str);

    Printf("Starting nothing_procs...\n");

    //All of process creation
    process_create(IDLE_PROC_NAME, 0, 0, sem_procs_completed_str, NULL);

    if (sem_wait(sem_procs_completed) != SYNC_SUCCESS) {
      Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); Printf(argv[0]); Printf("\n");
      Exit();
    }

    return 0;
}

