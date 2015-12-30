#include "PubSubClient.h"

PubSubClient::PubSubClient()
{
	CtrlLayout(*this, "ZMQ PUB/SUB Client");
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
	Value js = ParseJSON(msg);
	history.Insert(0, Vector<Value>() << js["sender"] << js["message"]);
}

GUI_APP_MAIN
{
	PubSubClient().Run();
}
