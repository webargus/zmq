#ifndef _lazypiretclient_GUI_lazypiretclient_GUI_h
#define _lazypiretclient_GUI_lazypiretclient_GUI_h

#include <CtrlLib/CtrlLib.h>
#include "ZMQClient.h"

using namespace Upp;

#define LAYOUTFILE <lazypiretclient_GUI/lazypiretclient_GUI.lay>
#include <CtrlCore/lay.h>

class lazypiretclient_GUI : public ZMQClient, public Withlazypiretclient_GUILayout<TopWindow> {
public:
	typedef lazypiretclient_GUI CLASSNAME;
	lazypiretclient_GUI();
	void sendClientMessage();

	private:
	virtual void processReportMessage(const String& rep);
	virtual void processClientMessage(const String& msg);
	void outReportMsg(const String& msg);
	void outClientMsg(const String& msg);
	void clearReports();
	
	String					clientId;
	Id						sender, message;
};

#endif
