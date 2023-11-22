//HEADER FILES
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<time.h>

//MACROS
#define LEFT(i)  (2 * i + 1)
#define RIGHT(i) (2 * i + 2)
#define PARENT(i) ( i - 1 ) / 2

//FUNCTION DECLARATIONS
void swap(int* Heap, int x, int y);
void MaxHeapify_rc(int* Heap, size_t HeapSize, int i);

//Heap Interface Routines

//Array of heap structure is sorted using heapSort in ascending order as we have used maxHeapify procedure
void heapSort(int* Heap, size_t HeapSize) {
	size_t size = HeapSize;
	for (int i = HeapSize - 1; i >= 1; i--) {
		swap(Heap, i, 0);
		size = size - 1;
		MaxHeapify_rc(Heap, size, 0);
	}
}

//Recursive Version of MaxHeapify procedure
void MaxHeapify_rc(int* Heap, size_t HeapSize, int i) {
	int l = LEFT(i);
	int r = RIGHT(i);
	int largest = i;

	if (l < HeapSize && Heap[i] <= Heap[l]) {
		largest = l;
	}
	if (r < HeapSize &&  Heap[r] > Heap[largest]) {
		largest = r;
	}
	//we check here, whether or not, largest has changed to l or r from i
	if (i != largest) {
		swap(Heap, i, largest);
		MaxHeapify_rc(Heap, HeapSize, largest);
	}
}

void MaxHeapify_nrc(int* Heap, size_t HeapSize, int i) {
	int l = 0, r = 0, largest = 0;
	while (true) {
		l = LEFT(i);
		r = RIGHT(i);
		if (l < HeapSize && Heap[i] < Heap[l]) {
			largest = l;
		}
		else {
			largest = i;
		}

		if (r < HeapSize && Heap[r] > Heap[largest]) {
			largest = r;
		}

		if (largest != i)
		{
			swap(Heap, i, largest);
			i = largest;
		}
		else
			break;
	}
}

void MinHeapify_rc(int* Heap, size_t HeapSize, int i) {
	int l = LEFT(i);
	int r = RIGHT(i);
	int smallest = 0;
	if (l < HeapSize && Heap[i] > Heap[l]) {
		smallest = l;
	}
	else
		smallest = i;

	if (r < HeapSize && Heap[r] < Heap[smallest]) {
		smallest = r;
	}
	if (i != smallest) {
		swap(Heap, i, smallest);
		MinHeapify_rc(Heap, HeapSize, smallest);
	}
}

//To build heap, we considered parent in heap structure that can be heapify except leaf nodes because they follow the heap property by default
//Note : We have subtracted 1 from HeapSize because, root node of heap started with index 'i' = 0
void buildHeap(int* Heap, size_t HeapSize) {
	for (int i = PARENT(HeapSize - 1); i >= 0; i--) {
		MaxHeapify_rc(Heap, HeapSize, i);
	}
}

void IncreaseKey(int* Heap, size_t HeapSize, int i, int key) {
	if (key <= Heap[i])
		return;

	Heap[i] = key;

	int parent = PARENT(i); // ( i - 1 ) / 2
	while (parent >= 0 && key > Heap[parent]) {
		swap(Heap, i, parent);
		i = parent;
		parent = (parent - 1) / 2;
	}//printHeap(Heap, HeapSize, "updated value ");
}

void InsertElement(int** pHeap, size_t* size, int key) {

	*size = *size + 1;
	int HeapSize = *size;
	*pHeap = (int*)realloc(*pHeap, HeapSize * sizeof(int));
	int* Heap = *pHeap;
	Heap[HeapSize - 1] = 0;
	//printHeap(Heap, HeapSize, "updated value ");
	IncreaseKey(Heap, HeapSize, HeapSize - 1, key);

}

int ExtractMax(int** pHeap, size_t* HeapSize) {
	
	int MaxEle = (*pHeap)[0];
	swap(*pHeap, 0, *HeapSize - 1);
	*HeapSize = *HeapSize - 1;

	*pHeap = realloc(*pHeap, *HeapSize * sizeof(int));
	MaxHeapify_rc(*pHeap, *HeapSize, 0);
	return MaxEle;
}

//Heap Auxillary Routines
static void printHeap(int* Heap, int HeapSize, const char* msg) {
	if (msg) puts(msg);
	for (int i = 0; i < HeapSize; i++)
		printf("[%d] ", Heap[i]);
	printf("\n\n");
}

static void swap(int* Heap, int x, int y) {
	int temp = Heap[x];
	Heap[x] = Heap[y];
	Heap[y] = temp;
}

static int*getHeap(size_t HeapSize) {
	int* heap = malloc(HeapSize * sizeof(int));
	memset(heap, 0, HeapSize * sizeof(int));
	return heap;
}

static void readHeap(int* Heap, size_t HeapSize) {
	srand(time(0));
	for (int i = 0; i < HeapSize; i++)
		Heap[i] = (rand() % 90 - 50 + 1) + 50;
}

//ENTRY POINT OF ALL FUNCTIONS
int main(void) {
	size_t heapSize = 10;
	int* heap = getHeap(heapSize);
	int arr[] = {4, 6, 8, 9, 12, 2, 3, 7, 1, 11, 10};

	for(int idx = 0; idx < (sizeof(arr) / sizeof(arr[0])); idx++ )
		heap[idx]  =  arr[idx];
	//readHeap(heap, heapSize);
	printHeap(heap, heapSize, "Before MaxHeapify : ");

	MaxHeapify_nrc(heap, heapSize, 0);
	printHeap(heap, heapSize, "After MaxHeapify : ");

	buildHeap(heap, heapSize);
	printHeap(heap, heapSize, "After Building Heap : ");

	//IncreaseKey(heap, size, index, value)
	IncreaseKey(heap, heapSize, heapSize - 1, 100);
	printHeap(heap, heapSize, "After increasing element at last index by 100 : ");

	InsertElement(&heap, &heapSize, 200);
	printHeap(heap, heapSize, "After inserting element 200");
	
	int MaxELe = ExtractMax(&heap, &heapSize);
	printf("Max element is %d\n", MaxELe);
	printHeap(heap, heapSize, "After extracting max element");

    MaxELe = ExtractMax(&heap, &heapSize);
	printf("Max element is %d\n", MaxELe);
	printHeap(heap, heapSize, "After extracting max element");

	heapSort(heap, heapSize);
	printHeap(heap, heapSize, "After Sorting Heap : ");

	free(heap);
	return 0;
}