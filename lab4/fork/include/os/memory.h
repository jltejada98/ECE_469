#ifndef	_memory_h_
#define	_memory_h_

// Put all your #define's in memory_constants.h
#include "memory_constants.h"

extern int lastosaddress; // Defined in an assembly file

//--------------------------------------------------------
// Existing function prototypes:
//--------------------------------------------------------

int MemoryGetSize();
void MemoryModuleInit();
uint32 MemoryTranslateUserToSystem (PCB *pcb, uint32 addr);
int MemoryMoveBetweenSpaces (PCB *pcb, unsigned char *system, unsigned char *user, int n, int dir);
int MemoryCopySystemToUser (PCB *pcb, unsigned char *from, unsigned char *to, int n);
int MemoryCopyUserToSystem (PCB *pcb, unsigned char *from, unsigned char *to, int n);
int MemoryPageFaultHandler(PCB *pcb);

//---------------------------------------------------------
// Put your function prototypes here
//---------------------------------------------------------
// All function prototypes including the malloc and mfree functions go here

void freemapSet(int page, int inuse);
int freemapGet(int page);

void MemorySharePage(uint32 pte);

uint32 MemoryAllocPage(void);
uint32 MemoryGetPte (uint32 flags);
int freePte(uint32 pte);
void MemoryFreePage(uint32 page);

void MemoryRopHandler(PCB* pcb);

void* malloc(PCB* pcb, int memsize);
int mfree(PCB* pcb, void* plast_os_pagetr);


#endif	// _memory_h_
