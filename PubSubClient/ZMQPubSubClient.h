#ifndef _PubSubClient_ZMQPubSubClient_h_
#define _PubSubClient_ZMQPubSubClient_h_

#include <CtrlLib/CtrlLib.h>
#include "zmq.hpp"

#define	REQUEST_TIMEOUT	1500		// depends on network latency
#define	REQUEST_RETRIES 3

using namespace Upp;

class ZMQPubSubClient
{
	public:
	typedef ZMQPubSubClient CLASSNAME;
	ZMQPubSubClient();
	~ZMQPubSubClient() { stopClient(); }
	void startClient();
	void stopClient();

	Callback1<String>					WhenMessage, WhenException, WhenWarning;

	private:
	void clientLoop();
	void processClientMessage(const String msg);
	void processClientException(const String exc);
	void processClientWarning(const String msg);
	
	Thread							worker;
	zmq::context_t					context;
	bool							running;
};

#endif
