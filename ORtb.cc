/**********************************************
*****This is the implementation of ORtb.h

*****Ted 05/13/2014
*****************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "ORtb.h"





GeneralTable::GeneralTable() {
  // Let's start with a 100 element maxelts.
  size = 0;
  head = NULL;
  tail = NULL;
}

void GeneralTable::insert(u_int8_t seqNo, nsaddr_t src, nsaddr_t dst, Packet *pkt){
  
  generalTable_ent *newNode = new generalTable_ent; 
  newNode->seqNo = seqNo;
  newNode->src = src;
  newNode->dst = dst;
  newNode->pkt = pkt;
  
  if (size == 0) {
    head = newNode;
    tail = newNode;
  }
  else {
    tail->next = newNode;
    tail = newNode;
  }
  size++;
}

bool GeneralTable::isInGeneralTable(nsaddr_t src){
  
  generalTable_ent* current;
  
  current = head;
  while (current != NULL) {
    if (current->src == src)	return TRUE;
    current = current->next;
  }
  return FALSE;
}


