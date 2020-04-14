//
//	memory.c
//
//	Routines for dealing with memory management.

//static char rcsid[] = "$Id: memory.c,v 1.1 2000/09/20 01:50:19 elm Exp elm $";

#include "ostraps.h"
#include "dlxos.h"
#include "process.h"
#include "memory.h"
#include "queue.h"

static int freemapmax = MEM_NUM_PAGES % 32 == 0 ? MEM_NUM_PAGES / 32 : MEM_NUM_PAGES / 32 + 1;
static uint32 freemap[MEM_NUM_PAGES % 32 == 0 ? MEM_NUM_PAGES / 32 : MEM_NUM_PAGES / 32 + 1];
static uint32 pagestart;	//Start of non-OS pages?
static int nfreepages;

//----------------------------------------------------------------------
//
//	This silliness is required because the compiler believes that
//	it can invert a number by subtracting it from zero and subtracting
//	an additional 1.  This works unless you try to negate 0x80000000,
//	which causes an overflow when subtracted from 0.  Simply
//	trying to do an XOR with 0xffffffff results in the same code
//	being emitted.
//
//----------------------------------------------------------------------
static int negativeone = 0xFFFFFFFF;
static inline uint32 invert (uint32 n) {
  return (n ^ negativeone);
}

//----------------------------------------------------------------------
//
//	MemoryGetSize
//
//	Return the total size of memory in the simulator.  This is
//	available by reading a special location.
//
//----------------------------------------------------------------------
int MemoryGetSize() {
  return (*((int *)DLX_MEMSIZE_ADDRESS));
}


//----------------------------------------------------------------------
//
//	MemoryModuleInit
//
//	Initialize the memory module of the operating system.
//      Basically just need to setup the freemap for pages, and mark
//      the ones in use by the operating system as "VALID", and mark
//      all the rest as not in use.
//
//----------------------------------------------------------------------
void MemoryModuleInit() {
  int i, bit;

  uint32 last_os_page = lastosaddress / MEM_PAGESIZE;

  nfreepages = MEM_NUM_PAGES - (last_os_page + 1);
  pagestart = last_os_page + 1;

  //Set entire freemap to not inuse
  for(i = 0; i<freemapmax; i++)
  	freemap[i] = 0;

  //Set memory used by OS in freemap to inuse
  i = 0;
  while(i <= last_os_page){
    bit = i % 32;
    freemapSet(i, 1);
    i++;
  }

}

//----------------------------------------------------------------------
//
//  freemapSet
//
//  This function sets a page as inuse or not inuse in
//	the freemap
//
//----------------------------------------------------------------------
void freemapSet(int page, int inuse) {
	int index = page / 32;
	int bit = page % 32;

	if(!(inuse == 0 || inuse == 1))
	{
		printf("Warning in freemapSet, inuse=%d, should be 1 or 0", inuse);
	}

	if(page >= MEM_NUM_PAGES)
	{
		printf("Fatal Error: Attempting to allocate a page that is out of bounds!\n");
		printf("Attempted to allocate page %d (0 indexed) but there are only %d pages.", page, MEM_NUM_PAGES);
		exitsim();
	}

	if(inuse){
		freemap[index] = freemap[index] | (0x1 << bit);
	}
	if(!inuse){
		freemap[index] = freemap[index] & invert(0x1 << bit);
	}
}

//----------------------------------------------------------------------
//
//  freemapGet
//
//  This function sets a page as inuse or not inuse in
//	the freemap
//
//----------------------------------------------------------------------
int freemapGet(int page) {
	int index = page / 32;
	int bit = page % 32;

	return (freemap[index] >> bit) & 0x1;
}


