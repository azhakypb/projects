#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pa12.h"




int main(int argc, char **argv)
{
  if(strcmp(argv[1], "-J") == 0){
    struct node * head = NULL;
    int length = atoi(argv[2]);
    int kval = atoi(argv[3]);
    int pval = atoi(argv[4]);
    if(length == 0){
      return EXIT_FAILURE;
    }
    init(&head, length);
    josp(&head, kval, pval, length);
    free(head);
  }
  else{
    struct dnode * head2 = NULL;
    int length2 = atoi(argv[2]);
    int kval2 = atoi(argv[3]);
    int pval2 = atoi(argv[4]);
    if(length2 == 0){
      return EXIT_FAILURE;
    }
    init2(&head2, length2);
    josp2(&head2, kval2, pval2, length2);
    free(head2);
  }
    
    
  return EXIT_SUCCESS;
}
