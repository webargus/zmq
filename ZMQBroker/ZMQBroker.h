#ifndef _ZMQBroker_ZMQBroker_h
#define _ZMQBroker_ZMQBroker_h

#include <CtrlLib/CtrlLib.h>
#include "ZMQQueueBroker.h"

using namespace Upp;

#define LAYOUTFILE <ZMQBroker/ZMQBroker.lay>
#include <CtrlCore/lay.h>

class ZMQBroker : public ZMQQueueBroker, public WithZMQBrokerLayout<TopWindow> {
public:
	typedef ZMQBroker CLASSNAME;
	ZMQBroker();
	void startQueueBroker();
};

#endif