//----------------------------------------------------------------------
//
// MemoryTranslateUserToSystem
//
//	Translate a user address (in the process referenced by pcb)
//	into an OS (physical) address.  Return the physical address.
//
//----------------------------------------------------------------------
uint32 MemoryTranslateUserToSystem (PCB *pcb, uint32 addr) {
	int page_table_index;
	uint32 page_table_entry;
	uint32 page_offset;
	uint32 physical_page_addr;

	page_table_index = (addr >> MEM_L1FIELD_FIRST_BITNUM);
	page_offset = MEM_PAGE_OFFSET_MASK & addr;

	if(page_table_index >= MEM_MAX_NUM_PTE)
	{
		printf("Fatal Error: Attempting to translate virtual address that does not have a corresponding PTE for proccess %d\n", GetPidFromAddress(pcb));
		printf("addr: %x\n Index: %d\n MEM_MAX_NUM_PTE: %d\n", addr, page_table_index, MEM_MAX_NUM_PTE);
		exitsim();
	}

	page_table_entry = pcb->pagetable[page_table_index];

	if(!(page_table_entry & MEM_PTE_VALID))
	{
		printf("Error: Reading Page Table Entry that is invalid!\n");
		return MEM_FAIL;
	}

	physical_page_addr = (page_table_entry & MEM_MASK_PTE_TO_PAGE) | page_offset;

	if(physical_page_addr > MEM_MAX_SIZE)
	{
		printf("Error: in MemoryTranslateUserToSystem:\nAttempting to translate address that is greater than system physcal memory size!\n");
		return MEM_FAIL;
	}

	return physical_page_addr;

}


//----------------------------------------------------------------------
//
//	MemoryMoveBetweenSpaces
//
//	Copy data between user and system spaces.  This is done page by
//	page by:
//	* Translating the user address into system space.
//	* Copying all of the data in that page
//	* Repeating until all of the data is copied.
//	A positive direction means the copy goes from system to user
//	space; negative direction means the copy goes from user to system
//	space.
//
//	This routine returns the number of bytes copied.  Note that this
//	may be less than the number requested if there were unmapped pages
//	in the user range.  If this happens, the copy stops at the
//	first unmapped address.
//
//----------------------------------------------------------------------
int MemoryMoveBetweenSpaces (PCB *pcb, unsigned char *system, unsigned char *user, int n, int dir) {
  unsigned char *curUser;         // Holds current physical address representing user-space virtual address
  int		bytesCopied = 0;  // Running counter
  int		bytesToCopy;      // Used to compute number of bytes left in page to be copied

  while (n > 0) {
    // Translate current user page to system address.  If this fails, return
    // the number of bytes copied so far.
    curUser = (unsigned char *)MemoryTranslateUserToSystem (pcb, (uint32)user);

    // If we could not translate address, exit now
    if (curUser == (unsigned char *)0) break;

    // Calculate the number of bytes to copy this time.  If we have more bytes
    // to copy than there are left in the current page, we'll have to just copy to the
    // end of the page and then go through the loop again with the next page.
    // In other words, "bytesToCopy" is the minimum of the bytes left on this page 
    // and the total number of bytes left to copy ("n").

    // First, compute number of bytes left in this page.  This is just
    // the total size of a page minus the current offset part of the physical
    // address.  MEM_PAGESIZE should be the size (in bytes) of 1 page of memory.
    // MEM_ADDRESS_OFFSET_MASK should be the bit mask required to get just the
    // "offset" portion of an address.
    bytesToCopy = MEM_PAGESIZE - ((uint32)curUser & MEM_PAGE_OFFSET_MASK);
    
    // Now find minimum of bytes in this page vs. total bytes left to copy
    if (bytesToCopy > n) {
      bytesToCopy = n;
    }

    // Perform the copy.
    if (dir >= 0) {
      bcopy (system, curUser, bytesToCopy);
    } else {
      bcopy (curUser, system, bytesToCopy);
    }

    // Keep track of bytes copied and adjust addresses appropriately.
    n -= bytesToCopy;           // Total number of bytes left to copy
    bytesCopied += bytesToCopy; // Total number of bytes copied thus far
    system += bytesToCopy;      // Current address in system space to copy next bytes from/into
    user += bytesToCopy;        // Current virtual address in user space to copy next bytes from/into
  }
  return (bytesCopied);
}

//----------------------------------------------------------------------
//
//	These two routines copy data between user and system spaces.
//	They call a common routine to do the copying; the only difference
//	between the calls is the actual call to do the copying.  Everything
//	else is identical.
//
//----------------------------------------------------------------------
int MemoryCopySystemToUser (PCB *pcb, unsigned char *from,unsigned char *to, int n) {
  return (MemoryMoveBetweenSpaces (pcb, from, to, n, 1));
}

int MemoryCopyUserToSystem (PCB *pcb, unsigned char *from,unsigned char *to, int n) {
  return (MemoryMoveBetweenSpaces (pcb, to, from, n, -1));
}

