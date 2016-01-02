#include "PubSubBroker.h"

PubSubBroker::PubSubBroker()
{
	CtrlLayout(*this, "ZMQ PUB/SUB Broker");
	AddFrame(status);
	report.AddColumn("raw msg");
	errboard.AddColumn("fault report");
	start <<= THISBACK(brokerStart);
	stop  <<= THISBACK(brokerStop);
}

void PubSubBroker::processBrokerException(const String& exc)
{
	PostCallback(THISBACK1(manageBrokerException, exc));
}

void PubSubBroker::manageBrokerException(const String exc)
{
	stop.Disable();
	stopBroker();		// lower 'running' flag
	errboard.Add(exc);
	errboard.Add("will restart broker...");
	DUMP(exc);
	Sleep(500);
	brokerStart();
	errboard.Add("...restarted client.");
}

void PubSubBroker::brokerStart()
{
	start.Disable();
	stop.Enable();
	startBroker();
}

void PubSubBroker::brokerStop()
{
	start.Enable();
	stop.Disable();
	stopBroker();
}

void PubSubBroker::processTransitMessage(const String& msg)
{
	PostCallback(THISBACK1(showTransitMessage, msg));
}

void PubSubBroker::showTransitMessage(const String& msg)
{
	static int msgcnt = 0;
	if((msgcnt % 500) == 0)
		report.Clear();
	status = Format("transit: %d", ++msgcnt);
	report.Insert(0, Vector<Value>() << msg);
}

GUI_APP_MAIN
{
	PubSubBroker().Run();
}











