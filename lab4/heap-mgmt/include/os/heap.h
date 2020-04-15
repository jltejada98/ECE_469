#ifndef	NULL
#define	NULL	((void *)0)
#endif

#include "ostraps.h"
#include "dlxos.h"
#include "memory_constants.h"

#define HEAP_NONE 0
#define HEAP_FAIL -1
#define HEAP_SUCCESS 1

typedef struct heapNode {
	//Points back to array containing tree
	struct heapNode* heapArr;
	int heapSize;

	int index; //Nodes index in array
	int order; //Power of 2 of size
	uint32 addrOffset;  //Address of memory

	//If a node isSplit, its children should be inuse
	//If a node isSplit=0, its children's inuse=0
	int inuse;		//the node is part of the tree
	int allocated;	//memory address has been assigned to node
	int isSplit;	//Has children that are part of tree
}heapNode;

int HeapInitialize(heapNode* heap, int len);
int HeapInitializeNode(heapNode* heap, int index, heapNode* heapArr, int heapSize);
void initializeAddrOffsets(heapNode* node);

int splitHeapNode(heapNode* heapNode);
int joinHeapNode(heapNode* parent);

//Finds an available node to be allocated of specified order
heapNode* findNodeOrder(heapNode* root, int order);

//Creates a node of desired order if possible
//Returns NULL on fail
//Returns Node created on success
heapNode* createOrder(heapNode* root, int order);

//Deallocs memory from a node
//If sibling node is also deallocated, it will join them
int deallocNode(heapNode* node);

//Sets a node as allocated
uint32 allocNode(heapNode* node);

heapNode* getParent(heapNode* n);
heapNode* getLeft(heapNode* n);
heapNode* getRight(heapNode* n);

int isAlwaysLeaf(heapNode* n);

int sizeToOrder(int memSize);
int orderToMemsize(int order);
void printNodeData(heapNode* node);