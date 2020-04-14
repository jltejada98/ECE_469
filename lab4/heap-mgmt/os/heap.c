
#include "heap.h"

int HeapInitialize(heapNode* heap, int len)
{
	int i;
	//Initialize all the values in the heap array
	for(i = 0; i < len; i++)
	{
		HeapInitializeNode(&heap[i], i, &heap[i].heapArr, len);
		heap[i].index = i;
		heap[i].heapArr = heap;
	}

	//Initialize the root node
	heap[0].inuse = 1;
	heap[0].allocated = 0;
	heap[0].isSplit = 0;

	return HEAP_SUCCESS;
}

int HeapInitializeNode(heapNode* node, int index, heapNode* heapArr, int heapSize){
	node->heapArr = heapArr;
	node->heapSize = heapSize;

	node->index = index;
	node->order = HEAP_NONE;
	node->addr = HEAP_NONE;

	node->inuse = 0;
	node->allocated = 0;
	node->isSplit = 0;

	return HEAP_SUCCESS;
}

int splitHeapNode(heapNode* parent){
	heapNode* left;
	heapNode* right;

	if(parent->allocated)
	{
		printf("Error: Attempted to split a node that had memory allocated\n");
		return HEAP_FAIL;
	}

	left = getLeft(parent);
	right = getRight(parent);

	left->order = parent->order - 1;
	left->inuse = 1;
	left->isSplit = 0;
	left->allocated = 0;

	right->order = parent->order -1;
	right->inuse = 1;
	right->isSplit = 0;
	right->allocated = 0;

	parent->isSplit = 1;
	return HEAP_SUCCESS;
}

int joinHeapNode(heapNode* parent){
	heapNode* left;
	heapNode* right;

	left = getLeft(parent);
	right = getRight(parent);

	if(left->allocated || left->isSplit || right->allocated || right->isSplit)
	{
		printf("Error: attempted to combine a parent node that had children that were doing something\n");
		return HEAP_FAIL;
	}

	left->inuse = 0;
	right->inuse = 0;
	parent->isSplit = 0;

	return HEAP_SUCCESS;
}


heapNode* getParent(heapNode* n) {
	int parentIdx = (n->index - 1)/2;
	if(parentIdx < 0){
		return HEAP_FAIL;	//Requested parent of root node, no parent so return HEAP_FAIL
	}
	
	return &(n->heapArr[parentIdx]);
}

heapNode* getLeft(heapNode* n) {return &n->heapArr[(n->index*2)+1];}
heapNode* getRight(heapNode* n) {return &n->heapArr[(n->index*2)+2];}


heapNode* searchNode(heapNode * node, int mem_size){
	heapNode *left;
	heapNode *right;

	if (node == NULL)
	{
		return HEAP_FAIL;
	}

	//Check if left is in use
	left = getLeft(node);
	if (left->inuse == 0)
	{
		/* code */
	}


}
