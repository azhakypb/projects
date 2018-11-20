#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASCIIT 257   //since ascii range 0-255, we add one more space for PS_EOF  
#define PS_EOF 256   //our pseudo eof
#define BITS 8	     //reading in 8 bit chunks

//HUFFMAN TREE
typedef struct TreeNode
{
  int data;
  int freq;
  struct TreeNode *left;
  struct TreeNode *right;
} TreeNode;

//DECODING FUNCTIONS //
void deCode(FILE *in, FILE *out);
int deCodeCurChar(FILE *in, TreeNode *head);

//READING FUNCTONS //
int readBit(FILE *in);
int *readHeaderInfo(FILE *in);

//UTILITY FUNCTIONS//
TreeNode *buildTree(int freqs[]);
void CleanTree(TreeNode *head);
int cmpFunc(const void *a, const void *b);
