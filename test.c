#include <stdlib.h>
#include <sys/time.h>
#include "coursework.h"
#include <stdio.h>
#include <stdbool.h>


//GLOBAL VARIABLE TAIL
struct process* TAIL;
bool ITERATED = false;

float avgResponseTime = 0.0;
float avgTurnAroundTime = 0.0;

void printList(struct process * head);

bool checkNull(struct process *target, char *name);

struct process * addRR(struct process *headRef, struct process *newNode);

struct process * traverseList(struct process *headRef);

struct process* removeProcess(struct process *headRef, struct process *newNode);

struct process * computeAvgs(struct process *headRef, struct timeval start, struct timeval end);

int main()
{
  struct process *head = NULL;

  int i;
  for (i = 0; i < NUMBER_OF_PROCESSES; i++) {
    // If its the last process being added than add the tail pointer to the newNode
    struct process *newNode = generateProcess();
    head = addRR(head, newNode);
  }

  /*
    This for loop sets the iBurstTime for all the processes that we generated
    from the previous for loop
  */
  // struct process *temp;
  // for(temp = head; temp != NULL; temp = temp->oNext) {
  //   // Instantiate two timeval structs
  //   struct timeval start, end;
  //   // Generates a new process each time
  //   computeAvgs(temp, start, end);
  // }

  // What Chris thinks should happen
  while (head != NULL) {
    struct timeval start, end;

  // computeAvgs should return the new head (note it may just be the same head)
    head = computeAvgs(head, start, end);
  }

  // Calculated at the end of the loop
  avgTurnAroundTime = avgTurnAroundTime/NUMBER_OF_PROCESSES;
  avgResponseTime = avgResponseTime/NUMBER_OF_PROCESSES;
  printf("Average response time = %f \n", avgResponseTime);
  printf("Average turn around time = %f \n", avgTurnAroundTime);

}

/*
  This function computes:
    1. the avgTurnAroundTime and the avgResponseTime for the
  all the processes in headRef
    2. Print out the turn around time and the avgResponseTime for each process
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
    simulateRoundRobinProcess(current, &start, &end);

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
      avgResponseTime += responseTime;
    }
    if (current->iState == FINISHED) {
      printf(", Turn Around Time = %f", turnaroundTime);
      avgTurnAroundTime += turnaroundTime;
      headRef = removeProcess(headRef, current);
    }
    printf("\n");
    current = temp;
  }
  ITERATED = true;
  return headRef;
}

/*
  addRR
    args:
    1. headRef => the head linkedlist that we intend to returns | NULL on iteration
    2. newNode => the next linkedlist item | NOT NULL on iteration
*/
struct process * addRR(struct process *headRef, struct process *newNode)
{
  // This will hold our current reference
  struct process *current;
  //Should never be the case, but just in case
  if (newNode == NULL) return headRef;

  if (headRef == NULL) { // list is empty
      headRef = newNode;
      TAIL = newNode;
      return headRef;
  } else
  {
    /*
      Chris, the issue seems to be in here.

      Basically, tail->oNext segfaults and I think its because my TAIL doesn't have
      anything to point to essentially.

      tail = traverseList(headRef); returns NULL


    */
    TAIL = traverseList(headRef);
    TAIL->oNext = newNode;
    TAIL = newNode;
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
  // next will be null for some reason
  return next;
}

/*
  A function to remove a process in the linkedlist
*/
struct process * removeProcess(struct process *headRef, struct process *oldNode)
{
  struct process *current = headRef;

  // Case if our head is the intended node we wish to remove
  if (headRef->iProcessId == oldNode->iProcessId)
  {
    struct process *newHeadRef = headRef->oNext;
    free(headRef);
    return newHeadRef;
  }
  /// if the tail->oNext is not null and the tail processId != oldNode processId
  while (current->oNext != NULL)
  {
    // if the current->iProcessId equal the oldNode->iProcessId
    // if its in the middle
    if (current->oNext->iProcessId == oldNode->iProcessId)
    {
      struct process * targetNode = current->oNext;
      // Reassign the next current
      current->oNext = current->oNext->oNext;
      //Changed it here
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
