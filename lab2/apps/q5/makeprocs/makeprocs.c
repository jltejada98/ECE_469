#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"
#include "utility.h"

int min3(int a, int b, int c);

int main (int argc, char *argv[]){
    int numprocs = 5;

    int numReact1;
    int numReact2;
    int numReact3;
    
    sem_t sem_procs_completed; // keeps track if all child processes are sleeping
    sem_t sem_water;
    sem_t sem_h;
    sem_t sem_o;
    sem_t sem_sulfate;
    sem_t sem_h_sulfate;
    sem_t sem_s02;

    int num_water_init = 0;
    int num_sulfate_init = 0;
    
    char num_water_init_str[10];
    char num_sulfate_init_str[10];

    char sem_procs_completed_str[10]; // Used as command-line argument to pass page_mapped handle to new processes
    char sem_water_str[10];
    char sem_h_str[10];
    char sem_o_str[10];
    char sem_sulfate_str[10];
    char sem_h_sulfate_str[10];
    char sem_s02_str[10];

    char numReact1_str[10];
    char numReact2_str[10];
    char numReact3_str[10];


    if (argc != 3) {
        Printf("Usage: "); Printf(argv[0]); Printf(" <number of H2O molecules> <number of SO4 molecules>\n");
        Exit();
    }

    // Convert string from ascii command line argument to integer number
    num_water_init = dstrtol(argv[1], NULL, 10); // the "10" means base 10
    num_sulfate_init = dstrtol(argv[2], NULL, 10); // the "10" means base 10

    numReact1 = num_water_init / 2;
    numReact2 = num_sulfate_init;
    numReact3 = min3(numReact1 * 2, numReact1 + numReact2, numReact2);   //Find minimum of H2, O2, and S04 molecules

    //Initalize all the semaphores
    sem_procs_completed = sem_create(-(numprocs - 1));
    sem_water = sem_create(0);
    sem_h = sem_create(0);
    sem_o = sem_create(0);
    sem_sulfate = sem_create(0);
    sem_h_sulfate = sem_create(0);
    sem_s02 = sem_create(0);

    //Error check all the semaphores
    if(sem_procs_completed == SYNC_FAIL ||
        sem_water == SYNC_FAIL ||
        sem_h == SYNC_FAIL ||
        sem_h == SYNC_FAIL ||
        sem_o == SYNC_FAIL ||
        sem_sulfate == SYNC_FAIL ||
        sem_h_sulfate == SYNC_FAIL ||
        sem_s02 == SYNC_FAIL)
    {
        Printf("Bad sem_create in ");
        Printf(argv[0]);
        Printf("\n");
        Exit();
    }

    //Convert command line arguments to strings
    ditoa(num_water_init, num_water_init_str);
    ditoa(num_sulfate_init, num_sulfate_init_str);

    ditoa(sem_procs_completed, sem_procs_completed_str);
    ditoa(sem_water, sem_water_str);
    ditoa(sem_h, sem_h_str);
    ditoa(sem_o, sem_o_str);
    ditoa(sem_sulfate, sem_sulfate_str);
    ditoa(sem_h_sulfate, sem_h_sulfate_str);
    ditoa(sem_s02, sem_s02_str);

    ditoa(numReact1, numReact1_str);
    ditoa(numReact2, numReact2_str);
    ditoa(numReact3, numReact3_str);

    //All of process creation
    process_create(PRODUCER_FILENAME_1,sem_procs_completed_str, num_water_init_str, sem_water_str, NULL);
    process_create(PRODUCER_FILENAME_2,sem_procs_completed_str, num_sulfate_init_str, sem_sulfate_str, NULL);
    process_create(REACT_FILENAME_1,sem_procs_completed_str, sem_water_str, sem_h_str, sem_o_str, numReact1_str, NULL);
    process_create(REACT_FILENAME_2,sem_procs_completed_str, sem_sulfate_str, sem_s02_str, sem_o_str, numReact2_str, NULL);
    process_create(REACT_FILENAME_3,sem_procs_completed_str, sem_h_str, sem_o_str, sem_s02_str, sem_h_sulfate_str, numReact3_str, NULL);


    if (sem_wait(sem_procs_completed) != SYNC_SUCCESS) {
      Printf("Bad semaphore sem_procs_completed (%d) in ", sem_procs_completed); Printf(argv[0]); Printf("\n");
      Exit();
    }


    Printf("%d H2O's left over. %d H2's left over. %d O2's left over. %d SO2's left over. %d H2SO4's created.", 
        num_water_init - (2*numReact1),
        (2*numReact1) - numReact3,
        (numReact1 + numReact2 - numReact3),
        numReact2 - numReact3,
        numReact3);

    return 0;
}

int min3(int a, int b, int c){
    if(a < b && a < c)
        return a;
    else if(b < a && b < c)
        return b;
    else
        return c;
}
