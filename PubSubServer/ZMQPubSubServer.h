#ifndef _PubSubServer_ZMQPubSubServer_h_
#define _PubSubServer_ZMQPubSubServer_h_

#include <Core/Core.h>
#include "zmq.hpp"

using namespace Upp;

class ZMQPubSubServer
{
	public:
	typedef ZMQPubSubServer CLASSNAME;
	ZMQPubSubServer();
	
	protected:
	void sendMessage(const String& msg);
	
	private:
	void publisherLoop(const String& msg);
	
	zmq::context_t				context;
	zmq::socket_t publisher;

};

#endif
