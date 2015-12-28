#include "lazypirateserver_GUI.h"

lazypirateserver_GUI::lazypirateserver_GUI() : sender("sender"), message("message")
{
	CtrlLayout(*this, "zeroMQ Lazy Pirate Server");
	report.AddColumn(sender, "sender");
	report.AddColumn(message, "message");
	stop.Enable(false);
	start <<= THISBACK(StartServer);
	stop  <<= THISBACK(StopServer);
	clear <<= callback(&report, &ArrayCtrl::Clear);
}

void lazypirateserver_GUI::StartServer()
{
	stop.Enable(true);
	start.Enable(false);
	serverStart();
}

void lazypirateserver_GUI::StopServer()
{
	start.Enable(true);
	stop.Enable(false);
	serverStop();
}

String lazypirateserver_GUI::processMessage(String msg)
{
	String rep, reply;
	Value js = ParseJSON(msg);
	if(js.IsError()) {
		reply = "msg was truncated";
		PostCallback(THISBACK1(addReport, js));
	} else {
		reply = "msg acknowledged";
		PostCallback(THISBACK1(addReport, js));
	}
	
	Json json;
	json("sender", Format("server (%s:%d)", GetComputerName(), getpid()))
		("message", reply);
	return json.ToString();
}

void lazypirateserver_GUI::addReport(const ValueMap& msg)
{
	report.AddMap(msg);
}

GUI_APP_MAIN
{
	lazypirateserver_GUI().Run();
}
