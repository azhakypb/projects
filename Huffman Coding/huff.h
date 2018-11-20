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
  
}TreeNode;


//FREQUENCY FUNCTION//
void freqCount(FILE * in, int *freq);

//WRITING TO FILE FUNCTIONS//
void writeBits(const char *encoding, FILE *out);
void writeHeaderInfo(FILE *out, int freqs[]);

//BUILDING HUFFMAN TREE AND ENCODING FUNCTIONS//
char **buildTable(int freqs[]);
void enCode(FILE *in, FILE *out);
TreeNode *buildTree(int freqs[]);
void mapData(TreeNode *head, char **entable, char *path);

//UTILITY FUNCTIONS//
int cmpFunc(const void *a, const void *b);
void CleanTable(char *entable[]);
void CleanTree(TreeNode *head);
char *pathUPD(char *path, char curLetter);
