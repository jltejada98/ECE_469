#include "usertraps.h"
#include "misc.h"


void main (int argc, char *argv[])
{
  int child_proc = 0;
  int val = 0;

  Printf("Starting test_fork\n");

  child_proc = fork();

  if (child_proc == 0) //Child Process
  {
  	Printf("child_proc with pid:", getpid());
  	Printf("Generating ROP exception.. Original Value: %d, New Value", val, val+2)
  	val += 2;
  	Printf("Finished ROP exception, Val: %d, PID:", val, getpid());
  }
  else{ //Parent Process
  	Printf("parent_proc with pid:", getpid());
  	Printf("Generating ROP exception.. Original Value: %d, New Value", val, val+1)
  	++val;
  	Printf("Finished ROP exception, Val: %d, PID:", val, getpid());
  }


}
