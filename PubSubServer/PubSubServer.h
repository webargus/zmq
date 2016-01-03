#ifndef _PubSubServer_PubSubServer_h
#define _PubSubServer_PubSubServer_h

#include <CtrlLib/CtrlLib.h>
#include "ZMQPubSubServer.h"

using namespace Upp;

#define LAYOUTFILE <PubSubServer/PubSubServer.lay>
#include <CtrlCore/lay.h>

class PubSubServer : public ZMQPubSubServer, public WithPubSubServerLayout<TopWindow> {
public:
	typedef PubSubServer CLASSNAME;
	PubSubServer();
	void BroadcastMessage();
	
	private:
	void toggleAutoMode();
	void runAutoMode();
	virtual void processServerException(const String exc);
			
	String								sender;
	StatusBar							status;
	InfoCtrl							srvid, cnt;
	bool								inautomode;
};

#endif
