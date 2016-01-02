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
		
		int retries = REQUEST_RETRIES;
		zmq::message_t msg;
		zmq::pollitem_t items[] = {{subscriber, 0, ZMQ_POLLIN, 0}};
		bool expecting_msg = true;
		while (running) {
			// keep on looping until some msg comes in or app control interrupts thread
			zmq::poll (&items [0],
					   1,					// no. of zmq::pollitem_t structures in items array
					   REQUEST_TIMEOUT);	// timeout in millisecs; 0 => return immediately
			if(items[0].revents & ZMQ_POLLIN) {
				// we got some incoming msg, so fetch and process it
				subscriber.recv(&msg, 0);
				processClientMessage(String(static_cast<char*>(msg.data()), msg.size()));
				retries = REQUEST_RETRIES;
				if(expecting_msg) {
					processClientWarning("client connected");
					expecting_msg = false;
				}
				continue;
			}
			expecting_msg = true;
			if(--retries == 0)
				throw(std::exception("server unreacheable, aborted"));
			processClientWarning(Format("got no msg from server since %d secs ago",
										int(REQUEST_TIMEOUT/1000)));
			processClientWarning(Format("%d. listening to server for another %d secs...",
										REQUEST_RETRIES-retries, int(REQUEST_TIMEOUT/1000)));
		}
	}
	catch(zmq::error_t ex)
	{
		running = false;
		processClientException(Format("%d - %s", errno, ex.what()));
	}
	catch(std::exception ex)
	{
		running = false;
		processClientException(ex.what());
	}
}








