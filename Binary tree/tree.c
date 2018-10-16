#include "tree.h"

#ifndef USIGN_TO_BIN
// function to create binary values from the input char values
char * UnSig2Bin(unsigned char value) {
  unsigned char rev[NUM_BITS+1];
  rev[NUM_BITS] = '\0';
  char * out;
  int k = NUM_BITS-1;
  for(int i = 0; i<NUM_BITS; i++){
    rev[k] = (value >> i) & 1;
    k--;
  }
  out = (char*)rev;
  return out;
    // determine the number of bits needed ("sizeof" returns bytes)
	// create mask which you would use for the getting each bit value
	// for loop to mast values and creating char array of bits
    
}

#endif

#ifndef WRITE_BINARY
// WriteInOrderBinary takes root and file * as input
// and creates binary representation of the tree as specified in the example
int WritePreOrderBinary(TreeNode * root, FILE *outptr){
  if(root == NULL){
    return 0;
  }
  if(root->leftChild != NULL || root->rightChild !=NULL){
    fprintf(outptr,"0");
  }
  if(root->leftChild == NULL && root->rightChild == NULL){
    fprintf(outptr, "1");
    char * val = UnSig2Bin(root->data);
    char temp[NUM_BITS+1]={0};//I dont know why, but if I dont make a local copy, my val disappears for the first two vlaues
    for(int j = 0; j<NUM_BITS; j++){
      temp[j] = val[j];
      }
    temp[NUM_BITS] = '\0';
    for(int i = 0; i< NUM_BITS; i++){
      fprintf(outptr,"%d", temp[i]);
    }
  }
  WritePreOrderBinary(root->leftChild, outptr);
  WritePreOrderBinary(root->rightChild, outptr);
  return EXIT_SUCCESS;
}
void fixbinary(FILE * ptr);
void fixbinary(FILE * ptr){
  int c = 0;
  int count = 0;
  int temp = 0;
  do{
    c = fgetc(ptr);
    if(c != '\n'){
      count++;
    }
  }while(c!=EOF);
  temp = count;
  count = count / 8;
  count+=1;
  count = count*8 - temp;
  fseek(ptr, ftell(ptr), SEEK_SET);
  for(int i = 0; i<=count; i++){
    fprintf(ptr, "0");
  }
}
int CreateBinaryFromTree(TreeNode * root, const char *outfile){
  FILE * tmp = tmpfile();
  FILE * ptr = fopen(outfile, "wb");
  if(ptr == NULL){
    return EXIT_FAILURE;
  }
  WritePreOrderBinary(root, tmp);
  fprintf(tmp, "0");
  fseek(tmp, 0, SEEK_SET);
  fixbinary(tmp); //basically I just add zeros to the end of file to make it even
  fseek(tmp, 0, SEEK_SET);
  unsigned char temp[NUM_BITS];
  unsigned int sum = 0;
  
  while(NUM_BITS == fread(temp,1,NUM_BITS, tmp)){
  for(int i = 0; i<NUM_BITS;i++){
    sum = sum<<1;
    sum += temp[i] - '0';
  }
  fputc(sum, ptr);
  }
  
  fclose(tmp);
  fclose(ptr);
  return EXIT_SUCCESS;
}

#endif





#ifndef CLEAN_TREE

void CleanTree(TreeNode * root){
  if(root == NULL){
    return;
  }
  CleanTree(root->leftChild);
  CleanTree(root->rightChild);
  free(root);
}
#endif





#ifndef BINARY_SEARCH_TREE

int IntCompare(const void * val1, const void * val2){
  return (*(unsigned char*)val1 - *(unsigned char*)val2);
}

TreeNode * CreateBinarySearchTree(long randomSeed, long numNodes){
  unsigned char * binArray = malloc(sizeof(unsigned char)*numNodes);
  if(binArray == NULL){
    printf("Memory allocation to array failed\n" );
    return NULL;
  }
  srand(randomSeed);
  // allocate values to array
  for(int index = 0; index < numNodes ; index++){
    binArray[index] = rand() % 256;
  }
  // sort the array
  qsort(binArray, numNodes, sizeof(unsigned char), IntCompare);

  // create bst from the sorted array
  TreeNode * root = CreateBST(binArray, 0, numNodes-1);
  free(binArray);
  return root;

}
// TreeNode *CreateBSTHelper()
TreeNode* CreateTreeNode(unsigned char value){
  TreeNode *newNode = malloc(sizeof(TreeNode));
  newNode->data = value;
  newNode->leftChild = NULL;
  newNode->rightChild = NULL;
  return newNode;
}

TreeNode *CreateBST(unsigned char *sortedArray, int start , int end){
  // int len = sizeof(sortedArray)/sizeof(int);
  if(start > end) return NULL;
  int mid = (start+end)/2;
  TreeNode *newNode = CreateTreeNode(sortedArray[mid]);
  newNode->leftChild = CreateBST(sortedArray, start, mid -1);
  newNode->rightChild = CreateBST(sortedArray, mid +1, end);
  return newNode;

}
#endif