//---------------------------------------------------------------------
// MemoryPageFaultHandler is called in traps.c whenever a page fault 
// (better known as a "seg fault" occurs.  If the address that was
// being accessed is on the stack, we need to allocate a new page 
// for the stack.  If it is not on the stack, then this is a legitimate
// seg fault and we should kill the process.  Returns MEM_SUCCESS
// on success, and kills the current process on failure.  Note that
// fault_address is the beginning of the page of the virtual address that 
// caused the page fault, i.e. it is the vaddr with the offset zero-ed
// out.
//
// Note: The existing code is incomplete and only for reference. 
// Feel free to edit.
//---------------------------------------------------------------------
int MemoryPageFaultHandler(PCB *pcb) {
	//User stack pointer virtual address
	uint32 user_stack_ptr = pcb->currentSavedFrame[PROCESS_STACK_USER_STACKPOINTER];
  
	//Pointer to attempted address access
  	uint32 fault_addr = pcb->currentSavedFrame[PROCESS_STACK_FAULT];

	//Index in pagetable where user stack currently ends
	uint32 user_stack_idx = user_stack_ptr >> MEM_L1FIELD_FIRST_BITNUM;

	if(fault_addr >= (user_stack_ptr & 0x1FF000))
	{
		printf("Allocating new page in pte %x\n", user_stack_idx-1);
		//If new page is already being used by heap
		if(pcb->pagetable[user_stack_idx - 1] & MEM_PTE_VALID)
		{
			//Cannot create new pagetable entry, out of virtual memory space
			printf("Process %d ran out of memory\n", GetPidFromAddress(pcb));
			ProcessKill();
		}
		printf("1\n");
		pcb->pagetable[user_stack_idx - 1] = MemoryGetPte(MEM_PTE_VALID);
		printf("2\n");

		if(pcb->pagetable[user_stack_idx - 1] == MEM_FAIL)
		{
			printf("Unable to allocate new page to stack for Process %d\n", GetPidFromAddress(pcb));
			ProcessKill();
		}
		printf("3\n");

		(pcb->npages)++;

		printf("Succesfully allocated new page!");

		return MEM_SUCCESS;
	}
	else 
	{
		//its a segFault so kill the process
		printf("Fatal Error: Segmentation Fault. Fautlt address: 0x%x with User Stack Pointer: 0x%x\n", fault_addr, user_stack_ptr & 0x1FF000);
		ProcessKill();
	}

	//Control should not reach this point
	return MEM_FAIL;
}


//---------------------------------------------------------------------
// You may need to implement the following functions and access them from process.c
// Feel free to edit/remove them
//---------------------------------------------------------------------

uint32 MemoryAllocPage(void) {
  int i;

  if(nfreepages <= 0)
  {
  	return MEM_FAIL;
  }

  for(i = 0; i < MEM_NUM_PAGES; i++)
  {
  	if(freemapGet(i) == 0)
  	{
  		freemapSet(i, 1);
  		nfreepages--;
  		return i * MEM_PAGESIZE;
   	}
  }

  printf("Fatal Error: System could not find free page although nfreepages > 0!\n");
  exitsim();	//Does not reutrn, but need return for compiler
  return MEM_FAIL;
}


uint32 MemoryGetPte (uint32 flags) {
	uint32 page;
	page = MemoryAllocPage();

	if(page == MEM_FAIL)
		return MEM_FAIL;

	return page | flags;
}


// Takes a pte (the base address) and frees the page
// that corresponds to the pte
int freePte(uint32 pte) {
	uint32 phys_base_addr;
	uint32 pageIdx;

	if(!(pte & MEM_PTE_VALID))
	{
		printf("Error: Attempting to free page that was not allocated!\n");
		return MEM_FAIL;
	}

	phys_base_addr = pte & MEM_MASK_PTE_TO_PAGE;
	pageIdx = phys_base_addr / MEM_PAGESIZE;

	MemoryFreePage(pageIdx);

	return MEM_SUCCESS;

}

//Takes in the pages index and frees that page
void MemoryFreePage(uint32 pageIdx) {
	freemapSet(pageIdx, 0);
	nfreepages++;
}

void* malloc(PCB* pcb, int memsize) {
  return NULL;
}

int mfree(PCB* pcb, void* ptr) {
  return -1;
}
