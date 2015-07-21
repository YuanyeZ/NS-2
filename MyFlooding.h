/*This file defines packet header

Ted 05/13/2014
*/
#ifndef MyFlooding_h
#define MyFlooding_h


#include "agent.h"
#include "tclcl.h"
#include "ip.h"
#include "packet.h"
#include "trace.h"
#include "random.h"
#include "node.h"
#include "classifier-port.h"
#include "address.h"
#include "object.h" 
#include "ORtb.h" 

#define JITTER                0.11       // (sec) to jitter broadcast

#define SEND_MESSAGE(x,y,z)  send_to_dmux(prepare_message(x,y,z), 0)

//packet_header access
#define HDR_MYFLOODING_PKT(p) hdr_MYFLOODING_pkt::access(p)


// MyFlooding Agent
class myfloodingAgent;
	
class myfloodingAgent : public Agent {

  //nsaddr_t ra_addr_;
  int myaddr_;
  u_int8_t seq_num_;
  
 public:
  myfloodingAgent();
  int command(int argc, const char*const* argv);
  void recv(Packet*, Handler*);
  void recv_myHeader(Packet*);
  GeneralTable *gTable;

 protected:
  int pk_count;
  Node *node;
  PortClassifier* dmux_;
  Trace* tracetarget;
  NsObject *port_dmux;
  
  //inline nsaddr_t& ra_addr() { return ra_addr_; }

   
  void send_packet();
  void forward_data(Packet* pkt);
  
  void trace(char *fmt,...);

};


//MyFlooding_pkt_header
struct hdr_MYFLOODING_pkt {
  char payload[512];
	nsaddr_t	pkt_src_;
	nsaddr_t	pkt_dst_;
	u_int16_t	pkt_len_;
	u_int8_t	pkt_seq_num_;

	inline		nsaddr_t&	pkt_src() {return pkt_src_;}
	inline		nsaddr_t&	pkt_dst() {return pkt_dst_;}
	inline		u_int16_t&	pkt_len() {return pkt_len_;}
	inline		u_int8_t&	pkt_seq_num() {return pkt_seq_num_;}

	static int offset_;
	inline static int& offset() {return offset_;}
	inline static hdr_MYFLOODING_pkt* access (const Packet* p)
	{	
		return (hdr_MYFLOODING_pkt*)p -> access (offset_);
	}
	
};


#endif


