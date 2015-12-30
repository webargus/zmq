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

void PubSubServer::toggleAutoMode()
{
	inautomode = inautomode ? false : true;
	status = "";
	msg.Enable(!inautomode);
	automode.SetLabel(inautomode ? "stop automode" : "start automode");
	if(!inautomode)
		return;
	Thread().Run(THISBACK(runAutoMode));
}

void PubSubServer::runAutoMode()
{
	while(inautomode) {
		// generate a random integer for a message
		int min = 10000;
		int max = 99999;
		autoModeMsg automsg;
		automsg.msg = Format("auto-generated msg %d", int(Random(max - min) + min));
		// generate a random delay time (ms) between msgs
		min = 500;			// half a sec
		max = 2000;			// 2 secs
		automsg.delay = int(Random(max - min) + min);
		// thread-safe send msg
		PostCallback(THISBACK1(AutoBroadcastMessage, automsg));
		// wait delay between msgs
		while(inautomode && (automsg.delay > 0)) {
			automsg.delay--;
			Sleep(1);
		}
	}
}

void PubSubServer::AutoBroadcastMessage(const autoModeMsg automsg)
{
	msg = automsg.msg;
	status = Format("waiting %d ms", automsg.delay);
	BroadcastMessage();
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
