#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unhuff.h"

int main(int argc, char *argv[]){
  if(argc != 2){
    printf("wrong input\n");
    return EXIT_FAILURE;
  }
  char *filename = argv[1];
  FILE *in = fopen(filename, "rb");
  strcat(filename, ".unhuff");
  FILE * out = fopen(filename, "wb");
  deCode(in, out);

  fclose(in);
  fclose(out);

  return EXIT_SUCCESS;
}
int *readHeaderInfo(FILE *in){
  static int freqs[ASCIIT];
  int i;
  int count;
  int data;
  int freq;

  if(fscanf(in, "%d", &count) != 1){
    printf("wrong input");
  }

  for(i = 0; i < count; i++){
    if((fscanf(in, "%d %d", &data, &freq) != 2) || data < 0 || data >= ASCIIT){
      printf("wrong input");
    }
    freqs[data] = freq;
  }
  fgetc(in);
  return freqs;
}

void deCode(FILE *in, FILE *out){
  int *freqs;
  int curChar;
  TreeNode *head = NULL;

  freqs = readHeaderInfo(in);
  head = buildTree(freqs);

  while((curChar = deCodeCurChar(in, head)) != PS_EOF){
    fputc(curChar, out);
  }
  CleanTree(head);
}
int cmpFunc(const void *a, const void *b){
  const TreeNode **x = (const TreeNode **) a;
  const TreeNode **y = (const TreeNode **) b;
  if((*x)->freq == (*y)->freq){
    return 0;
  }
  else{
    return ((*x)->freq < (*y)->freq) ? 1 : -1;
  }
}
int ReadBit(FILE *in){
  static int theBits = 0, bitCount = 0;
  int nextBit;

  if(bitCount == 0){
    theBits = fgetc(in);
    bitCount = (1 << (BITS - 1));
  }

  nextBit = theBits / bitCount;
  theBits %= bitCount;
  bitCount /= 2;

  return nextBit;
}
int deCodeCurChar(FILE *in, TreeNode *head){
  while(head->left || head->right){
    if(ReadBit(in)){
      head = head->right;
    }
    else{
      head = head->left;
    }
  }
  return head->data;
}
void CleanTree(TreeNode *head){
    if(head != NULL)
    {
        CleanTree(head->left);
        CleanTree(head->right);
        free(head);
    }
}
TreeNode *buildTree(int freqs[]){
  int i = 0;
  int listLen = 0;
  TreeNode * nodes[ASCIIT];

  for(i = 0; i < ASCIIT; i++){
    if(freqs[i] != 0){
      TreeNode *temp = (TreeNode *)calloc(1, sizeof(TreeNode));
      temp->data = i;
      temp->freq = freqs[i];
      nodes[listLen] = temp;
      listLen++;
    }
  }

  while(listLen > 1) {
    TreeNode *temp = (TreeNode *)malloc(sizeof(TreeNode));
    qsort(nodes, listLen, sizeof(TreeNode *), cmpFunc);
    temp->left = nodes[--listLen];
    temp->right = nodes[--listLen];
    temp->freq = temp->left->freq + temp->right->freq;

    nodes[listLen++] = temp;
  }
  return nodes[0];
}
