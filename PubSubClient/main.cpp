#include "PubSubClient.h"

PubSubClient::PubSubClient()
{
	CtrlLayout(*this, "ZMQ PUB/SUB Client");
	AddFrame(status);
	history.AddColumn("sender");
	history.AddColumn("message");
	errboard.AddColumn("fault report");
	clear <<= THISBACK(ClearGUI);
	start <<= THISBACK(ClientStart);
	stop  <<= THISBACK(ClientStop);
	stop.Disable();
	client.WhenWarning = THISBACK(ManageClientWarning);
	client.WhenMessage = THISBACK(ManageClientMessage);
	client.WhenException = THISBACK(ManageClientException);
}

void PubSubClient::ClearGUI()
{
	history.Clear();
	errboard.Clear();
}

void PubSubClient::ManageClientWarning(const String msg)
{
	errboard.Insert(0);
	errboard.Set(0, 0, msg);
}

void PubSubClient::ManageClientException(const String exc)
{
	static int cnt = 0;
	cnt++;
	if(cnt == 2) {
		if(PromptAbortRetry(Format("Couldn't recover from fault after %d attempts", cnt))) {
			PostCallback(callback(this, &TopWindow::Close));		// close window!
			return;
		}
		cnt = 0;
	}
	ClientStop();
	stop.Disable();
	errboard.Insert(0);
	errboard.Set(0, 0, exc);
	errboard.Insert(0);
	errboard.Set(0, 0, "restarting client...");
	DUMP(exc);
	ClientStart();
}

void PubSubClient::ManageClientMessage(const String msg)
{
	static int msgcnt = 0;
	if((msgcnt % 500) == 0)
		history.Clear();
	Value js = ParseJSON(msg);
	history.Insert(0, Vector<Value>() << js["sender"] << js["message"]);
	status.Set(0, Format("count: %d", ++msgcnt), 40);
}

void PubSubClient::ClientStart()
{
	start.Disable();
	stop.Enable();
	client.startClient();
}

void PubSubClient::ClientStop()
{
	stop.Disable();
	start.Enable();
	client.stopClient();
}

GUI_APP_MAIN
{
	PubSubClient().Run();
}
