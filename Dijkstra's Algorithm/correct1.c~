#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_VERTEX_NUMBER 100000
#define INF INT_MAX

// Structure for edge list of each vertex
typedef struct edgelistnode
{
	long dest;
	float length;
	struct edgelistnode *link;
} EdgeListNode;

// Structure for edge vertex of graph
typedef struct vertex
{
	int x;
	int y;
	EdgeListNode *head;
} Vertex;

// Structure for each node in min heap
typedef struct heapnode
{
	long  vertex;
	float length;
} heapNode;
 
// Create new min heap node
heapNode* createHeapNode(long vertex_num, float length)
{
	heapNode* newnode = malloc(sizeof(heapNode *));
	newnode->vertex = vertex_num;
	newnode->length = length;
	return newnode;
}

// Swap the pointers for two heap node pointers
void swapNodes(heapNode ** hn1, heapNode** hn2)
{
	heapNode* temp = *hn1;
	*hn1 = *hn2;
	*hn2 = temp;
}

// Standard min heapify function
void minHeapify(heapNode * * minHeap, long index, long num_elements, long * position, long max_size)
{
	long root, lchild, rchild;
	root = index;
	lchild = 2 * root + 1;
	rchild = 2 * root + 2;
 
	if (lchild < num_elements && minHeap[lchild]->length < minHeap[root]->length ) root = lchild;
 	if (rchild < num_elements && minHeap[rchild]->length < minHeap[root]->length ) root = rchild;
	if (root != index)
	{
 		position[minHeap[root]->vertex] = index;
 		position[minHeap[index]->vertex] = root;
 		swapNodes(&minHeap[root], &minHeap[index]);
 		minHeapify(minHeap, root, num_elements, position, max_size);
	}
}

// Extract min value from heap 
heapNode* extractMin(heapNode * * minHeap, long *num_elements, long * position, long max_size)
{
	if (*num_elements == 0) return NULL;

	heapNode* root = minHeap[0];
	heapNode* lastNode = minHeap[*num_elements - 1];
	minHeap[0] = lastNode;
 	position[root->vertex] = *num_elements-1;
	position[lastNode->vertex] = 0;
 	*num_elements = *num_elements - 1 ;
	minHeapify(minHeap, 0, *num_elements, position, max_size);
 	return root;
}


// Update index in position after changing length
void indexEval(heapNode * * minHeap, long vertex, float length, long num_elements, long * position, long max_size)
{
 	long i = position[vertex];
 
	minHeap[i]->length = length;
 
	while (i > 0 && minHeap[i]->length < minHeap[(i-1)/2]->length)
	{
		position[minHeap[i]->vertex] = (i-1)/2;
		position[minHeap[(i-1)/2]->vertex] = i;
		swapNodes(&minHeap[i],  &minHeap[(i-1)/2]);
 		i = (i-1)/2;
	}
}

// Assign initial null values to graoh elements 
void assignNullValues(Vertex * Graph)
{
	int i;
	for(i = 0; i < MAX_VERTEX_NUMBER; i++)
	{
		Graph[i].x = -1;
		Graph[i].y = -1;
		Graph[i].head = NULL;		
	}
}

// Create new edge list node
EdgeListNode* createNewEdge(long dest, float length)
{
	EdgeListNode* node = malloc(sizeof(EdgeListNode));
	node -> dest = dest;
	node -> length = length;
	node -> link = NULL;
	return node;
}

// Destroy edge list of a vertex
void destroyEdgeList(EdgeListNode * head)
{
	EdgeListNode *temp = head;
	while(1 == 1)
	{
		if(head == NULL) break;
		temp = head;
		head = head->link;
		free(temp);	
	}
}

// Add an edge to a edge list
void addToEdgeList(Vertex *graph, long src, long dest)
{
	EdgeListNode *newnode = createNewEdge(dest, sqrt( (graph[src].x - graph[dest].x)*(graph[src].x - graph[dest].x) + 
		     (graph[src].y - graph[dest].y)*(graph[src].y - graph[dest].y) )  ); 
					


	if(graph[src].head == NULL)
	{
		graph[src].head = newnode;
	}
	else if(graph[src].head->link == NULL) 
	{
		if(dest > graph[src].head->dest)
		{
			graph[src].head->link = newnode;
		}	
		else
		{
			newnode->link = graph[src].head;
		}
	
	}
	else if (newnode->dest < graph[src].head->dest)
	{
			newnode->link = graph[src].head;
			graph[src].head = newnode;
	}
	else
	{
		EdgeListNode *temp;	
		temp = graph[src].head;
		while(temp->link != NULL)
		{
			EdgeListNode *temp2 = temp->link;
			if (newnode->dest < temp2->dest) break;
			temp = temp -> link;
		}
			
		if (temp -> link == NULL)
		{
			temp->link = newnode;
		}
		else
		{
			newnode->link = temp->link;
			temp->link = newnode;
		}
		//return head;
	}


} 

