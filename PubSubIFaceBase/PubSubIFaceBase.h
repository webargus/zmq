#ifndef _PubSubIFaceBase_PubSubIFaceBase_h_
#define _PubSubIFaceBase_PubSubIFaceBase_h_

#include <Core/Core.h>
#include <PubSubBroker/ZMQPubSubBroker.h>
#include <PubSubClient/ZMQPubSubClient.h>
#include "TCPMessage.h"

class PubSubIFaceBase {
	public:
	typedef PubSubIFaceBase CLASSNAME;
	
	PubSubIFaceBase();
	~PubSubIFaceBase();
	static void Broadcast(TCPMessage& msg);
	virtual void ReceiveMessage(const String msg) { }
	
	static ZMQPubSubBroker		broker;
	static ZMQPubSubClient		client;
};

#endif



















