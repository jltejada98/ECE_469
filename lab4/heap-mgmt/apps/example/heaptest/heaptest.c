#include "usertraps.h"
#include "misc.h"
#include "memory_constants.h"

void main (int argc, char *argv[])
{
  int num_alloc = 40;
  int *array;

  //Attempt to allocate memory.
  Printf("Heaptest: Attempting to allocated %d bytes.\n", num_alloc * sizeof(int));
  array = malloc(sizeof(int) * num_alloc);
  if (array == NULL)
  {
    Printf("Heaptest: could not allocate heap. Exiting");
    return; //Exit ??
  }
  Printf("Heaptest: Successfully allocate %d bytes at address (decimal) %d\n", num_alloc * sizeof(int), array);

  //Write to memory (Just checking validity of address)
  Printf("Writing to allocated memory...\n");
  array[0] = 1;
  Printf("Successfully wrote to to allocated memory...\n");

  //Attempt to free memory.
  Printf("Heaptest: Attempting to free %d bytes.\n", num_alloc * sizeof(int));
  if(mfree(array) == MEM_FAIL)
  {
    Printf("Heaptest: could not free heap. Exiting");
    return; //Exit ??
  }
  Printf("Heaptest: Successfully freed %d bytes.\n", num_alloc * sizeof(int));

}
