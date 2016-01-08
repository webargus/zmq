#ifndef _PubSubClient_PubSubClient_h
#define _PubSubClient_PubSubClient_h

#include <CtrlLib/CtrlLib.h>
#include <PubSubClient/ZMQPubSubClient.h>

using namespace Upp;

#define LAYOUTFILE <PubSubClient_test/PubSubClient.lay>
#include <CtrlCore/lay.h>

class PubSubClient : public WithPubSubClientLayout<TopWindow> {
public:
	typedef PubSubClient CLASSNAME;
	PubSubClient();
	
	private:
	void ManageClientMessage(const String msg);
	void ManageClientWarning(const String msg);
	void ManageClientException(const String exc);
	void ClientStart();
	void ClientStop();
	void ClearGUI();
	
	ZMQPubSubClient				client;
	StatusBar					status;
	InfoCtrl					total;
};

#endif
