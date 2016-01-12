#ifndef _PubSubBroker_ZMQPubSubBroker_h_
#define _PubSubBroker_ZMQPubSubBroker_h_

#include <CtrlLib/CtrlLib.h>
#include "zmq.hpp"

using namespace Upp;

class ZMQPubSubBroker
{
	public:
	typedef ZMQPubSubBroker CLASSNAME;
	ZMQPubSubBroker();
	ZMQPubSubBroker(const String& srvport, const String& cltport);
	void startBroker();
	void stopBroker();
	bool isRunning() { return running; }
	void setBrokerPorts(const String& sp, const String& cp);
	const String& getServerPort() { return srvport; }
	const String& getClientPort() { return cltport; }
	
	Callback1<String>				WhenTransitMessage, WhenBrokerException;
		
	private:
	void brokerLoop();
	void processTransitMessage(const String msg);
	void processBrokerException(const String msg);
	
	String						srvport, cltport;
	zmq::context_t				context;
	bool						running;
};

#endif
