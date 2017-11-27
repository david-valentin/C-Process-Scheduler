#include <stdlib.h>
#include <sys/time.h>
#include "coursework.h"
#include <stdio.h>
#include <stdbool.h>

//Size of the linkedList
float avgResponseTime;
float avgTurnAroundTime;

bool compareBurstTime(struct process *head, struct process *next);

void print_list(struct process * head);

struct process* addSJF(struct process* headRef, struct process* newNode);

struct process* removeProcess(struct process *headRef, struct process *newNode);

int main()
{
  // Set the head to null => this is a pointer which contains an address
  // If I do *head
  // *P = pointer reference to an address A
  // p = changes p's memory
  struct process *head = NULL;

  int i;
  for (i = 0; i < NUMBER_OF_PROCESSES; i++) {
    // Generates a new process each time
    struct process *newNode = generateProcess();
    // adding values to the head and use insertion sort
    head = addSJF(head, newNode);
  }

  struct process *temp;
  for(temp = head; temp != NULL; temp = temp->oNext) {
    // Generate the new timeval struct
    struct timeval start, end;
    // Generates a new process each time
    simulateSJFProcess(temp, &start, &end);

  }

  printf("Executing print_list\n");
  // print_list(head);
}

/*
  ADDSJF
    args:
    1. headRef => the head linkedlist that we intend to returns | NULL on first go
    2. newNode => the next linkedlist item | NOT NULL on first go
    3. addMethod => a pointer to an addMethod | can be insertionSort for SJF or RoundRobin
*/
struct process * addSJF(struct process *headRef, struct process *newNode)
{
  /*
    if the headRef is null, we know:
      newNode is the first item in the list
  */
  // This will hold our current reference
  struct process *current;
  //Should never be the case...
  if (newNode == NULL) return headRef;

  // on the first iteration
  if (headRef == NULL) return newNode;

  if (headRef->oNext == NULL)
  {
    if (headRef->iBurstTime > newNode->iBurstTime)
    {
        newNode->oNext = headRef;
        headRef = newNode;
        return headRef;
    } else
    {
        headRef->oNext = newNode;
        return headRef;
    }
  }
  else
  {
    current = headRef;
    struct process *previous = headRef;
    if (current->iBurstTime >= newNode->iBurstTime)
    {
      newNode->oNext = headRef;
      headRef = newNode;
      return headRef;
    }

    while (current != NULL && current->iBurstTime < newNode->iBurstTime)
    {
      previous = current;
      current = current->oNext;
    }
    // the value before
    previous->oNext = newNode;
    newNode->oNext = current;
    return headRef;
  }
}

struct process * removeProcess(struct process *headRef, struct process *oldNode)
{
  struct process *current = headRef;

  // Case if our head is the intended node we wish to remove
  if (headRef == oldNode)
  {
    struct process *newHeadRef = headRef->oNext;
    free(headRef);
    return newHeadRef;
  }
  /// if the tail->oNext is not null and the tail processId != oldNode processId
  while (current->oNext != NULL)
  {
    if (current->oNext->iProcessId == oldNode->iProcessId)
    {
      struct process * targetNode = current->oNext;
      current->oNext = current->oNext->oNext;
      free(targetNode);
      return headRef;
    }
    current = headRef->oNext;
  }
  //Return headRef if all else if fails
  return headRef;
}

// Calculates the average response time for the linkedList
void computeAverages(struct process *headRef)
{

  simulateSJFProcess(headRef, &start, &end);
  // The linkedlist is updated
  struct process *temp;

  for(temp = headRef; temp != NULL; temp = temp->oNext) {
    // Generate the new timeval struct
    if (headRef->iState == FINISHED)
    {
      avgTurnAroundTime += temp->iBurstTime - temp->oEndTime;
      printf("Turn Around Time %2.f\n", temp->iBurstTime);
    } else {
      avgResponseTime += temp->iBurstTime;
      printf("Process iBurstTime %2.f\n", temp->iBurstTime);
    }
  }

  printf("Average turn around time: %2.f\n", avgTurnAroundTime);
  printf("Average response time: %2.f\n", avgResponseTime);

}

bool compareBurstTime(struct process *head, struct process *next)
{
    return (head->iBurstTime > next->iBurstTime);
}


void print_list(struct process *head)
{
    struct process *temp = head;
    while(temp != NULL)
    {
        printf("The value is: %d \n", temp->iBurstTime);
        temp = temp->oNext;
    }
}
