#ifndef _PubSubIFaceBase_PubSubIFaceBase_h_
#define _PubSubIFaceBase_PubSubIFaceBase_h_

#include <Core/Core.h>
#include <PubSubBroker/ZMQPubSubBroker.h>
#include <PubSubClient/ZMQPubSubClient.h>
#include <PubSubServer/ZMQPubSubServer.h>

using namespace Upp;

struct TCPMessage {
	TCPMessage() {}
	TCPMessage(const String& msg);
	String ToString() { return *this; }
	operator String ();
	
	String			sender;			// machine:pid identifier
	int				issuerId;		// app module originating the msg
	int				itemId;			// some general use id
	int				statement;		// message statement
};

class PubSubIFaceBase {
	public:
	typedef PubSubIFaceBase CLASSNAME;
	
	protected:
	PubSubIFaceBase();
	static void Broadcast(TCPMessage& msg);
	virtual void ReceiveMessage(const String msg) { }
	
	static ZMQPubSubBroker		broker;
	static ZMQPubSubServer		server;
	static ZMQPubSubClient		client;

	public:
	static const String getTCPMsgStatement(const int id);
	enum TCPMsg_Issuer			{ SERVER = 1000 };
	enum TCPMsg_Statement		{ SERVER_KEEP_ALIVE = 1010, SERVER_SHUTDOWN };
};

#endif



















