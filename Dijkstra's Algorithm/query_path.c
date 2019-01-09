#include "theHeader.h"
void traverseQuery(char *filename, mapData *fullData, long verticesNum){
	FILE *ptr = fopen(filename, "r");

	long src;
	long dst;
	long i;
	long queriesNum;

	fscanf(ptr, "%ld", &queriesNum);

	for(i = 0; i < queriesNum; i++){
	  fscanf(ptr, "%ld %ld", &src, &dst);
	  doDijkstra(src, dst, fullData, verticesNum);
	}
	fclose(ptr);
}
void doDijkstra(long src, long dst, mapData *fullData, long verticesNum){
	long distFromSrc[verticesNum];
	long pathVector[verticesNum];
	MinHeap *minHeap = createMinHeap(verticesNum);

	long v;
	for(v = 0; v < verticesNum; v++){
		distFromSrc[v] = INF;
		minHeap->array[v] = newMinHeapNode(v, distFromSrc[v]);
		minHeap->pos[v] = v;
	}
	pathVector[src] = -1;
	distFromSrc[src] = 0;
	dcrKey(minHeap, src, distFromSrc[src]);
	minHeap -> curSize = verticesNum;

	while(!isEmpty(minHeap)){
		MinHeapNode *temp = extractMin(minHeap);
		long minDistID = temp->v;
		free(temp);
		if(minDistID == dst){
			printAns(distFromSrc[minDistID], pathVector, src, dst);
			freeMinHeap(minHeap, verticesNum);
			break;
		}
		edgeArr *currentNode = fullData[minDistID].head;
		while(currentNode != NULL){
			v = currentNode->toID;
			if(isInMinHeap(minHeap, v) && distFromSrc[minDistID] != INT_MAX && currentNode->dist +distFromSrc[minDistID] < distFromSrc[v]){
			  				pathVector[v] = minDistID;
				distFromSrc[v] = distFromSrc[minDistID] + currentNode->dist;
				dcrKey(minHeap, v, distFromSrc[v]);
			}
			currentNode = currentNode->next;
		}
	}
}
void printPath(long path[], long j){
  if(path[j] == -1){
    return;
  }
  printPath(path, path[j]);
  printf("%ld ", j);
  }
void printAns(long dist, long path[], long src, long dst){
	if(dist != INF){
		printf("%ld\n", dist);
		printf("%ld ", src);
		printPath(path, dst);
		printf("\n");
	}
	else{
		printf("INF\n");
		printf("%ld %ld\n", src, dst);
	}
}

//Utility Functions//
// A utility function to create a new Min Heap Node 
MinHeapNode* newMinHeapNode(long v, long dist){ 
	MinHeapNode* minHeapNode = (MinHeapNode*) malloc(sizeof(MinHeapNode)); 
    minHeapNode->v = v; 
    minHeapNode->dist = dist; 
    return minHeapNode; 
} 
  
// A utility function to create a Min Heap 
MinHeap* createMinHeap(long capacity){ 
	MinHeap* minHeap = (MinHeap*) malloc(sizeof(MinHeap)); 
    minHeap->pos = (long *)malloc(capacity * sizeof(long)); 
    minHeap->curSize = 0; 
    minHeap->capacity = capacity; 
    minHeap->array = 
         (MinHeapNode**) malloc(capacity * sizeof(MinHeapNode*)); 
    return minHeap; 
} 
  
// A standard function to heapify at given idx 
// This function also updates position of nodes when they are swapped. 
void minHeapify(MinHeap* minHeap, long curIdx){ 
  long smallest;
    long left;
    long right;

    smallest = curIdx; 
    left = 2 * curIdx + 1; 
    right = 2 * curIdx + 2; 
  
  if (left < minHeap->curSize && minHeap->array[left]->dist < minHeap->array[smallest]->dist){ 
  	smallest = left;
  }
  
  if (right < minHeap->curSize && minHeap->array[right]->dist < minHeap->array[smallest]->dist){ 
  	smallest = right;
  }
  
  if (smallest != curIdx){ 
  	MinHeapNode *smallestNode = minHeap->array[smallest]; 
  	MinHeapNode *curIdxNode = minHeap->array[curIdx]; 
  
    minHeap->pos[smallestNode->v] = curIdx; 
    minHeap->pos[curIdxNode->v] = smallest; 
  
    swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[curIdx]); 
  
    minHeapify(minHeap, smallest); 
    } 
}  
long isEmpty(MinHeap* minHeap){ 
    return minHeap->curSize == 0; 
} 
  
// Standard function to extract minimum node from heap 
MinHeapNode* extractMin(MinHeap* minHeap){ 
    if (isEmpty(minHeap)) 
        return NULL; 
  
    MinHeapNode* head = minHeap->array[0]; 
  
    MinHeapNode* lastNode = minHeap->array[minHeap->curSize - 1]; 
    minHeap->array[0] = lastNode; 
  
    // Update position of last node 
    minHeap->pos[head->v] = minHeap->curSize-1; 
    minHeap->pos[lastNode->v] = 0; 
  
   	minHeap->curSize -=1; 
    minHeapify(minHeap, 0); 
  
    return head; 
} 
  
// Function to decrease dist value of a given vertex v. This function 
// uses pos[] of min heap to get the current index of node in min heap 
void dcrKey(MinHeap* minHeap, long v, long dist){ 
    long i = minHeap->pos[v]; 
  
    minHeap->array[i]->dist = dist; 
  
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist){ 
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2; 
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i; 
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]); 
  
        // move to parent index 
        i = (i - 1) / 2; 
    } 
} 
  
// A utility function to check if a given vertex 
// 'v' is in min heap or not 
bool isInMinHeap(MinHeap *minHeap, long v){ 
   if (minHeap->pos[v] < minHeap->curSize){
   	return true;
   } 
   return false; 
}

void freeMinHeap(MinHeap *head, long verticesNum){
  /*long i;
  for(i = 0; i < verticesNum; i++){
    if(head->array[i] != NULL){
      free(head->array[i]);
    }
    }*/
  free(head->array);
  free(head->pos);
  free(head);
}
// A utility function to swap two nodes of min heap. Needed for min heapify 
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b){ 
    MinHeapNode* t = *a; 
    *a = *b; 
    *b = t; 
} 
