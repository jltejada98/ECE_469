#include "usertraps.h"

void main (int x)
{
  Printf("Hello World!\n");
  Printf("The PID of this user program is : %d", Getpid());
  while(1); // Use CTRL-C to exit the simulator
}
