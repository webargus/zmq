#include "ZMQBroker.h"

ZMQBroker::ZMQBroker()
{
	CtrlLayout(*this, "Broker");
	start <<= THISBACK(startQueueBroker);
}

void ZMQBroker::startQueueBroker()
{
	start.Disable();
	startBroker();
}

GUI_APP_MAIN
{
	ZMQBroker().Run();
}
