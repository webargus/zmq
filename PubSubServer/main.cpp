#include "PubSubServer.h"

PubSubServer::PubSubServer() : inautomode(false), server("NOTEHOPE", "5559")
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
	server.WhenException = THISBACK(processServerException);
}

void PubSubServer::processServerException(const String exc)
{
	// if in automode, stop it
	if(inautomode)
		toggleAutoMode();
	// although exc took place in server thread, that's ok, no need to post callback,
	// server loop already did it, we're running in main thread now.
	if(PromptAbortRetry(exc)) {
		// added callback here to correct for panic msgs on corrupted memory
		// issued when aborting process.
		PostCallback(callback(this, &TopWindow::Close));		// close window!
		return;
	}
	// try to send msg again
	BroadcastMessage();
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
	server.sendMessage(json.ToString());
}

GUI_APP_MAIN
{
	PubSubServer().Run();
}
