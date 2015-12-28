#ifndef _ZMQBroker_ZMQQueueBroker_h_
#define _ZMQBroker_ZMQQueueBroker_h_

#include <Core/Core.h>
#include "zmq.hpp"

using namespace Upp;

class ZMQQueueBroker
{
	public:
		typedef ZMQQueueBroker CLASSNAME;
		ZMQQueueBroker();
		void startBroker();
		
	private:
		void brokerLoop();
	
		zmq::context_t			context;
		bool					running;
};

#endif
