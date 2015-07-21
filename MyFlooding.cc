/*MYFLOODING.CC
simply flooding protocol for wireless adhoc networks:
the source broadcasts the packet; 
other nodes receive this message check the packet: 
if it has not been received before, forward it; else, drop it

Ted 05/13/2014
*/
#include "MyFlooding.h"
#include "cmu-trace.h"

//bind packet header to Tcl interface
int hdr_MYFLOODING_pkt::offset_;

static class MyHeaderClass : public PacketHeaderClass {
public:
  MyHeaderClass() : PacketHeaderClass("PacketHeader/MYFLOODING", sizeof(hdr_MYFLOODING_pkt)) {
  	bind_offset(&hdr_MYFLOODING_pkt::offset_);
  }
} class_MYFLOODING_hdr;

// Export C++ class to OTcl
static class myfloodingAgentClass : public TclClass {
public:
  myfloodingAgentClass() : TclClass("Agent/MYFLOODING") {}
  TclObject* create(int argc, const char*const* argv) {
    return (new myfloodingAgent());
  }
} clas_MYFLOODING;

// Initialize function
myfloodingAgent::myfloodingAgent() : Agent(PT_MYFLOODING)
{
 // Initialize variables.
  gTable = new GeneralTable();
  pk_count = 0;
  target_ = 0;
  node = NULL;
  tracetarget = NULL;
}


//recv function
void myfloodingAgent::recv(Packet* pkt, Handler* h){
	hdr_cmn* cmh = HDR_CMN(pkt);
	hdr_ip* iph = HDR_IP(pkt);
	printf("node_%d receive packet~~~~\n",myaddr_);
	//if ( iph->saddr() == myaddr_)
	//{
	//	drop(pkt, DROP_RTR_LOOP); 
	//	return;				
	//}
	//else
	if (cmh->ptype() == PT_MYFLOODING)
	{	
		recv_myHeader(pkt);
		Packet::free(pkt);		
	}
	 else {
           	iph->ttl_--;
                if (iph->ttl_ == 0) {
                      drop(pkt, DROP_RTR_TTL);
                      return;
                 }
                 forward_data(pkt);
	} 
	
}

void myfloodingAgent::recv_myHeader(Packet* pkt){

	hdr_MYFLOODING_pkt* mh = HDR_MYFLOODING_PKT(pkt);
	
	if (gTable->isInGeneralTable(mh->pkt_src())) 
	drop(pkt, DROP_RTR_ROUTE_LOOP);
	else
	gTable->insert(mh->pkt_seq_num(), mh->pkt_src(), mh->pkt_dst(), pkt);
	forward_data(pkt);
}

// send
void myfloodingAgent::send_packet(){
	printf("myfloodingAgent::send_packet\n ");
	//new packet
	Packet 	*pkt,*copyPkt;
	pkt = allocpkt();
	hdr_cmn* cmh = HDR_CMN(pkt);
	hdr_ip* iph = HDR_IP(pkt);
	hdr_MYFLOODING_pkt* mh = HDR_MYFLOODING_PKT(pkt);
	
	cmh->ptype() = PT_MYFLOODING;	
	cmh->direction() = hdr_cmn::DOWN;
	printf("I am in send function and cmh->direction()=%d\n",cmh->direction());
	//cmh->size() = IP_HDR_LEN + mh->pkt_len();	
	cmh->error() = 0;
	//cmh->next_hop() = IP_BROADCAST;
	//cmh->addr_type() = NS_AF_INET;
	cmh->next_hop_    = IP_BROADCAST;

	//iph->saddr() = ra_addr();

	// iph->dport()= IP_BROADCAST;
	iph->daddr() = IP_BROADCAST;
	//iph->sport() = RT_PORT;
	iph->dport() = iph->sport();
	// iph->ttl() = IP_DEF_TTL;
	iph->ttl() = 255;
	iph->saddr() = here_.addr_;        

	mh->pkt_src() = myaddr_;
	mh->pkt_dst() = IP_BROADCAST;
	// mh->pkt_len() = 7;
	mh->pkt_seq_num() = seq_num_++;

 	copyPkt = pkt->copy();

	Scheduler::instance().schedule(target_, copyPkt, JITTER);
	printf("myfloodingAgent::send_packet over\n ");

}


//forward_data
void myfloodingAgent::forward_data(Packet* pkt){

	hdr_cmn* cmh = HDR_CMN(pkt);
	hdr_ip* iph = HDR_IP(pkt);
	

	 printf("Begining I am in forward_data\n");

	printf("iph->daddr()=%d iph->daddr()=%d cmh->direction()=%d, myaddr_=%d\n\n\n\n",(u_int32_t)iph->daddr(), iph->daddr() , cmh->direction(),myaddr_);
	if (cmh->direction() == hdr_cmn::UP && 
	((u_int32_t)iph->daddr() == IP_BROADCAST || iph->daddr() == myaddr_ )){
	printf("****************I am in forward_data\n");
	dmux_->recv(pkt, 0);
	return;	
	}
	else 
	{
	  printf("*******************Node_%d is in forward_data to an other node****************\n",here_.addr_);
	cmh->direction() = hdr_cmn::DOWN;
	cmh->addr_type() = NS_AF_INET;
	cmh->next_hop() = IP_BROADCAST;
	
	Scheduler::instance().schedule(target_, pkt, 0.0);
	}
}

//Export C++ Object Control Commands to OTcl.
int myfloodingAgent::command(int argc, const char*const* argv)
{  
	if (argc == 2) 
	{
		if (strcasecmp(argv[1], "send_my_message") == 0){
		printf("receive command >> send_my_message \n");
		send_packet();
		return TCL_OK;
		}
	}
	else if (argc == 3)
	{
		if (strcasecmp (argv[1], "set-addr") == 0){
		int temp = Address::instance().str2addr(argv[2]);
		myaddr_ = temp;
		return TCL_OK;
}
		if (strcasecmp (argv[1], "tracetarget") == 0) {
    			TclObject *obj;
      			if ((obj = TclObject::lookup (argv[2])) == 0) {
	  		fprintf (stderr, "%s: %s lookup of %s failed\n", __FILE__, argv[1],
		   	argv[2]);
	  		return TCL_ERROR;
     			}
      		tracetarget = (Trace *) obj;
     		return TCL_OK;
    		}

   		if (strcasecmp(argv[1], "port-dmux") == 0) {

     			TclObject *obj;

     			if ( (obj = TclObject::lookup(argv[2])) == 0) {
			fprintf(stderr, "%s: %s lookup of %s failed\n", __FILE__,
			argv[1],			
			argv[2]);
			return TCL_ERROR;
			}
			port_dmux = (NsObject *) obj;
		return TCL_OK;
    		}

  	} 

	return Agent::command(argc, argv);
}


