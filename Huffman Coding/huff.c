#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huff.h"


int main(int argc, char *argv[]){
  if(argc != 2){
    printf("wrong input\n");
    return EXIT_FAILURE;
  }
  char *filename = argv[1];
  FILE *in = fopen(filename, "rb");
  strcat(filename, ".huff");
  FILE * out = fopen(filename, "wb");
  enCode(in, out);
    

  fclose(in);
  fclose(out);

  return EXIT_SUCCESS;
}


void freqCount(FILE * in, int *freq){
  int curFreq = 0;
  while((curFreq = fgetc(in)) != EOF){
    freq[curFreq]++;
  }
  freq[PS_EOF] = 1;
  fseek(in, 0, SEEK_SET);
}


void writeBits(const char *encoding, FILE *out){
  static int theBits = 0, bitCount = 0;

  while(*encoding){
    theBits = theBits * 2 + *encoding - '0';
    bitCount++;

    if(bitCount == BITS){
      fputc(theBits, out);
      theBits = 0;
      bitCount = 0;
    }
    encoding++;
  }
}
void writeHeaderInfo(FILE *out, int freqs[]){
  int i;
  int count = 0;

  for(i = 0; i < ASCIIT; i++){
    if(freqs[i] != 0){
      count++;
    }
  }
  fprintf(out, "%d\n", count);

  for(i = 0; i < ASCIIT; i++){
    if(freqs[i] != 0){
      fprintf(out, "%d %d\n", i, freqs[i]);
    }
  }
}


char **buildTable(int freqs[]){
  static char *entable[ASCIIT];
  char *path = (char *)calloc(1, sizeof(char));

  TreeNode *head = buildTree(freqs);
  mapData(head, entable, path);
  CleanTree(head);

  return entable;
}
void enCode(FILE *in, FILE *out){
  int curFreq;
  int freqs[ASCIIT];
  int i;
  for(i = 0; i < ASCIIT; i++){
    freqs[i] = 0;
  }
  char **enTable;

  freqCount(in, freqs);

  enTable = buildTable(freqs);
  writeHeaderInfo(out, freqs);

  while((curFreq= fgetc(in)) != EOF){
    writeBits(enTable[curFreq], out);
  }

  writeBits(enTable[PS_EOF], out);
  writeBits("0000000", out);

  CleanTable(enTable);
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

  while(listLen > 1){
    TreeNode *temp = (TreeNode *)malloc(sizeof(TreeNode));        
    qsort(nodes, listLen, sizeof(TreeNode *), cmpFunc);
        
    temp->left = nodes[--listLen];
    temp->right = nodes[--listLen];
    temp->freq = temp->left->freq + temp->right->freq;

    nodes[listLen++] = temp;
  }

  return nodes[0];
}
void mapData(TreeNode *head, char **entable, char *path){
  if(!head->left && !head->right){
    entable[head->data] = path;
  }
  else{
    if(head->left){
      mapData(head->left, entable, pathUPD(path, '0'));
    }
    if(head-> right){
      mapData(head->right, entable, pathUPD(path, '1'));
    }
    free(path);
    }
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
void CleanTable(char *entable[]){
  int i;
  for(i = 0; i < ASCIIT; i++){
    if(entable[i]){
      free(entable[i]);
    }
  }
}
void CleanTree(TreeNode *head){
  if(head != NULL){
    CleanTree(head->left);
    CleanTree(head->right);
    free(head);
  }
}
char *pathUPD(char *path, char curLetter){
  char *result = (char *)malloc(strlen(path) + 2);
  sprintf(result, "%s%c", path, curLetter);
  return result;
}
