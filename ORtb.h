/**********************************************
*****This file specifies a hashtable for storing the packets

*****Ted 05/13/2014
***********************************************/

#ifndef cmu_fixtable_h_
#define cmu_fixtable_h_

#include "config.h"
#include "scheduler.h"
#include "packet.h"

#ifndef uint
typedef unsigned int uint;
#endif // !uint
#define DeleteEntryTime 20.0 // the time that data can be stored in sent table after 20 second it will be deleted

/* NOTE: we depend on bzero setting the booleans to ``false''
   but if false != 0, so many things are screwed up, I don't
   know what to say... */

#define candidateSize 5
#define ENTRY_VALIDIDTY 20.000001

class generalTable_ent{
 
public:

  generalTable_ent() { bzero(this, sizeof(generalTable_ent));}

    u_int8_t seqNo;
    //int candidate[5];
    nsaddr_t src;
    nsaddr_t dst;
    //nsaddr_t prehop;
    Packet *pkt;
    //bool isDelivered;
    //double timestamp;
    //double timetoforward;
    //double timetoretx;
    //int hopCount;
    //int dataTx;
    //int dataReTx;

    generalTable_ent* next;
   
};
/****************************************************************************/
class GeneralTable {

 public:
  GeneralTable();

  void insert(u_int8_t seqNo, nsaddr_t src, nsaddr_t dst, Packet *pkt);
  /* bool deleteEntry(int seqNo, nsaddr_t src, nsaddr_t dst); */
  //void deleteEntry(void);
  bool isInGeneralTable(nsaddr_t src);

  //void needReTx(generalTable_ent* entries[1000], int NumReTx);
  //void printTable(void);

  //void checkTable(generalTable_ent* entries[1000]);

  //generalTable_ent* search(int seqNo, nsaddr_t src, nsaddr_t dst);
  //void deleteNode(generalTable_ent* nodetodelete);
  //  //  sentBuffer_ent  *GetEntry(nsaddr_t src, nsaddr_t dest, int seqNo);
  /* private: */
  int         size;
  generalTable_ent *head;
  generalTable_ent *tail;
}; 

#endif

