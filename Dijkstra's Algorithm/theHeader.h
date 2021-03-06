#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>
#define INF INT_MAX

typedef struct MinHeap { 
    long curSize;      // Number of heap nodes present currently 
    long capacity;  // Capacity of min heap 
    long *pos;     // This is needed for decreaseKey() 
    struct MinHeapNode **array; 
}MinHeap;

typedef struct MinHeapNode { 
    long v; 
    long dist; 
}MinHeapNode; 

typedef struct edgeArr{
	long toID;
	long dist;
	struct edgeArr *next;
}edgeArr;

typedef struct mapData{
	long vertexID;
	unsigned int x;
	unsigned int y;
	edgeArr *head;
}mapData;

mapData *readMap(char *filename, long *vertices);
void addEdge(mapData * fullData, long src, long dst);
edgeArr * createEdge(long dst, long dist);
long calcDist(mapData * fullData, long src, long dst);
void freeMem(mapData *fullData, long verticesNum);

void traverseQuery(char *filename, mapData *fullData, long verticesNum);
void doDijkstra(long src, long dst, mapData *fullData, long verticesNum);
void printAns(long dist, long path[], long src, long dst);
bool ifEdge(mapData *fullData, long i, long j);
long edgeDist(mapData *fullData, long i, long j);
long findMinDist(long distFromSrc[], bool sptSet[], long verticesNum);

bool isInMinHeap(MinHeap *minHeap, long v);
void dcrKey(MinHeap* minHeap, long v, long dist);
MinHeapNode* extractMin(MinHeap* minHeap);
long isEmpty(MinHeap* minHeap);
void minHeapify(MinHeap* minHeap, long curIdx);
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b);
MinHeap* createMinHeap(long capacity);
MinHeapNode* newMinHeapNode(long v, long dist);
void freeMinHeap(MinHeap *minHeap, long verticesNum);
