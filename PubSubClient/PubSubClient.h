#ifndef _PubSubClient_PubSubClient_h
#define _PubSubClient_PubSubClient_h

#include <CtrlLib/CtrlLib.h>
#include "ZMQPubSubClient.h"

using namespace Upp;

#define LAYOUTFILE <PubSubClient/PubSubClient.lay>
#include <CtrlCore/lay.h>

class PubSubClient : public WithPubSubClientLayout<TopWindow> {
public:
	typedef PubSubClient CLASSNAME;
	PubSubClient();
	//~PubSubClient() { PromptOK("parent destructor"); stopClient(); } // avoids pure virtual call crashes on window close
	
	private:
	void ManageClientMessage(const String msg);
	void ManageClientWarning(const String msg);
	void ManageClientException(const String exc);
	void ClientStart();
	void ClientStop();
	void ClearGUI();
	
	ZMQPubSubClient				client;
	StatusBar					status;
};

#endif
