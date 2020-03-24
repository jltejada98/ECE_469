#include "usertraps.h"
#include "misc.h"
#include "utility.h"


int main (int argc, char *argv[]){

    sem_t sem_procs_completed;
    char sem_procs_completed_str[10]; // Used as command-line argument
    char num_sleeps[10]; // Used as command-line argument

    char sleep_time1[10]; // Used as command-line argument
    char sleep_time2[10]; // Used as command-line argument
    char sleep_time3[10]; // Used as command-line argument
    char sleep_time4[10]; // Used as command-line argument

    if (argc != 1) {
        Printf(argv[0]); 
        Printf(" should not have any arguments.\n");
        Exit();
    }

    sem_procs_completed = sem_create(0);

    //Error check semaphore
    if(sem_procs_completed == SYNC_FAIL)
    {
        Printf("Bad sem_create in ");
        Printf(argv[0]);
        Printf("\n");
        Exit();
    }

    ditoa(sem_procs_completed, sem_procs_completed_str);

    ditoa(3, num_sleeps);

    ditoa(0, sleep_time1);
    ditoa(1, sleep_time2);
    ditoa(2, sleep_time3);

    //process creation
    process_create(SLEEP_PROC_NAME, 0, 0, sem_procs_completed_str, num_sleeps, sleep_time1, sleep_time2, sleep_time3, NULL);

    if (sem_wait(sem_procs_completed) != SYNC_SUCCESS) {
      Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); Printf(argv[0]); Printf("\n");
      Exit();
    }

    printf("Exiting makeprocs\n");

    return 0;
}

