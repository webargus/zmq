#ifndef _PubSubClient_ZMQPubSubClient_h_
#define _PubSubClient_ZMQPubSubClient_h_

#include <Core/Core.h>
#include "zmq.hpp"

using namespace Upp;

class ZMQPubSubClient
{
	public:
	typedef ZMQPubSubClient CLASSNAME;
	ZMQPubSubClient();
	void startClient();
	void stopClient();
	
	protected:
	virtual void processClientMessage(const String msg) = 0;
	virtual void processClientException(const String exc) = 0;

	private:
	void clientLoop();
	
	zmq::context_t					context;
	bool							running;
};

#endif
