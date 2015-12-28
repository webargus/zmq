#ifndef _lazypirateserver_GUI_lazypirateserver_GUI_h
#define _lazypirateserver_GUI_lazypirateserver_GUI_h

#include <CtrlLib/CtrlLib.h>
#include "ZMQServer.h"

using namespace Upp;

#define LAYOUTFILE <lazypirateserver_GUI/lazypirateserver_GUI.lay>
#include <CtrlCore/lay.h>

class lazypirateserver_GUI : public ZMQServer, public Withlazypirateserver_GUILayout<TopWindow> {
public:
	typedef lazypirateserver_GUI CLASSNAME;
	lazypirateserver_GUI();
	
private:
	virtual String processMessage(String msg);
	void addReport(const ValueMap& msg);
	void StartServer();
	void StopServer();
	
	Id			sender, message;
};

#endif
