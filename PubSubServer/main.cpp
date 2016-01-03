#include "PubSubServer.h"

PubSubServer::PubSubServer() : inautomode(false)
{
	CtrlLayout(*this, "ZMQ PUB/SUB Broadcast Server");
	sender = GetComputerName().Cat() << Format(":%d", getpid());
	AddFrame(status);
	cnt.Set("count: 0");
	status.AddFrame(cnt.Right(80));
	srvid.Set("id: "+sender);
	status.AddFrame(srvid.Right(160));
	history.AddColumn("messages");
	clear <<= callback(&history, &ArrayCtrl::Clear);
	msg.WhenEnter = THISBACK(BroadcastMessage);
	automode <<= THISBACK(toggleAutoMode);
}

void PubSubServer::processServerException(const String exc)
{
	// if exc in server loop, that's ok, no need to post callback,
	// server loop already does it.
	if(PromptAbortRetry(exc))
		Close();		// close window!
}

void PubSubServer::toggleAutoMode()
{
	inautomode = inautomode ? false : true;
	status = "";
	msg.Enable(!inautomode);
	automode.SetLabel(inautomode ? "stop automode" : "start automode");
	if(!inautomode) {
		KillTimeCallback();
		return;
	}
	SetTimeCallback(0, THISBACK(runAutoMode));
}

void PubSubServer::runAutoMode()
{
	// generate a random integer for a message
	int min = 10000;
	int max = 99999;
	msg = Format("auto-generated msg %d", int(Random(max - min) + min));
	// generate a random delay time (ms) between msgs
	min = 500;			// half a sec
	max = 2000;			// 2 secs
	int delay = int(Random(max - min) + min);
	status = Format("waiting %d ms", delay);
	BroadcastMessage();
	SetTimeCallback(delay, THISBACK(runAutoMode));
}

void PubSubServer::BroadcastMessage()
{
	static int msgcnt = 0;
	cnt.Set(Format("count: %d", ++msgcnt));
	// clear history if msg count too big
	if((msgcnt % 500) == 0)
		history.Clear();
	history.Insert(0, Vector<Value>() << ~msg);
	Json json;
	json("sender", sender)("message", (~msg).ToString());
	sendMessage(json.ToString());
}

GUI_APP_MAIN
{
	PubSubServer().Run();
}
