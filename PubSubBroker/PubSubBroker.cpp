#include "PubSubBroker.h"

PubSubBroker::PubSubBroker()
{
	Title("ZMQ PUB/SUB Broker");
	SetRect(0, 0, 380, 260);
	AddFrame(status);
	Add(report.HSizePos(4, 4).VSizePos(34, 64));
	report.AutoHideSb();
	report.AddColumn("raw msg");
	start.SetLabel("start");
	stop.SetLabel("stop");
	Add(start.HSizePos(40, 40).TopPos(10));
	Add(stop.HSizePos(40, 40).BottomPos(30));
	stop.Disable();
	start <<= THISBACK(brokerStart);
	stop  <<= THISBACK(brokerStop);
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











