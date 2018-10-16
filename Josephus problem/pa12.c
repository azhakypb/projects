#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pa12.h"

#ifndef INIT
//head: the head of the singly linkedlist.
//length: the length of the singly linkedlist.
//This function will create and initialize the singly linkedlist with length=length, 
//The values of the nodes within the singly linkedlist will be the same as array indices.
//For example, if the singly linkedlist length is 4, the values of the nodes will be 0 1 2 3
void init(struct node **head,int length)
{
  struct node * temp = NULL;
  struct node * last = NULL;
  for(int i = 0; i<length; i++){
    temp = malloc(sizeof(struct node));
    temp->value = i;
    temp->next = NULL;
    if(*head == NULL){
      *head = temp;
    }
    else{
      last->next = temp;
    }
    last = temp;
  }
  last->next = *head;
}
    
  /*struct node * temp = malloc(sizeof(struct node));
  temp ->value = 0;
  temp->next = NULL;
  *head = temp;
  for(int i = 1; i<length; i++){
    temp->next = malloc(sizeof(struct node));
    temp = temp->next;
    temp->value = i;
  }
  temp->next = NULL;
  print(head);*/
  /* while((*head) != NULL){
    printf("%d, ", (*head)->value);
    *head = (*head)->next;
    }*/
#endif

#ifndef PRINT
//head: the head of the singly linkedlist.
//This function will print the values of the nodes within the singly linkedlist.
//For example, if the values of the nodes within the singly linkedlist are 1 5 6 8 9 10, this function will print 1,5,6,8,9,10
//Note: there is no extra comma nor at the beginning nor at the end.
void print(struct node **head)
{
  struct node * temp = *head;
  if(temp == temp->next){
    printf("%d\n", temp->value);
    return;
  }
  /*int count = 1;
  temp = temp->next;
  while(temp !=(*head)){
    temp = temp->next;
    count+=1;
  }
  temp = *head;
  for(int i = 0; i<count; i++){
    printf("%d,", temp->value);
    temp = temp->next;
  }
  printf("%d\n", temp->value);*/
  
  do{
    printf("%d,", temp->value);
    temp = temp->next;
  }while((*head) != temp->next);
  printf("%d\n", temp->value);
    
    /*while(temp->next !=NULL){
    printf("%d,", temp->value);
    temp = temp->next;
  }
  printf("%d\n", temp->value);*/
}
#endif

#ifndef JOSPI
//head: the head of the singly linkedlist.
//k : this variable will be used for counting if count = k, the element will be out.
//p : this variable will be used for printing if the number of remaining elements/nodes is divisible by p (call the print function).
//length: the length of the singly linkedlist.
//This function will solve the josephus problem using the singly linkedlist.
void josp(struct node ** head,int k,int p,int length)
{
  struct node * rem = *head;
  struct node * temp = *head;
  //struct node * fix = *head;
  int count = length;
  int i;
  while(rem->next != rem){
    for(i = 0; i<k; i++){
      temp = rem;
      rem = rem->next;
    }
    temp -> next = rem -> next;
    if(rem == *head){
      *head = rem->next;
    }
    free(rem);
    rem = temp->next;
    count = count -1;
    if(count % p == 0){
      print(head);
      /*if((*head)->next->next != NULL){
	print(head);
      }
      else{
	print(&rem);
	}*/
    }
  }
  *head = rem;
}

#endif


#ifndef INITII
//head: the head of the doubly linkedlist.
//length: the length of the doubly linkedlist.
//This function will create and initialize the doubly linkedlist with length=length, 
//The values of the nodes within the doubly linkedlist will be the same as array indices.
//For example, if the doubly linkedlist length is 4, the values of the nodes will be 0 1 2 3
//NOTE: this is a doubly linkedlist you should assign the prev pointer for each node probably.
//NOTE: head->prev is NULL as well as tail->next.

void init2(struct dnode ** head,int length)
{
  int i = 0;
  struct dnode * last = NULL;
  if(*head==NULL){
    struct dnode * temp = malloc(sizeof(struct dnode));
    temp->value = i;
    temp->next = temp->prev = temp;
    *head = temp;
  }
  for(i=1; i<length; i++){
    last = (*head)->prev;
    struct dnode * new = malloc(sizeof(struct dnode));
    new->value = i;
    new->next = *head;
    (*head)->prev = new;
    new->prev = last;
    last->next = new;
  }
  /*for(int i = length; i>=0; i--){
    struct dnode * temp = malloc(sizeof(struct dnode));
    temp->value = i;
    temp->next = *head;
    temp->prev = NULL;
    if((*head) !=NULL){
      (*head)->prev = temp;
    }
    *head = temp;
    }*/
 
}
#endif