// Add a vertex to graph
void addVertex(Vertex * graph, long vertex_num, int x, int y)
{
	graph[vertex_num].x = x;
	graph[vertex_num].y = y;
}

// Create a graph from a file
long createGraphFromFile(Vertex * graph, char *filename)
{
	long num_vertices, num_edges, src, dest, vertex_num;
	long i;
	int x, y;
	
	FILE * fp = fopen(filename, "r");
	fscanf(fp, "%ld %ld", &num_vertices, &num_edges);
	for(i = 0; i < num_vertices; i++)
	{
		fscanf(fp, "%ld %d %d", &vertex_num, &x, &y);
		addVertex(graph, i, x, y);
	}

	for(i = 0; i < num_edges; i++)
	{
		fscanf(fp, "%ld %ld", &src, &dest);
		addToEdgeList(graph, src, dest);
		addToEdgeList(graph, dest, src);
	}
	return num_vertices;
		 
}

// Print adjacency list
void printAdjacencyList(Vertex *graph, long num_vertices)
{
	long i;
	EdgeListNode *temp;
	for(i = 0; i < num_vertices; i++)
	{
		printf("%ld: ", i);
		temp = graph[i].head;
		while(temp != NULL)
		{
			printf("%ld ", temp->dest);
			temp = temp->link;
		}
		printf("\n");
	}
}

// Implement Djisktra's algorithm to print shortest path and its length
void findShortestPath(Vertex *Graph, long start, long stop, long num_vertices)
{
	
	float * lengths = malloc(num_vertices * sizeof(float));	
	long * revpath = malloc(num_vertices * sizeof(long));
	long * forpath = malloc(num_vertices * sizeof(long));

	long i;

	for(i = 0 ; i < num_vertices ; i++)
	{    	
		lengths[i] = INF;
    		revpath[i] = -1;

	}

	long num_elements = 0;
	long max_size = MAX_VERTEX_NUMBER;
	long *position = malloc(max_size * sizeof(long));
	heapNode * * minHeap = malloc(max_size * sizeof(heapNode*));;

	for (i = 0; i < num_vertices; i++)
	{
		if(i == start) continue;
		minHeap[i] = createHeapNode(i, lengths[i]);
		position[i] = i;
    	}
 	
    	lengths[start] = 0;
	minHeap[start] = createHeapNode(start, lengths[start]);
	position[start]   = start;
	indexEval(minHeap, start, lengths[start], num_elements, position,max_size);
 
    	num_elements = num_vertices;

	while (lengths[stop] == INF && num_elements > 0)
	{
		heapNode* minHeapNode = extractMin(minHeap, &num_elements, position, max_size);
		EdgeListNode * edge = Graph[minHeapNode->vertex].head;
		while (edge != NULL)
		{
			if (position[edge->dest] < num_elements  && (edge->length + lengths[minHeapNode->vertex] < lengths[edge->dest]) && lengths[minHeapNode->vertex] != INF)
			{
				lengths[edge->dest] = lengths[minHeapNode->vertex] + edge->length;
 				revpath[edge->dest] = minHeapNode->vertex;
				indexEval(minHeap, edge->dest, lengths[edge->dest], num_elements, position,max_size);
			}
			edge = edge->link;
		}
	}

	// Print shortest path
	long j;
	printf("%ld\n", (long int)lengths[stop]);
	long p = stop;
	j = 0;
	while(p >= 0)
	{
		forpath[j++] = p;
		p = revpath[p];
	}
	while(j > 0)
	{
		printf("%ld ", forpath[--j]);
	}
	printf("\n");

	free(lengths);
	free(revpath);
	free(forpath);

}

// Parse query file and print requested paths
void findPaths(Vertex * graph, long num_vertices, char * filename)
{
	long start, stop, num_queries, i;
	FILE * fp = fopen(filename, "r");
	fscanf(fp, "%ld", &num_queries);
	for(i = 0; i < num_queries; i++)
	{
		fscanf(fp, "%ld %ld", &start, &stop);
		findShortestPath(graph, start, stop, num_vertices);
	}
}

// Main function to handle command line arguments and memory deallocation
int main(int argc, char ** argv)
{
	long num_vertices;
	long i;
	Vertex graph[MAX_VERTEX_NUMBER];	
	assignNullValues(graph);
	if (argc != 3)	
	{
		printf("Invalid number of arguments.\nUsage:./shortestpath <graph file> <query file>\n");
		return -1;
	}

	// Create vertices
	num_vertices = createGraphFromFile(graph, argv[1]);
	
	// Find paths	
	findPaths(graph, num_vertices, argv[2]);
	
	// Destroy Graph
	for(i = 0; i < num_vertices; i++)
	{
		destroyEdgeList(graph[i].head);
	}	
	return 0;
}
