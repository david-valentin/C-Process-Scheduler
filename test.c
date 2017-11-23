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
    addSJF(head, next);
  }
  print_list(sorted_list);
}

/*
  ADDSJF
    args:
    1. headRef => the head linkedlist that we intend to returns | NULL on first go
    2. nextRef => the next linkedlist item | NOT NULL on first go
    3. addMethod => a pointer to an addMethod | can be insertionSort for SJF or RoundRobin
*/
struct process * addSJF(struct process *headRef, struct process *nextRef, (struct process *) (*addMethod)(struct process *head)(struct process *next))
{
  /*
    if the headRef is null, we know:
      nextRef is the first item in the list
  */
  if (headRef == NULL) {
    printf("%d\n", nextRef->iBurstTime);
    // Assign the oNext pointer to the NULL head value
    nextRef->oNext = *headRef;
    *headRef = new_node;
    return nextRef;
  } else {
    return insertionSort(headRef, nextRef);
  }
}

/*
  Desc:
    Conducts the insertion sort on the struct process headRef
  Args:
    1. headRef is the process reference that we are looking at addShortFirst
    2. nextRef is the process that we are adding to the head

  Cases:
    1. if the head->oNext process is null we know:
      * that we need to compare the nextRef to head => compareBurstTime();
      * if compareBurstTime() => true

*/

struct process * insertionSort(struct process *headRef, struct process *nextRef) {

  // This will hold our current reference =?
  struct process *current = headRef;
  struct process *next = nextRef;
  // Will hold the temporary value to swap
  struct process *temp;

  // check if the headRef has any other elements  =>
  if (current->oNext == NULL) {
    // if this is true => we know that the headRef iBurstTime > nextRef
    if (compareBurstTime(current, next)) {
      // Keep going backwards and looking at previous pointer
      temp = current;
      next->oNext = temp;
    // Know the nextRef iBurstTime < headRef
    } else {
        printf("Next in WHILE: %d\n", next->iBurstTime);

        current->oNext = next;
        printf("Current: %d\n", current->iBurstTime);
        sleep(2);

        return current;
    }
  }


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
        sleep(2);
    }

}