#ifndef PRINTII
//head: the head of the doubly linkedlist.
//This function will print the values of the nodes within the doubly linkedlist.
//For example, if the values of the nodes within the doubly linkedlist are 1 5 6 8 9 10, this function will print 1,5,6,8,9,10
//Note: there is no extra comma nor at the beginning nor at the end.
void print2(struct dnode ** head)
{
  struct dnode * temp = *head;
  if(temp == NULL){
    return;
  }
  if(temp->next == temp || temp->prev == temp){
    printf("%d\n", temp->value);
    return;
  }
  do{
    printf("%d,", temp->value);
    temp = temp->next;
  }while((*head)->prev != temp);
  printf("%d\n", temp->value);
	


}

#endif


#ifndef JOSPII
//head: the head of the doubly linkedlist.
//k : this variable will be used for counting if count = k, the element will be out.
//q : this variable will be used for flipping the direction. If while counting, the value of the node is divisible by q,
//you will take the node out, flip the direction, and continue counting.
//length: the length of the doubly linkedlist.
//This function will solve the modified josephus problem using the doubly linkedlist.
//NOTE: you should call the print2() function after any delete operation.
void josp2(struct dnode **head, int k, int q,int length)
{
  struct dnode * rem = *head;
  struct dnode * temp = *head;
  struct dnode * prevhead = NULL;
  // struct dnode * remhead = *head;
  bool flip = false;
  
  int count = 0;
  while(length !=1){
    while(count != k){
      if(flip == true){
	prevhead = rem;
	rem = rem->prev;
	count+=1;
	if(rem->value % q == 0){
	  flip = !flip;
	  if(rem == *head){
	    prevhead = (*head)->prev;
	    *head = (*head)->next;
	    prevhead->next = *head;
	    (*head)->prev = prevhead;
	    free(rem);
	    length-=1;
	    rem = prevhead;
	  }
	  else if(rem->next == *head){
	    prevhead->next = *head;
	    (*head)->prev = prevhead;
	    free(rem);
	    length-=1;
	    rem = prevhead;
	  }
	  else{
	    temp = rem->next;
	    prevhead->next = temp;
	    temp->prev = prevhead;
	    free(rem);
	    length-=1;
	    rem = prevhead;
	  }
	  print2(head);
	}
      }
      if(flip == false){
	prevhead = rem;
	rem = rem->next;
	count+=1;
	if(rem->value % q == 0){
	  flip = !flip;
	  if(rem == *head){
	    prevhead = (*head)->prev;
	    *head = (*head)->next;
	    prevhead->next = *head;
	    (*head)->prev = prevhead;
	    free(rem);
	    length-=1;
	    rem = prevhead;
	  }
	  else if(rem->next == *head){
	    prevhead->next = *head;
	    (*head)->prev = prevhead;
	    free(rem);
	    length-=1;
	    rem = prevhead;
	  }
	  else{
	    temp = rem->next;
	    prevhead->next = temp;
	    temp->prev = prevhead;
	    free(rem);
	    length-=1;
	    rem = prevhead;
	  }
	  print2(head);
	}
      }
    }
    if(rem->value % q == 0){
      flip = !flip;
    }
    
    if(rem == *head){
      prevhead = (*head)->prev;
      *head = (*head)->next;
      prevhead->next = *head;
      (*head)->prev = prevhead;
      free(rem);
      length-=1;
      rem = prevhead;
    }
    else if(rem->next == *head){
      prevhead->next = *head;
      (*head)->prev = prevhead;
      free(rem);
      length-=1;
      rem = prevhead;
    }
    else{
      temp = rem->next;
      prevhead->next = temp;
      temp->prev = prevhead;
      free(rem);
      length-=1;
      rem = prevhead;
    }
    print2(head);
    count = 0;
  }
    
   
  /* struct node * temp = *head;
  struct node * rem = *head;
  int len = length;
  int count = 0;
  bool flip = false;
  
  while(len != 1){
    while(count != k){
    temp = rem;
    rem = rem->next;
    while(rem->value % q == 0){
      flip = !flip;
      if(rem->next !=NULL){
	rem->next->prev = rem->prev;
      }
      if(rem->prev !=NULL){
	rem->prev->next = rem->next;
      }
      if(rem == *head){
	*head = rem->next;
      }
      free(rem);
      
      rem = temp->next;
      print(head);
      count+=1;
      }*/
    
}

#endif




