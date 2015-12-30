#include "PubSubClient.h"

PubSubClient::PubSubClient()
{
	CtrlLayout(*this, "ZMQ PUB/SUB Client");
	AddFrame(status);
	history.AddColumn("sender");
	history.AddColumn("message");
	clear <<= callback(&history, &ArrayCtrl::Clear);
	start <<= THISBACK(ClientStart);
	stop  <<= THISBACK(ClientStop);
	stop.Disable();
}

void PubSubClient::ClientStart()
{
	start.Disable();
	stop.Enable();
	startClient();
}

void PubSubClient::ClientStop()
{
	stop.Disable();
	start.Enable();
	stopClient();
}

void PubSubClient::processClientMessage(const String& msg)
{
	PostCallback(THISBACK1(addMessageToHistory, msg));
}

void PubSubClient::addMessageToHistory(const String& msg)
{
	static int msgcnt = 0;
	if((msgcnt % 500) == 0)
		history.Clear();
	Value js = ParseJSON(msg);
	history.Insert(0, Vector<Value>() << js["sender"] << js["message"]);
	status.Set(0, Format("count: %d", ++msgcnt), 40);

}

GUI_APP_MAIN
{
	PubSubClient().Run();
}
