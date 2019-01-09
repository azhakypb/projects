#include "theHeader.h"

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("Wrong Input\n");
		return EXIT_FAILURE;
	}

	mapData *fullData = NULL;
	long verticesNum = 0;

	fullData = readMap(argv[1], &verticesNum);

	traverseQuery(argv[2], fullData, verticesNum);
	//Free Memory
	freeMem(fullData, verticesNum);
	free(fullData);

	return EXIT_SUCCESS;
}
