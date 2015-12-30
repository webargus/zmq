#include "ZMQPubSubClient.h"

ZMQPubSubClient::ZMQPubSubClient() : context(1), running(false)
{
}

void ZMQPubSubClient::startClient()
{
	if(running)
		return;
	running = true;
	Thread().Run(THISBACK(clientLoop));
}

void ZMQPubSubClient::stopClient()
{
	running = false;
}

void ZMQPubSubClient::clientLoop()
{
	try
	{
	    zmq::socket_t subscriber (context, ZMQ_SUB);
	    subscriber.connect("tcp://NOTEHOPE:5560");
	
	    //  zero length filter string subscribes to all publishers
	    const char *filter = "";
	    subscriber.setsockopt(ZMQ_SUBSCRIBE, filter, strlen (filter));
		
		zmq::message_t msg;
		zmq::pollitem_t items[] = {{subscriber, 0, ZMQ_POLLIN, 0}};
		while (running) {
			// keep on looping until some msg comes in or app control breaks out from thread
			zmq::poll (&items [0],
					   1,			// no. of zmq::pollitem_t structures in items array
					   0);			// timeout in millisecs; 0 => return immediately
			if(items[0].revents & ZMQ_POLLIN) {
				// we got some incoming msg, so fetch and process it
				subscriber.recv(&msg, 0);
				processClientMessage(String(static_cast<char*>(msg.data()), msg.size()));
			}
		}
	}
	catch(zmq::error_t ex)
	{
		DUMP(ex.what());
	}
}








