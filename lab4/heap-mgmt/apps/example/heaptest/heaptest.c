#include "usertraps.h"
#include "misc.h"
#include "os/memory_constants.h"

void printDiv();

void main (int argc, char *argv[])
{
  int num_alloc = 5;
  int *array;

  int num_ptrs;
  int *ptrs[128];
  int mem_size;

  //=========================================================================//
  Printf("Test 1: Basic Memory Allocate\n");
  printDiv();
  //=========================================================================//
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
    Printf("Heaptest: could not free heap. Exiting\n");
    return; //Exit ??
  }
  Printf("Heaptest: Successfully freed %d bytes.\n", num_alloc * sizeof(int));


  //=========================================================================//
  printDiv();
  Printf("Test 2: Fill levels\n");
  printDiv();
  //=========================================================================//

  mem_size = MEM_PAGESIZE;
  num_ptrs = 1;
  for(i = 7; i >= 0; i--){
    Printf("Filling level with order=%d. Allocating %d blocks of size %d\n", i, num_ptrs, MEM_PAGESIZE);
    Printf("Allocating pages...\n");
    for(j = 0; j < num_ptrs; j++){
      ptrs[j] = malloc(mem_size);
      if(ptrs[j] == NULL){
        Printf("FAIL IN TEST 2 MALLOC\n");
        return;
      }
    }
    Printf("Writing to pages...\n");
    for(j = 0; j < num_ptrs; j++){
      ptrs[j] = 1;
    }
    Printf("Freeing pages...\n");
    for(j = 0; j < num_ptrs; j++){
      if(mfree(ptrs[j]) == MEM_FAIL)
      {
        Printf("FAIL IN TEST 2 FREE\n");
      }
    }

    mem_size /= 2;
    num_ptrs *= 2;
  }
}

void printDiv()
{
  Printf("--------------------------------------------------------------------------------\n");
}