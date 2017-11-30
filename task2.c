#include "coursework.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>
#include <stdbool.h>



struct process *Bounded_Buffer_Head;
// Semaphores for my bounded buffer - 1 to indicate full and 1 to indicate empty. Need a semaphore protectSem to
sem_t fullSem;
sem_t emptySem;
sem_t protectSem;
// Threads for producerTidand the consumer
pthread_t producerTid;
pthread_t consumerTid;
// Variable to pass into the semaphore to indicate an empty variable
int EMPTY = 0;
// Number of iterations to run the process scheduling
int ITERATIONS = 1000;
// To keep track of the number of processes consumed and generated
int NUM_CONSUMED = 0;
int NUM_PRODUCED = 0;
// Number of processes in the BUFFER => should never be more than BUFFER SIZE i.e. 5
int BUFFER_COUNT = 0;
// Keeps track whether we iterated through the Bounded Buffer the first time or not
bool ITERATED = false;
// TAIL of our list
struct process* TAIL;
// The max wait in case a process deadlocks
struct timespec MAX_WAIT;
// Float values to calculate the avg response time and avg turn around time
float AvgResponseTime = 0.0;
float AvgTurnAroundTime = 0.0;

bool running = true;


/*
  Functions
*/
struct process * addSJF(struct process *headRef, struct process *newNode);

struct process * removeProcess(struct process *headRef, struct process *oldNode);

void * consumer(void *arg);

void * producer(void *arg);

bool initializeSemaphores();

// bool initializeConsumerProducer();
void initializeConsumerProducer();


void printList(struct process *head);

struct process * computeAvgs(struct process *headRef, struct timeval start, struct timeval end);


// The producerTidcan only add processes to the buffer if spaces are available, the consumer can only remove processes from the buffer if processes are available. The producerTidand consumer must run in separate threads and with maximum parallelism.
int main()
{
  // Set the bounded buffer to null
  Bounded_Buffer_Head = NULL;

  // Initialize sempahores
  initializeSemaphores();
  // Initialize the Consumer and producerTidthreads
  initializeConsumerProducer();

  // Turn off running
  // Join the threads
  pthread_join(producerTid,NULL);
  pthread_join(consumerTid,NULL);

  int totalProduced = NUM_PRODUCED;
  int totalConsumed = NUM_CONSUMED;
  printf("Produced: %d, Consumed: %d \n",totalProduced,totalConsumed);
  // Calculated at the end of the loop
  AvgTurnAroundTime = AvgTurnAroundTime/ITERATIONS;
  AvgResponseTime = AvgResponseTime/ITERATIONS;
  printf("Average response time = %f \n", AvgResponseTime);
  printf("Average turn around time = %f \n", AvgTurnAroundTime);
  // running = false;
  return 1;

  // Bounded_Buffer_Head - should be organized by
  // while there is space in the boundedBuffer
  // Add a process
  // Otherwise keep checking if the boundedBuffer has space available
}

/*
  A function to output the text results
*/
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
    printf("Process Id = %d, Previous Burst Time = %d, New Burst Time = %d, Response Time = %f", current->iProcessId, previousBurstime, current->iBurstTime, responseTime);
    // Always print this
    if (ITERATED == false) {
      printf(", Response Time = %f", responseTime);
      AvgResponseTime += responseTime;
    }
    if (current->iState == FINISHED) {
      printf(", Turn Around Time = %f", turnaroundTime);
      AvgTurnAroundTime += turnaroundTime;
      headRef = removeProcess(headRef, current);
      // INCREMENT NUM_CONSUMED and BUFFER_COUNT
      ++NUM_CONSUMED;
      BUFFER_COUNT = NUM_PRODUCED - NUM_CONSUMED;
      // Signal the emptySem and FullSem and Protect Sem that space is available
    }
    printf("\n");
    current = temp;
  }
  ITERATED = true;
  return headRef;
}


bool initializeSemaphores()
{

  if (sem_init(&fullSem, 0, BUFFER_SIZE) < 0)
  {
    printf("The full semaphore initialization failed. \n");
    return false;
  }

  if (sem_init(&emptySem, 0, 0) < 0)
  {
    printf("The empty semaphore initialization failed. \n");
    return false;
  }

  if (sem_init(&protectSem, 0, 1) < 0)
  {
    printf("The protect count semaphore initialization failed. \n");
    return false;
  }
  return true;
}

/*
  Initialize the producerTid and consumers =>
    returns true if initialized properly
    returns false if initialized properly
*/
void initializeConsumerProducer()
{
  if(pthread_create(&producerTid, NULL, &producer, NULL) != 0){
    printf("\n Can't create the producer thread \n");
    exit(1);
    // return false;
  }

  if(pthread_create(&consumerTid, NULL, &consumer, NULL)!= 0){
      printf("\n Can't create the consumer thread\n");
      pthread_join(producerTid, NULL);
      running = 0;
      exit(1);
      // return false;
  }
  // return true;
}


/*
  Initialize the producer function
*/
void * producer(void *arg)
{
  printf("\nStarting Producer\n");

  // while(running && ITERATIONS < 0)
  // {
  int i;
  for (i = 0; i < NUMBER_OF_PROCESSES; i++ )
  {
    sem_wait(&emptySem);
    sem_wait(&protectSem);
    // Sort by SJ
    struct process *newProcess = generateProcess();
    // printf("Process ID: %d\n", newProcess->iProcessId);
    Bounded_Buffer_Head = addSJF(Bounded_Buffer_Head, newProcess);
    NUM_PRODUCED++;
    sem_post(&protectSem);
    sem_post(&fullSem);
  }


  printf("\nStopping Producer\n");
}

/*
  A function to create the consumer thread
*/
void * consumer(void *arg)
{
  printf("\nStarting Consumer\n");
  int i;
  for (i = 0; i < NUMBER_OF_PROCESSES; i++)
  {
      sem_wait(&fullSem);
      sem_wait(&protectSem);
      struct timeval start, end;
      Bounded_Buffer_Head = computeAvgs(Bounded_Buffer_Head, start, end);
      sem_post(&emptySem);
      sem_post(&protectSem);
  }
  printf("\nStopping Consumer\n");
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
  A function to remove a node from a incoming headRef
*/
struct process * removeProcess(struct process *headRef, struct process *oldNode)
{
  struct process *current = headRef;

  // Case if our head is the intended node we wish to remove
  if (headRef == oldNode)
  {
    struct process *newHeadRef = headRef->oNext;
    free(headRef);
    NUM_CONSUMED++;
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
      NUM_CONSUMED++;
      return headRef;
    }
    current = headRef->oNext;
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
