#ifndef _PubSubBroker_PubSubBroker_h
#define _PubSubBroker_PubSubBroker_h

#include <CtrlLib/CtrlLib.h>
#include "ZMQPubSubBroker.h"

using namespace Upp;

#define LAYOUTFILE <PubSubBroker/PubSubBroker.lay>
#include <CtrlCore/lay.h>

class PubSubBroker : public ZMQPubSubBroker, public WithPubSubBrokerLayout<TopWindow>
{
	public:
	typedef PubSubBroker CLASSNAME;
	PubSubBroker();
	void brokerStart();
	void brokerStop();
	
	private:
	StatusBar		status;
	
	virtual void processTransitMessage(const String& msg);
	virtual void processBrokerException(const String& exc);
	void manageBrokerException(const String exc);
	void showTransitMessage(const String& msg);
};

#endif
