#ifndef _PubSubServer_ZMQPubSubServer_h_
#define _PubSubServer_ZMQPubSubServer_h_

#include <CtrlLib/CtrlLib.h>
#include "zmq.hpp"

using namespace Upp;

class ZMQPubSubServer
{
	public:
	typedef ZMQPubSubServer CLASSNAME;
	ZMQPubSubServer();
	ZMQPubSubServer(const String& srv, const String& sp, const String& lp = "5555");
	void sendMessage(const String msg);
	void stopServer();
	void processServerException(const String exc);
	void setServerParams(const String& srv, const String& sp);
	void setLocalPort(const String& lp) { lclport = lp; }
	const String& getServerPort() { return srvport; }
	const String& getLocalPort()  { return lclport; }
	const String& getServerName() { return server; }
	
	Callback1<String>			WhenException;
	private:
	void publisherLoop();
	
	String						srvport, lclport, server;
	zmq::context_t				context;
	Thread						worker;
	bool						running;
};

#endif
