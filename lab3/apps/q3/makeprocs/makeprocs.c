#include "usertraps.h"
#include "misc.h"
#include "utility.h"

int min2(int a, int b);

int main (int argc, char *argv[]){

    sem_t sem_procs_completed;
    char sem_procs_completed_str[10]; // Used as command-line argument to pass page_mapped handle to new processes
    char small_num_str[10];
    char med_num_str[10];
    char large_num_str[10];

    if (argc != 1) {
        Printf(argv[0]); Printf(" should not have any arguments.\n");
        Exit();
    }

    sem_procs_completed = sem_create(3);

    //Error check semaphore
    if(sem_procs_completed == SYNC_FAIL)
    {
        Printf("Bad sem_create in ");
        Printf(argv[0]);
        Printf("\n");
        Exit();
    }

    ditoa(100, small_num_str);
    ditoa(10000, med_num_str);
    ditoa(1000000, large_num_str);


    //All of process creation
    process_create(PROC_FILENAME, 0, 0, small_num_str, NULL);
    process_create(PROC_FILENAME, 0, 0, med_num_str, NULL);
    process_create(PROC_FILENAME, 0, 0, large_num_str, NULL);

    if (sem_wait(sem_procs_completed) != SYNC_SUCCESS) {
      Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); Printf(argv[0]); Printf("\n");
      Exit();
    }

    return 0;
}

