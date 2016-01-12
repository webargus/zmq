#ifndef _PubSubIFaceBase_TCPMessage_h_
#define _PubSubIFaceBase_TCPMessage_h_

#include <Core/Core.h>

using namespace Upp;

class TCPMessage {
	public:
	typedef TCPMessage CLASSNAME;
	
	TCPMessage();
	TCPMessage(const String msg);
	String ToString() { return *this; }
	operator String ();
	virtual String getTCPMsgStatement(const int id) const;

	enum TCPMsg_Issuer		{ SERVER = 1010, CLIENT, BROKER };
	enum TCPMsg_Statement	{ SERVER_KEEP_ALIVE = 1020, SERVER_SHUTDOWN,
							  CLIENT_CONNECT_OK, CLIENT_RECV_RETRY, CLIENT_ABORT,
							  CLIENT_CONNECT_ERROR, CLIENT_ERROR };
	
	String			sender;			// machine:pid identifier
	int				issuerId;		// app module originating the msg
	int				itemId;			// some general use id
	int				statement;		// message statement
	String			content;		// general content
};

#endif
