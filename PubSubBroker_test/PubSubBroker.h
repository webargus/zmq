#ifndef _PubSubBroker_PubSubBroker_h
#define _PubSubBroker_PubSubBroker_h

#include <CtrlLib/CtrlLib.h>
#include <PubSubBroker/ZMQPubSubBroker.h>

#define	BROKER_EXC_RETRIES	3

using namespace Upp;

#define LAYOUTFILE <PubSubBroker_test/PubSubBroker.lay>
#include <CtrlCore/lay.h>

class PubSubBroker : public WithPubSubBrokerLayout<TopWindow>
{
	public:
	typedef PubSubBroker CLASSNAME;
	PubSubBroker();
	void brokerStart();
	void brokerStop();
	
	private:
	ZMQPubSubBroker		broker;
	StatusBar			status;
	InfoCtrl			total;
	
	void manageBrokerException(const String exc);
	void showTransitMessage(const String msg);
};

#endif
