#ifndef _PubSubIFaceBase_TCPMessage_h_
#define _PubSubIFaceBase_TCPMessage_h_

#include <Core/Core.h>

using namespace Upp;

class TCPMessage {
	public:
	typedef TCPMessage CLASSNAME;
	
	TCPMessage() {}
	TCPMessage(const String& msg);
	String ToString() { return *this; }
	operator String ();
	static const String getTCPMsgStatement(const int id);

	enum TCPMsg_Issuer		{ SERVER = 1010 };
	enum TCPMsg_Statement	{ SERVER_KEEP_ALIVE = 1020, SERVER_SHUTDOWN };
	
	String			sender;			// machine:pid identifier
	int				issuerId;		// app module originating the msg
	int				itemId;			// some general use id
	int				statement;		// message statement
};

#endif
