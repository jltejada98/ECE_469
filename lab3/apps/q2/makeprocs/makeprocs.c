#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"
#include "utility.h"

int min2(int a, int b);

int main (int argc, char *argv[]){
    int numprocs = 5;

    int numReact1;
    int numReact2;
    int numReact3;
    
    sem_t sem_procs_completed; // keeps track if all child processes are sleeping

    //Mailboxes
    mbox_t mbox_S2;
    mbox_t mbox_S;
    mbox_t mbox_CO;
    mbox_t mbox_O2;
    mbox_t mbox_C2;
    mbox_t mbox_SO4;

    int num_S2_init = 0;
    int num_CO_init = 0;
    
    char num_S2_init_str[10];
    char num_CO_init_str[10];

    char sem_procs_completed_str[10]; // Used as command-line argument to pass page_mapped handle to new processes

    char numReact1_str[10];
    char numReact2_str[10];
    char numReact3_str[10];


    if (argc != 3) {
        Printf("Usage: "); Printf(argv[0]); Printf(" <number of H2O molecules> <number of SO4 molecules>\n");
        Exit();
    }

    // Convert string from ascii command line argument to integer number
    num_S2_init = dstrtol(argv[1], NULL, 10); // the "10" means base 10
    num_CO_init = dstrtol(argv[2], NULL, 10); // the "10" means base 10

    numReact1 = num_S2_init;
    numReact2 = num_CO_init / 4; 
    numReact3 = min2(numReact1 * 2, numReact2);   //Find minimum of H2, O2, and S04 molecules

    //Initalize semaphore
    sem_procs_completed = sem_create(-(numprocs - 1));

    //Error check semaphore
    if(sem_procs_completed == SYNC_FAIL)
    {
        Printf("Bad sem_create in ");
        Printf(argv[0]);
        Printf("\n");
        Exit();
    }

    //Create mailboxes
    mbox_S2 = mbox_create();
    mbox_S = mbox_create();
    mbox_CO = mbox_create();
    mbox_O2 = mbox_create();
    mbox_C2 = mbox_create();
    mbox_SO4 = mbox_create();

    if (mbox_S2 == MBOX_FAIL ||
        mbox_S == MBOX_FAIL ||
        mbox_CO == MBOX_FAIL ||
        mbox_O2 == MBOX_FAIL ||
        mbox_C2 == MBOX_FAIL ||
        mbox_SO4 == MBOX_FAIL)
    {
        Printf("Bad mbox_create in ");
        Printf(argv[0]);
        Printf("\n");
        Exit();
    }


    //Convert command line arguments to strings
    ditoa(num_S2_init, num_S2_init_str);
    ditoa(num_CO_init, num_CO_init_str);

    ditoa(sem_procs_completed, sem_procs_completed_str);

    ditoa(numReact1, numReact1_str);
    ditoa(numReact2, numReact2_str);
    ditoa(numReact3, numReact3_str);

    //All of process creation
    process_create(PRODUCER_FILENAME_1,sem_procs_completed_str, num_S2_init_str, sem_S2_str, NULL);
    process_create(PRODUCER_FILENAME_2,sem_procs_completed_str, num_CO_init_str, sem_CO_str, NULL);
    process_create(REACT_FILENAME_1,sem_procs_completed_str, sem_S2_str, sem_h_str, sem_o_str, numReact1_str, NULL);
    process_create(REACT_FILENAME_2,sem_procs_completed_str, sem_CO_str, sem_s02_str, sem_o_str, numReact2_str, NULL);
    process_create(REACT_FILENAME_3,sem_procs_completed_str, sem_h_str, sem_o_str, sem_s02_str, sem_h_CO_str, numReact3_str, NULL);


    if (sem_wait(sem_procs_completed) != SYNC_SUCCESS) {
      Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); Printf(argv[0]); Printf("\n");
      Exit();
    }


    Printf("%d S2's left over. ", num_S2_init - numReact1);
    Printf("%d S's left over. ", (2*numReact1) - numReact3);
    Printf("%d CO's left over. ", num_CO_init - numReact2);
    Printf("%d O2's left over. ", (2*numReact2) - (2*numReact3));
    Printf("%d C2's created. \n", (2*numReact2));
    Printf("%d SO4's created. \n", (numReact3));

    return 0;
}

int min2(int a, int b){
    if(a < b)
        return a;
    return b;
}
