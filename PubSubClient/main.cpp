#include "PubSubClient.h"

PubSubClient::PubSubClient()
{
	CtrlLayout(*this, "ZMQ PUB/SUB Client");
	AddFrame(status);
	history.AddColumn("sender");
	history.AddColumn("message");
	errboard.AddColumn("fault report");
	clear <<= callback(&history, &ArrayCtrl::Clear);
	start <<= THISBACK(ClientStart);
	stop  <<= THISBACK(ClientStop);
	stop.Disable();
}

void PubSubClient::processClientWarning(const String msg)
{
	PostCallback(THISBACK1(manageClientWarning, msg));
}

void PubSubClient::manageClientWarning(const String msg)
{
	errboard.Insert(0);
	errboard.Set(0, 0, msg);
}

void PubSubClient::processClientException(const String exc)
{
	PostCallback(THISBACK1(manageClientException, exc));
}

void PubSubClient::manageClientException(const String exc)
{
	static int cnt = 0;
	cnt++;
	if(cnt == 2) {
		if(PromptAbortRetry(Format("Couldn't recover from fault after %d attempts", cnt)))
			Close();		// close window!
		cnt = 0;
	}
	stop.Disable();
	errboard.Insert(0);
	errboard.Set(0, 0, exc);
	errboard.Insert(0);
	errboard.Set(0, 0, "restarting client...");
	DUMP(exc);
	ClientStart();
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

void PubSubClient::processClientMessage(const String msg)
{
	PostCallback(THISBACK1(addMessageToHistory, msg));
}

void PubSubClient::addMessageToHistory(const String msg)
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
