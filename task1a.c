#include <stdlib.h>
#include <sys/time.h>
#include "coursework.h"
#include <stdio.h>
#include <stdbool.h>

//GLOBAL VARIABLE TAIL
struct process* TAIL;
bool ITERATED = false;

float AvgResponseTime = 0.0;
float AvgTurnAroundTime = 0.0;

struct process * computeAvgs(struct process *headRef, struct timeval start, struct timeval end);

bool compareBurstTime(struct process *head, struct process *next);

void printList(struct process * head);

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

  while (head != NULL) {
    struct timeval start, end;
    // computeAvgs should return the new head (note it may just be the same head)
    head = computeAvgs(head, start, end);
  }

  // Calculated at the end of the loop
  AvgTurnAroundTime = AvgTurnAroundTime/NUMBER_OF_PROCESSES;
  AvgResponseTime = AvgResponseTime/NUMBER_OF_PROCESSES;
  printf("Average response time = %f \n", AvgResponseTime);
  printf("Average turn around time = %f \n", AvgTurnAroundTime);
}

/*
  This function computes:
    1. the AvgTurnAroundTime and the AvgResponseTime for the
  all the processes in headRef
    2. Print out the turn around time and the AvgResponseTime for each process
    depending on the order
*/
struct process * computeAvgs(struct process *headRef, struct timeval start, struct timeval end)
{
  //response time: amount of time it takes from when a request is submitted to when the response is produced (minimize); does not include the time for a response to be output
  // turnaround time (TA): total amount of time to execute a particular process (minimize)
  // Turnaroudn time = 0TimeCreated +
  // | *oTimeCreated ------ | *oStartTime ---cpu_usage--- | *endProcess = Turnaround time
  // Response time
  // | *oTimeCreated ------- | *oStartTime


  // The linkedlist is updated
  struct process *current = headRef;
  struct process *temp;
  int previousBurstime;
  float turnaroundTime = 0.0;
  float responseTime = 0.0;

  while (current != NULL) {
    temp = current->oNext;

    previousBurstime = current->iBurstTime;
    /*
      This is an external function in coursework.c that simulates a process being
    scheduled and takes a timeval structs which generates times for the process.
    */
    simulateSJFProcess(current, &start, &end);

    // This is called to simply calculate the turnaroundTime time
    turnaroundTime = getDifferenceInMilliSeconds(current->oTimeCreated, end);

    // This is called to simply calculate the responseTime time
    responseTime = getDifferenceInMilliSeconds(current->oTimeCreated, start);

    // Always print this
    printf("Process Id = %d, Previous Burst Time = %d, New Burst Time = %d, ", current->iProcessId, previousBurstime, current->iBurstTime);

    // This is called to simply calculate the turnaroundTime time
    turnaroundTime = getDifferenceInMilliSeconds(current->oTimeCreated, end);

    // This is called to simply calculate the responseTime time
    responseTime = getDifferenceInMilliSeconds(current->oTimeCreated, start);

    // Always print this
    printf("Process Id = %d, Previous Burst Time = %d, New Burst Time = %d", current->iProcessId, previousBurstime, current->iBurstTime);
    if (ITERATED == false) {
      printf(", Response Time = %f", responseTime);
      AvgResponseTime += responseTime;
    }
    if (current->iState == FINISHED) {
      printf(", Turn Around Time = %f", turnaroundTime);
      AvgTurnAroundTime += turnaroundTime;
      headRef = removeProcess(headRef, current);
    }
    printf("\n");
    current = temp;
  }
  ITERATED = true;
  return headRef;
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

/*
  A function to remove a process from a incoming headRef
*/
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

/*
  A function to compare the iBurstTime of the headRef and the other struct process
*/
bool compareBurstTime(struct process *head, struct process *next)
{
    return (head->iBurstTime > next->iBurstTime);
}

/*
  A util function to print out the list of processes
*/
void printList(struct process *head)
{
    struct process *temp = head;
    while(temp != NULL)
    {
        printf("The value is: %d \n", temp->iBurstTime);
        temp = temp->oNext;
    }
}

/*
  A util function to check if a process is null and print out the
  associated name
*/
bool checkNull(struct process *target, char *name)
{
  if (target) {
    printf("%s IS NOT NULL \n", name);
    return true;
  } else {
    printf("%s IS NULL \n", name);
    return false;
  }
}
