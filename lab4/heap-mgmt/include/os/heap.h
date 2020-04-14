#ifndef	NULL
#define	NULL	((void *)0)
#endif

#include "ostraps.h"
#include "dlxos.h"

#define HEAP_NONE 0
#define HEAP_FAIL -1
#define HEAL_SUCCESS 1

typedef struct heapNode {
	//Points back to array containing tree
	struct heapNode* heapArr;
	int heapSize;

	int index; //Nodes index in arra
	int order; //Power of 2 of size
	uint32 addr;  //Address of memory

	int inuse;		//the node is part of the tree
	int allocated;	//memory address has been assigned to node
	int isSplit;	//Has children that are part of tree
};

int HeapInitialize(heapNode* heap, int len);
int HeapInitializeNode(heapNode* heap, int index, heapNode* heapArr, int heapSize);

int splitHeapNode(heapNode* heapNode);
int joinHeapNode(heapNode* parent);

heapNode* getParent(heapNode* n);
heapNode* getLeft(heapNode* n);
heapNode* getRight(heapNode* n);
