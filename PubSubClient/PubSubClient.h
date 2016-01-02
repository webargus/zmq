#ifndef _PubSubClient_PubSubClient_h
#define _PubSubClient_PubSubClient_h

#include <CtrlLib/CtrlLib.h>
#include "ZMQPubSubClient.h"

using namespace Upp;

#define LAYOUTFILE <PubSubClient/PubSubClient.lay>
#include <CtrlCore/lay.h>

class PubSubClient : public ZMQPubSubClient, public WithPubSubClientLayout<TopWindow> {
public:
	typedef PubSubClient CLASSNAME;
	PubSubClient();
	
	private:
	virtual void processClientMessage(const String msg);
	virtual void processClientException(const String exc);
	void manageClientException(const String exc);
	void addMessageToHistory(const String msg);
	void ClientStart();
	void ClientStop();
	
	StatusBar					status;
};

#endif
