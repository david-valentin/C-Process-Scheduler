#include <stdlib.h>
#include <sys/time.h>
#include "coursework.h"
#include <stdio.h>
#include <stdbool.h>

bool compareBurstTime(struct process *head, struct process *next);

void print_list(struct process * head);

struct process * addSJF(struct process* headRef, struct process* new_node);

void createLinkedList(struct process **head);


int main()
{
  // Set the head to null
  struct process *head = NULL;

  int i;
  for (i = 0; i < NUMBER_OF_PROCESSES; i++) {
    // Generates a new process each time
    struct process *next = generateProcess();
    printf("The value of next is: %d\n", next->iBurstTime);
    // adding values to the head and use insertion sort
    head = addSJF(head, next, insertionSort);
  }
  print_list(sorted_list);
}

/*
  ADDSJF
    args:
    1. headRef => the head linkedlist that we intend to returns | NULL on first go
    2. newNode => the next linkedlist item | NOT NULL on first go
    3. addMethod => a pointer to an addMethod | can be insertionSort for SJF or RoundRobin
*/
struct process * addSJF(struct process *headRef, struct process *newNode, struct process * (*addMethod)(struct process *head)(struct process *next))
{
  /*
    if the headRef is null, we know:
      newNode is the first item in the list
  */
  if (headRef == NULL) {
    printf("%d\n", newNode->iBurstTime);
    // Assign the oNext pointer to the NULL head value
    newNode->oNext = *headRef;
    *headRef = new_node;
    return newNode;
  } else {
    return addMethod(headRef, newNode);
  }
}

/*
  Desc:
    Conducts the insertion sort on the struct process headRef
  Args:
    1. headRef is the process reference that we are looking at addShortFirst
    2. newNode is the process that we are adding to the head

  Cases:
    1. if the head->oNext process is null we know:
      * that we need to compare the newNode to head => compareBurstTime();
      * if compareBurstTime() => true

*/
struct process * insertionSort(struct process *headRef, struct process *newNode) {

  // This will hold our current reference
  struct process *current;

  // TRUE IF: the head ref iBurstTime is bigger than the newNode iBurstTime
  if (compareBurstTime(headRef, newNode))
  {
    newNode->oNext = *headRef;
    *headRef = new_node;
  } else
  {
    current = headRef;
    // if the current->oNext value isn't NULL and current/headRef->iBurstTime > newNode
    while (current->oNext != NULL && compareBurstTime(current, newNode))
    {
      current = current->oNext;
    }
    // Break out of the loop status:
    /*
      current = 7
      current->oNext = 10;
      newNode = 9
      newNode->oNext = NULL;
    */

    // Assign the next ref of newNode to
    // the pointer after 9 will point to 10
    newNode->oNext = current->oNext;
    /*
      current = 7
      current->oNext = 10;
      newNode = 9
      newNode->oNext = 10;
    */
    current->oNext = newNode;
  }
  // return the current linkedlist
  return current;

}

bool compareBurstTime(struct process *head, struct process *next)
{
  return (head->iBurstTime > next->iBurstTime);
}

void print_list(struct process * head) {
    struct process * current = head;

    while (current != NULL && current->oNext != NULL && current->oNext == current) {
        printf("%d\n", current->iBurstTime);
        current = current->oNext;
        sleep(1);
    }

}
