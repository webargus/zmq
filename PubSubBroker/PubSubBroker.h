#ifndef _PubSubBroker_PubSubBroker_h
#define _PubSubBroker_PubSubBroker_h

#include <CtrlLib/CtrlLib.h>
#include "ZMQPubSubBroker.h"

using namespace Upp;

class PubSubBroker : public ZMQPubSubBroker, public TopWindow
{
	public:
	typedef PubSubBroker CLASSNAME;
	PubSubBroker();
	void brokerStart();
	void brokerStop();
	
	private:
	Button			start, stop;
	ArrayCtrl		report;
	StatusBar		status;
	
	virtual void processTransitMessage(const String& msg);
	void showTransitMessage(const String& msg);
};

#endif
