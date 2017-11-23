#include <stdlib.h>
#include <sys/time.h>
#include "coursework.h"
#include <stdio.h>

// The coursework library

struct process *

struct process * createLinkedListProcess(void (*adder)(struct process *head)(struct process *next)) {

  struct process *temp = generateProcess();
  if (head== NULL) {
    head = generateProcess();
  } else if (head->iBurstTime > temp->iBurstTime) {
    temp->oNext = head;
    head = temp;
  } else {

    return adder(head, temp);
  }


  // int i;
  // for(i = 0 ; i<NUMBER_OF_PROCESSES ; i++){
    // Will only be executed once
  //   if (i == 0) {
  //     head = generateProcess();
  //     temp = head
  //     addFCFS(head);
  //   // Will be executed everytime
  //   } else {
  //     temp = head;
  //     temp -> oNext = generateProcess();
  //     temp = temp->oNext;
  //     addFCFS(temp);
  //   }
  // }
  //   printf("%d \n" , temp -> iBurstTime);
  // }
}

// This does an insertion sort based off an insertion sort
void addShortFirst(struct process *head, struct process *next) {
  //Local Variables to keep track of lowest iBurstTime
  int lowestBurstTime;

  while (head->iBurstTime > next->iBurstTime) {

  }
}


int main(int argc, char const *argv[]) {
  // We have acces to these variables in the linked list:
  //  int iProcessId;
  // 	struct timeval oTimeCreated;
  // 	int iBurstTime;
  // 	struct process * oNext;
  // 	int iState;
  // 	int iEventType;
  // Call createLinkedListProcess * 2 with two different sortings

  createLinkedListProcess(addFCFS);
  return 0;
}

// This sorts the linked list based off Round Robin
// void addRR(*process) {
//   //
//
// }
