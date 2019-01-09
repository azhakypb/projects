#include "theHeader.h"

mapData *readMap(char *filename, long *verticesNum){
	FILE *ptr = fopen(filename, "r");
	if(ptr == NULL){
		return NULL;
	}
	long edgesNum;

	fscanf(ptr, "%ld", verticesNum);
	fscanf(ptr, "%ld", &edgesNum);

	mapData *temp = malloc(sizeof(mapData)*(*verticesNum));

	long i;
	for(i = 0; i < (*verticesNum); i++){
		fscanf(ptr, "%ld %d %d", &temp[i].vertexID, &temp[i].x, &temp[i].y);
		temp[i].head = NULL;
	}

	long from;
	long to;
	for(i = 0; i < edgesNum; i++){
		fscanf(ptr, "%ld %ld", &from, &to);
		//I have to add the edge from both sides to ensure solution to queries where "from" > "to" //
		addEdge(temp, from, to);
		addEdge(temp, to, from);
	}
	fclose(ptr);
	return temp;
}
void addEdge(mapData * fullData, long src, long dst){
	long dist = calcDist(fullData, src, dst);
	edgeArr *newEdge = createEdge(dst, dist);

	if(fullData[src].head == NULL){
		fullData[src].head = newEdge;
	}

	else if(fullData[src].head->next == NULL){
			fullData[src].head->next = newEdge;
	}
	else{
		edgeArr *temp1 = fullData[src].head->next;
		while(temp1->next != NULL){
			temp1 = temp1->next;
		}
			temp1->next = newEdge;
	}
}


//Utility Functions//
edgeArr * createEdge(long dst, long dist){
	edgeArr * newEdge = malloc(sizeof(edgeArr));
	newEdge->toID = dst;
	newEdge->dist = dist;
	newEdge->next = NULL;
	return newEdge;
}
long calcDist(mapData * fullData, long src, long dst){
        long temp1 = abs(fullData[src].x - fullData[dst].x);
	long temp2 = abs(fullData[src].y - fullData[dst].y);
	temp1 *=temp1;
	temp2 *=temp2;
	long ans = sqrt((double)temp1 + (double)temp2);
	
	return ans;
}
void freeMem(mapData *fullData, long verticesNum){
	long i;
	edgeArr * temp = NULL;
	for(i = 0; i < verticesNum; i++){
		while(fullData[i].head != NULL){
			temp = fullData[i].head;
			fullData[i].head =fullData[i].head->next;
			free(temp);
		}
	}
}
