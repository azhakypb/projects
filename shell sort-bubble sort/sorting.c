#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "sorting.h"
#include <math.h>
#include <stdbool.h>

// helper functions
int * seq1gen(int N, int * seq1, int * gap);
int * seq2gen(int N, int *seq2, int * gap);
void swap(long *xx, long *yy){
  long temp = *xx;
  *xx = *yy;
  *yy = temp;
}
// *** //
void Improved_Bubble_Sort(long * Array, int Size, double *NComp, double *NMove)
{
  int gaps = 0;
  int *seq2 = malloc(sizeof(int)*Size);
  seq2 = seq2gen(Size, seq2, &gaps);
  int i;
  int j;


  for(i = 0; i < gaps; i++){
    for(j = 0; j < Size - seq2[i]; j++){
      if(Array[j] > Array[j+seq2[i]]){
	swap(&Array[j], &Array[j+seq2[i]]);
	(*NComp)++;
	(*NMove)+=3;
      }
    }
    for(j = Size - 1; j >= seq2[i]; j--){
      if(Array[j - seq2[i]] > Array[j]){
	swap(&Array[j], &Array[j-seq2[i]]);
	(*NComp)++;
	(*NMove)+=3;
      }
    }
  }
  free(seq2);
}
void Shell_Insertion_Sort(long * Array, int Size, double *NComp, double *NMove)
{
  int gaps = 0;
  int i;
  int j;
  int k = 0;
  int curGap;
  int insertionKey; //temp
  int *seq1 = malloc(sizeof(int)*Size);
  seq1 = seq1gen(Size, seq1, &gaps);

  for(curGap = seq1[k]; k <= gaps; k++){
    for(i = curGap; i < Size; i += 1){
      insertionKey = Array[i];
      
      for(j = i; j >= curGap && Array[j-curGap] > insertionKey; j-=curGap){
	Array[j] = Array[j-curGap];
	(*NMove)++;
	(*NComp)++;
      }
      Array[j] = insertionKey;
      (*NMove)++;
    }
  }
  free(seq1);
}
	
long *Load_File(char *Filename, int *Size)
{
    long *Arr = NULL;
    *Size = 0;
    FILE * ptr = fopen(Filename, "r");
    if(ptr == NULL){
      return Arr;
    }
    fscanf(ptr, "%d", Size);
    Arr = malloc(sizeof(long)*(*Size));
    int i = 0;
    while(fscanf(ptr, "%ld", &Arr[i]) == 1){
      i++;
    }
    
    fclose(ptr);
    return Arr;
}

int Save_File(char *Filename, long *Array, int Size)
{
    int n_written = 0;
    int i;
    FILE *ptr = fopen(Filename, "w");
    fprintf(ptr, "%d\n", Size);
    for(i = 0; i < Size; i++){
      fprintf(ptr, "%ld\n", Array[n_written]);
      n_written++;
    }

    fclose(ptr);
    return n_written;
}
void Save_Seq1(char *Filename, int N)
{
  int *seq1 = malloc(sizeof(int)*N);
  int seqSize = 0;
  seq1 = seq1gen(N, seq1, &seqSize);
  FILE * ptr = fopen(Filename, "w");
  if(ptr == NULL){
    printf("Error opening the file in Save_Seq1\n\n");
  }
  int i;
  for(i = 0; i <= seqSize; i++){
    fprintf(ptr, "%d ", seq1[i]);
  }
  free(seq1);
  fclose(ptr);
}
void Save_Seq2(char *Filename, int N)
{
  int *seq2 = malloc(sizeof(int)*N);
  int seqSize = 0;
  seq2 = seq2gen(N, seq2, &seqSize);
  FILE * ptr = fopen(Filename, "w");
  if(ptr == NULL){
    printf("Error opening the file in Save_Seq2\n\n");
  }
  int i;
  for(i = 0; i <= seqSize; i++){
    fprintf(ptr, "%d ", seq2[i]);
  }
  free(seq2);
  fclose(ptr);
}
int * seq1gen(int N, int * seq1, int * gapSize)
{
  int last2ind = 0;
  int last3ind = 0;
  int i = 0;
  int k = 0;
  seq1[0] = 1;
  for(i = 1; k < N; i++){
    if(seq1[last2ind]*2 < seq1[last3ind]*3){
      seq1[i] = seq1[last2ind]*2;
      last2ind++;
      if(k < N){
	k = seq1[i];
      }
    }
    else if(seq1[last2ind]*2 > seq1[last3ind]*3){
      seq1[i] = seq1[last3ind]*3;
      last3ind++;
      if(k < N){
	k = seq1[i];
      }
    }
    else{
      seq1[i] = seq1[last2ind]*2;
      last2ind++;
      last3ind++;
      if(k < N){
	k = seq1[i];
      }
    }
  }
  *gapSize = i-2;
  return seq1;
}
int * seq2gen(int N, int *seq2, int * gapSize)
{
  int i = 0;
  do{
    N  /= 1.3;
    if(N == 9 || N == 10){
      seq2[i] = 11;
      N = 11;
    }
    else{
      seq2[i] = N;
    }
    i++;
  }while(N != 1);
  *gapSize = i;
  seq2[i-1] = 1;
  return seq2;
}

