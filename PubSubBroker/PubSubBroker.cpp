#include "PubSubBroker.h"

PubSubBroker::PubSubBroker() : broker("5559", "5560")
{
	CtrlLayout(*this, "ZMQ PUB/SUB Broker");
	AddFrame(status);
	status.AddFrame(total.Right(160));
	status = String("frontend port: ").Cat() << broker.getServerPort() <<
			 "; backend port: " << broker.getClientPort();
	report.AddColumn("raw msg");
	errboard.AddColumn("fault report");
	start <<= THISBACK(brokerStart);
	stop  <<= THISBACK(brokerStop);
	broker.WhenBrokerException = THISBACK(manageBrokerException);
	broker.WhenTransitMessage  = THISBACK(showTransitMessage);
}

void PubSubBroker::manageBrokerException(const String exc)
{
	static int cnt = BROKER_EXC_RETRIES;

	stop.Disable();
	errboard.Add(exc);
	if(cnt-- == 0) {
		if(PromptAbortRetry(Format("Couldn't recover from fault after %d attempts",
								   BROKER_EXC_RETRIES))) {
			PostCallback(callback(this, &TopWindow::Close));		// close window!
			return;
		}
		cnt = BROKER_EXC_RETRIES;
		errboard.Clear();
	}
	
	errboard.Add("will restart broker...");
	DUMP(exc);
	Sleep(500);
	brokerStart();
	errboard.Add("...restarted broker.");
}

void PubSubBroker::brokerStart()
{
	start.Disable();
	stop.Enable();
	broker.startBroker();
}

void PubSubBroker::brokerStop()
{
	start.Enable();
	stop.Disable();
	broker.stopBroker();
}

void PubSubBroker::showTransitMessage(const String msg)
{
	static int msgcnt = 0;
	if((msgcnt % 500) == 0)
		report.Clear();
	total = Format("transit: %d", ++msgcnt);
	report.Insert(0, Vector<Value>() << msg);
}

GUI_APP_MAIN
{
	PubSubBroker().Run();
}











