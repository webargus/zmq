#include "lazypiretclient_GUI.h"

lazypiretclient_GUI::lazypiretclient_GUI() : sender("sender"), message("message")
{
	CtrlLayout(*this, "zeroMQ Lazy Pirate Client");
	send <<= THISBACK(sendClientMessage);
	msgs.AddColumn(sender, "sender");
	msgs.AddColumn(message, "message");
	report.AddColumn("");
	clientId = GetComputerName();
	clientId.Cat() << ":" << getpid();
	lb1.SetText("client Id: " + clientId);
	clear <<= THISBACK(clearReports);
}

void lazypiretclient_GUI::clearReports()
{
	msgs.Clear();
	report.Clear();
}

void lazypiretclient_GUI::sendClientMessage()
{
	Json json;
	json("sender", clientId)
		("message", (~inpMsg).ToString());
	//for(int i = 0; i < 10; i++)
	Thread().Run(THISBACK1(sendMessage, json.ToString()));
	//sendMessage(json.ToString().ToStd());
}

void lazypiretclient_GUI::processReportMessage(const String& rep)
{
	PostCallback(THISBACK1(outReportMsg, rep));
}

void lazypiretclient_GUI::processClientMessage(const String& msg)
{
	PostCallback(THISBACK1(outClientMsg, msg));
}

void lazypiretclient_GUI::outReportMsg(const String& msg) { report.Add(msg); }

void lazypiretclient_GUI::outClientMsg(const String& msg)
{
	Value js = ParseJSON(msg);
	msgs.AddMap(js);
}

GUI_APP_MAIN
{
	lazypiretclient_GUI().Run();
}
