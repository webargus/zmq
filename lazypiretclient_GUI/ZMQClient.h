#ifndef _lazypiretclient_GUI_ZMQClient_h_
#define _lazypiretclient_GUI_ZMQClient_h_

#include <Core/Core.h>
#include "zmq.hpp"

#define REQUEST_TIMEOUT     2500    //  msecs, (> 1000!)
#define REQUEST_RETRIES     3       //  Before we abandon

using namespace Upp;

class ZMQClient {
	public:
	
	ZMQClient();
	void sendMessage(String msg);
	
	protected:
	virtual void processReportMessage(const String& rep) = 0;
	virtual void processClientMessage(const String& msg) = 0;
	
	private:
	//  Helper function that returns a new configured socket
	//
	zmq::socket_t * s_client_socket ();
	
	zmq::context_t context;
};

#endif
