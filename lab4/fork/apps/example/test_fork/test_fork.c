#include "usertraps.h"
#include "misc.h"


void main (int argc, char *argv[])
{
  int child_proc;
  int val;

  Printf("Starting test_fork\n");
  
  val = 0;


  child_proc = fork();

  print("PID: %d | Child_proc: %d\n", getpid(), child_proc);

  /*
  if (child_proc == 0) //Child Process
  {
  	Printf("child_proc with pid:", getpid());
  	Printf("Generating ROP exception.. Original Value: %d, New Value", val, val+2);
  	val += 2;
  	Printf("Finished ROP exception, Val: %d, PID:", val, getpid());
  }
  else{ //Parent Process
  	Printf("parent_proc with pid:", getpid());
  	Printf("Generating ROP exception.. Original Value: %d, New Value", val, val+1);
  	++val;
  	Printf("Finished ROP exception, Val: %d, PID:", val, getpid());
  }
  */

}
