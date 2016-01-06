#include "PubSubClient.h"

PubSubClient::PubSubClient() : client("NOTEHOPE", "5560")
{
	CtrlLayout(*this, "ZMQ PUB/SUB Client");
	AddFrame(status);
	status.AddFrame(total.Right(120));
	status = String("Server at 'tcp://").Cat()
			 << client.getServerName() << ":" << client.getPort() << "'";
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
	stop.Disable();
	
	// ping-ing server might help in restablishing connectivity to it,
	// seen it while running home network tests
	String resp;
	errboard.Insert(0);
	if(Sys(String("ping ").Cat() << client.getServerName(), resp) == -1)
		resp = "no ping response";
	errboard.Set(0, 0, resp);
	
	// add exception description to board
	errboard.Insert(0);
	errboard.Set(0, 0, exc);
	
	// restart client
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
	total.Set(Format("count: %d", ++msgcnt));
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
