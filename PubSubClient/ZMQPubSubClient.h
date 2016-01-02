#ifndef _PubSubClient_ZMQPubSubClient_h_
#define _PubSubClient_ZMQPubSubClient_h_

#include <Core/Core.h>
#include "zmq.hpp"

#define	REQUEST_TIMEOUT	4000		// depends on network latency
#define	REQUEST_RETRIES 3

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
	virtual void processClientWarning(const String msg) = 0;

	private:
	void clientLoop();
	
	zmq::context_t					context;
	bool							running;
};

#endif
