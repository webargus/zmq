#include "ZMQPubSubServer.h"

ZMQPubSubServer::ZMQPubSubServer() : context(1), publisher(context, ZMQ_PUB)
{
	
}

void ZMQPubSubServer::sendMessage(const String& msg)
{
	static bool socket_init = false;
	if(!socket_init) {
		try {
			publisher.connect("tcp://NOTEHOPE:5559");
		} catch (zmq::error_t ex) {
			processServerException(Format("%d - %s", errno, ex.what()));
			return;
		}
		socket_init = true;
	}
	Thread().Run(THISBACK1(publisherLoop, msg));
}

void ZMQPubSubServer::publisherLoop(const String msg)
{
	/*
		broker simply does not work if we create a PUB socket within this thread loop;
		reason why? unknown; I've tried everything, from checking the zmq_connect()
		return value, through creating and deleting a new socket for each loop run,
		to Sleep()ing between socket creation and sending msgs. Nothing worked;
		zmq_connect() has always returned zero, but it does not mean an effective
		connection has been established; it's somewhat useless, though :(
		a true value for send(message) only means the message has been
		delivered successfully to the zmq message queue, but it does not mean
		that the message has been necessarily delivered to its destination;
		we just hope there'll be no thread-related issues,
		since zmq specs insist that sockets are not thread-safe;
		anyway, if shit happens, we hopefully catch and dump it to log;
		Current solution: PUB socket had to be declared as class member and
		initialized later, when user sends his first message (see this->sendMessage),
		otherwise a sys crash occurs.
	*/
	try
	{
	    //  Send message to broker
	    zmq::message_t message(msg.Begin(), msg.GetCount());
	    publisher.send(message);
	}
	catch (zmq::error_t ex)
	{
		PostCallback(THISBACK1(processServerException, Format("%d - %s", errno, ex.what())));
	}
}

void ZMQPubSubServer::processServerException(const String exc)
{
	DUMP(exc);
}








