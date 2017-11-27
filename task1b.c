#include <stdlib.h>
#include <sys/time.h>
#include "coursework.h"
#include <stdio.h>
#include <stdbool.h>


//GLOBAL VARIABLE TAIL
struct process* TAIL;

float avgResponseTime = 0.0;
float avgTurnAroundTime = 0.0;

void print_list(struct process * head);

struct process * addRR(struct process *headRef, struct process *newNode, struct process *tail);

struct process * traverseList(struct process *headRef);

struct process* removeProcess(struct process *headRef, struct process *newNode);

void computeAvgs(struct process *headRef, struct timeval start, struct timeval end);

int main()
{
  struct process *head = NULL;

  int i;
  for (i = 0; i < NUMBER_OF_PROCESSES; i++) {
    // If its the last process being added than add the tail pointer to the newNode
    struct process *newNode = generateProcess();
    head = addRR(head, newNode, TAIL);
  }

  // print_list(head);

  // The linkedlist is updated
  struct process *temp;
  for(temp = head; temp != NULL; temp = temp->oNext) {

    // Instantiate two timeval structs
    struct timeval start, end;
    // Generates a new process each time
    printf("HEAD %d\n", temp->iBurstTime);
    computeAvgs(temp, start, end);
  }

}

// // Calculates the average response time for the linkedList
void computeAvgs(struct process *headRef, struct timeval start, struct timeval end)
{
  //response time: amount of time it takes from when a request is submitted to when the response is produced (minimize); does not include the time for a response to be output
  // turnaround time (TA): total amount of time to execute a particular process (minimize)
  // Turnaroudn time = 0TimeCreated +
  // | *oTimeCreated ------ | *oStartTime ---cpu_usage--- | *endProcess = Turnaround time
  // Response time
  // | *oTimeCreated ------- | *oStartTime


  // The linkedlist is updated
  struct process *current = headRef;
  long int previousBurstime;
  long int turnaroundTime;
  long int responseTime;

  while (current != NULL) {
    headRef = headRef->oNext;

    previousBurstime = current->iBurstTime;
    simulateRoundRobinProcess(current, &start, &end);
    turnaroundTime = getDifferenceInMilliSeconds(current->oTimeCreated, end);
    responseTime = getDifferenceInMilliSeconds(current->oTimeCreated, start);

    printf("Process Id = %d, Previous Burst Time = %d, New Burst Time = %d, ", current->iProcessId, previousBurstime, current->iBurstTime);
    if (current->iState == NEW) {
      printf("Response Time = %d, \n", responseTime);
      avgResponseTime += responseTime;
    }
    if (current->iState == FINISHED) {
      current = removeProcess(headRef, current);
      printf("Turn Around Time = %d\n ", turnaroundTime);
      avgTurnAroundTime += turnaroundTime;
      // Need an else case because the process might try to stick it in before the beginning of the linked list
    } else {
      printf("IT HIT HERE \n");
    }
  }
  avgTurnAroundTime = avgTurnAroundTime/NUMBER_OF_PROCESSES;
  avgResponseTime = avgResponseTime/NUMBER_OF_PROCESSES;
  printf("Average response time = %2.f \n", avgResponseTime);
  printf("Average turn around time = %2.f \n", avgTurnAroundTime);
}

/*
  addRR
    args:
    1. headRef => the head linkedlist that we intend to returns | NULL on first go
    2. newNode => the next linkedlist item | NOT NULL on first go
*/
struct process * addRR(struct process *headRef, struct process *newNode, struct process *tail)
{
  // This will hold our current reference
  struct process *current;
  //Should never be the case...
  if (newNode == NULL) return headRef;

  if (headRef == NULL) { // list is empty
      headRef = newNode;

      tail = newNode;
      return headRef;
  } else
  {
    tail = traverseList(headRef);
    tail->oNext = newNode;
    tail = newNode;
    return headRef;
  }

}

/*
  Function to traverse to the end of the list
*/
struct process * traverseList(struct process *headRef)
{
  struct process *next = headRef;
  while (next->oNext != NULL)
  {
    next = next->oNext;
  }
  return next;

}

/*
  A function to remove a process in the linkedlist
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
    // if the current->iProcessId equal the oldNode->iProcessId
    if (current->oNext->iProcessId == oldNode->iProcessId)
    {
      struct process * targetNode = current->oNext;
      current->oNext = current->oNext->oNext;
      free(targetNode);
      return headRef;
    }
    current = current->oNext;
  }
  //Return headRef if all else if fails
  return headRef;
}

/*
  A util function to print out the list of processes
*/
void print_list(struct process *head)
{
    struct process *temp = head;
    while(temp != NULL)
    {
        printf("The value is: %d \n", temp->iBurstTime);
        temp = temp->oNext;
    }
}
