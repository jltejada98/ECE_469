
#include "heap.h"

int HeapInitialize(heapNode* heap, int len)
{
	int i;
	//Initialize all the values in the heap array
	for(i = 0; i < len; i++)
	{
		HeapInitializeNode(&heap[i], i, heap, len);
		heap[i].index = i;
		heap[i].heapArr = heap;
	}

	//Initialize the root node
	heap[0].inuse = 1;
	heap[0].allocated = 0;
	heap[0].isSplit = 0;
	heap[0].order = 7;

	//Initialize the address offsets
	initializeAddrOffsets(&heap[0]);

	return HEAP_SUCCESS;
}

void initializeAddrOffsets(heapNode* node)
{

	if(isAlwaysLeaf(node))
	{
		return;
	}


	getLeft(node)->addrOffset = node->addrOffset;

	getRight(node)->addrOffset = (orderToMemsize(node->order) / 2) + node->addrOffset;

	initializeAddrOffsets(getLeft(node));
	initializeAddrOffsets(getRight(node));
}

int HeapInitializeNode(heapNode* node, int index, heapNode* heapArr, int heapSize){
	node->heapArr = heapArr;
	node->heapSize = heapSize;

	node->index = index;
	node->order = HEAP_NONE;
	node->addrOffset = 0;

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

	if(isAlwaysLeaf(parent))
	{
		printf("Tried joining heap node that was always a leafnode, it cannot have children\n");
		return HEAP_FAIL;
	}

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





heapNode* findNodeOrder(heapNode* root, int order){
	heapNode *find;

	if (root == NULL || root->allocated || !root->inuse)
	{
		return NULL;
	}

	if(root->order < order)
	{
		return NULL;
	}

	if (root->order == order && root->inuse && !root->allocated && !root->isSplit)
	{
		return root;
	}
	
	find = findNodeOrder(getLeft(root), order);

	if(find == NULL){
		find = findNodeOrder(getRight(root), order);
	}

	return find;
}

heapNode* createOrder(heapNode* root, int order){
	int lowest_order;
	int i;
	heapNode* node;

	for(lowest_order = order+1; lowest_order <= root->order; lowest_order++)
	{
		node = findNodeOrder(root, lowest_order);

		if(node != NULL)
			break;
	}

	if(node == NULL)
	{
		return NULL;
	}

	printf("Found node of order %d to start splitting\n", node->order);


	for(i = lowest_order; i > order; i--)
	{
		printf("Splitting node of order %d\n", node->order);
		if(splitHeapNode(node) == HEAP_FAIL)
		{
			return NULL;
		}
		node = getLeft(node);
	}

	return node;
}

int deallocNode(heapNode* node){
	heapNode* parent;
	heapNode* sibling;

	//We do not want to join a node if its split, this should never happen
	if(node->isSplit)
	{
		printf("Error: Attempted deallocating a split node\n");
		return HEAP_FAIL;
	}

	//Cannot join root node, so just dealloc and return
	if(node->index == 0)
	{
		//node->addrOffset = NULL;
		node->allocated = 0;
		return HEAP_SUCCESS;
	}

	parent = getParent(node);

	if(getLeft(parent) == node)
	{
		sibling = getRight(parent);
	} 
	else if(getRight(parent) == node)
	{
		sibling = getLeft(parent);
	}
	else
	{
		printf("Error: could not find sibling correctly\n");
		return HEAP_FAIL;
	}

	//node->addrOffset = NULL;
	node->allocated = 0;

	if(sibling->allocated)
	{
		//Sibling is allocated, so do not join
		return HEAP_SUCCESS;
	}
	else
	{
		//Sibling is not allocated, so join them
		if(coalessNodes(parent) == HEAP_SUCCESS)
		{	
			return HEAP_SUCCESS;
		}
		else
		{
			return HEAP_FAIL;
		}
	}
}

int caolessNodes(heapNode parent)
{
	if(joinHeapNode(parent) != HEAP_SUCCESS)
		return HEAP_FAIL;

	printf("Coalesced buddy nodes ");
	printNodeData(getLeft(parent));
	printf(" & ");
	printNodeData(getRight(parent));
	printf(" into the parent node ");
	printNodeData(parent);
	printf("\n");

	if(parent->index == 0){
		//This is the root node, cannot coaless anymore
		return HEAP_SUCCESS;
	}

	parent = getParent(parent);

	if(!(getLeft(parent)->allocated) && !(getLeft(parent)->allocated))
		return coalessNodes(parent);
	else
		return HEAP_SUCCESS;

}



uint32 allocNode(heapNode* node)
{
	if(node == NULL)
	{
		return HEAP_FAIL;
	}

	node->allocated = 1;
	return node->addrOffset;
}

int isAlwaysLeaf(heapNode* n){
	if( (n->index * 2) + 1 > (n->heapSize - 1) ){
		return 1;
	}
	return 0;
}

heapNode* getParent(heapNode* n) {
	int parentIdx = (n->index - 1)/2;
	if(parentIdx < 0){
		return NULL;	//Requested parent of root node, no parent so return HEAP_FAIL
	}
	
	return &(n->heapArr[parentIdx]);
}

heapNode* getLeft(heapNode* n) {
	int leftIdx = (n->index*2)+1;

	if( isAlwaysLeaf(n) )
		return NULL;

	return &n->heapArr[leftIdx];
}
heapNode* getRight(heapNode* n) {
	int rightIdx = (n->index*2)+2;

	if( isAlwaysLeaf(n) )
		return NULL;

	return &n->heapArr[rightIdx];
}

int sizeToOrder(int memSize)
{
	int order = 0;
	int num = MEM_MIN_HEAP_NODE_SIZE;

	if(memSize < 0)
	{
		return HEAP_FAIL;
	}
	while(num < memSize)
	{
		order++;
		num *= 2;
	}

	return order;
}

int orderToMemsize(int order) {
	int i;
	int num = MEM_MIN_HEAP_NODE_SIZE;

	if(order < 0)
	{
		return HEAP_FAIL;
	}

	for(i = 0; i < order; i++)
	{
		num *= 2;
	}
	return num;
}

void printNodeData(heapNode* node)
{
	printf("(order = %d, addr = %d, size = %d)", node->order, node->addrOffset, orderToMemsize(node->order));
	return;
}