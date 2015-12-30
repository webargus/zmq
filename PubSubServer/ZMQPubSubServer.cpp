#include "ZMQPubSubServer.h"

ZMQPubSubServer::ZMQPubSubServer() : context(1), publisher(context, ZMQ_PUB)
{
	publisher.connect("tcp://NOTEHOPE:5559");
}

void ZMQPubSubServer::sendMessage(const String& msg)
{
	Thread().Run(THISBACK1(publisherLoop, msg));
}

void ZMQPubSubServer::publisherLoop(const String& msg)
{
	/*
		broker just does not work if we create a PUB socket within this thread loop;
		PUB socket had to be declared as class member and initialized in constructor;
		reason why? unknown; we just hope there'll be no thread-related issues,
		since zmq specs insist that sockets are not thread-safe;
		anyway, if shit happens, we hopefully catch and dump it to log;
	*/
	try
	{
	    //  Send message to broker
	    zmq::message_t message(msg.ToStd().c_str(), msg.GetCount());
	    publisher.send(message);
	}
	catch (zmq::error_t ex)
	{
		DUMP(ex.what());
	}
}

