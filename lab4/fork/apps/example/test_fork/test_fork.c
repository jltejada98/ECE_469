#include "usertraps.h"
#include "misc.h"


void main (int argc, char *argv[])
{
  int child_proc;
  int val;

  Printf("Starting test_fork, parent_proc: %d\n", getpid());
  Printf("var child_proc virtual address: %d\n", &child_proc);
  val = 0;

  child_proc = fork();

  Printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");

  if (child_proc == 0) //Child Process
  {
  	Printf("This is the child (%d), child_proc=%d\n", getpid(), child_proc);
  	val += 2;
  	Printf("Val: %d, PID: %d\n", val, getpid());
    Printf(">>>>>Success Child!<<<<<\n");
  }
  else{ //Parent Process
    Printf("This is the parent (%d), child_proc=%d\n", getpid(), child_proc);
    val += 5;
    Printf("Val: %d, PID: %d\n", val, getpid());
    Printf(">>>>>>Success Parent!<<<<<\n");

  }


}
