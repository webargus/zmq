#ifndef _PubSubBroker_ZMQPubSubBroker_h_
#define _PubSubBroker_ZMQPubSubBroker_h_

#include <Core/Core.h>
#include "zmq.hpp"

using namespace Upp;

class ZMQPubSubBroker
{
	public:
	typedef ZMQPubSubBroker CLASSNAME;
	ZMQPubSubBroker();
	void startBroker();
	void stopBroker();
	
	protected:
	virtual void processTransitMessage(const String& msg) = 0;
	virtual void processBrokerException(const String& msg) = 0;
		
	private:
	void brokerLoop();
	zmq::context_t				context;
	bool						running;
};

#endif
