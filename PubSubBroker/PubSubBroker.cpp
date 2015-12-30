#include "PubSubBroker.h"

PubSubBroker::PubSubBroker()
{
	Title("ZMQ PUB/SUB Broker");
	SetRect(0, 0, 360, 240);
	Add(report.HSizePos(4, 4).VSizePos(34, 34));
	report.AutoHideSb();
	report.AddColumn("raw msg");
	start.SetLabel("start");
	stop.SetLabel("stop");
	Add(start.HSizePos(40, 40).TopPos(10));
	Add(stop.HSizePos(40, 40).BottomPos(10));
	stop.Disable();
	start <<= THISBACK(brokerStart);
	stop  <<= THISBACK(brokerStop);
}

void PubSubBroker::brokerStart()
{
	report.Clear();
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
	report.Add(msg);
}

GUI_APP_MAIN
{
	PubSubBroker().Run();
}











