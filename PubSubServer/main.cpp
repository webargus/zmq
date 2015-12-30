#include "PubSubServer.h"

const String PubSubServer::sender = GetComputerName().Cat() << Format(":%d", getpid());

PubSubServer::PubSubServer()
{
	CtrlLayout(*this, "ZMQ PUB/SUB Broadcast Server");
	history.AddColumn("messages");
	id.SetText("Broadcaster id: " + sender);
	clear <<= callback(&history, &ArrayCtrl::Clear);
	msg.WhenEnter = THISBACK(BroadcastMessage);
}

void PubSubServer::BroadcastMessage()
{
	history.Insert(0, Vector<Value>() << ~msg);
	Json json;
	json("sender", sender)("message", (~msg).ToString());
	sendMessage(json.ToString());
}

GUI_APP_MAIN
{
	PubSubServer().Run();
}
