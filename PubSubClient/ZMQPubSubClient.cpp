#include "ZMQPubSubClient.h"

ZMQPubSubClient::ZMQPubSubClient() : context(1), running(false)
{
}

void ZMQPubSubClient::startClient()
{
	if(running)
		return;
	running = true;
	worker.Run(THISBACK(clientLoop));
}

void ZMQPubSubClient::stopClient()
{
	if(!running)
		return;
	running = false;
	worker.Wait();
	//PromptOK("Thread joined");
}

void ZMQPubSubClient::clientLoop()
{
	zmq::socket_t * subscriber;
	try
	{
    	subscriber = new zmq::socket_t(context, ZMQ_SUB);
    	subscriber->connect("tcp://NOTEHOPE:5560");

	    //  zero length filter string subscribes to all publishers
	    const char *filter = "";
	    subscriber->setsockopt(ZMQ_SUBSCRIBE, filter, strlen (filter));
	    //  Configure socket to not wait at close time
	    /*int linger = 0;
	    subscriber->setsockopt(ZMQ_LINGER, &linger, sizeof (linger));*/
	}
	catch(zmq::error_t ex)
	{
		running = false;
		subscriber->close();
		delete subscriber;
		PostCallback(THISBACK1(processClientException,
							   Format("subscriber connect; %d - %s", errno, ex.what())));
		return;
	}

	int retries = REQUEST_RETRIES;
	zmq::message_t msg;
	zmq::pollitem_t items[] = {{*subscriber, 0, ZMQ_POLLIN, 0}};
	bool expecting_msg = true;
	while (running) {
		try
		{
			// keep on looping until some msg comes in or app control interrupts thread
			zmq::poll (&items [0],
					   1,					// no. of zmq::pollitem_t structures in items array
					   REQUEST_TIMEOUT);	// timeout in millisecs; 0 => return immediately
			if(!running)
				break;
			if(items[0].revents & ZMQ_POLLIN) {
				// we got some incoming msg, so fetch and process it
				subscriber->recv(&msg, 0);
				PostCallback(THISBACK1(processClientMessage,
							 String(static_cast<char*>(msg.data()), msg.size())));
				retries = REQUEST_RETRIES;
				if(expecting_msg) {
					PostCallback(THISBACK1(processClientWarning, "client connected"));
					expecting_msg = false;
				}
				continue;
			}
			
			expecting_msg = true;
			
			if(--retries == 0)
				throw(String("server unreacheable, aborted"));
			
			PostCallback(THISBACK1(processClientWarning,
								  Format("got no msg from server since %d secs ago",
										 int(REQUEST_TIMEOUT/1000))));
			PostCallback(THISBACK1(processClientWarning,
								  Format("%d. listening to server for another %d secs...",
										 REQUEST_RETRIES-retries, int(REQUEST_TIMEOUT/1000))));
		}
		catch(zmq::error_t ex)
		{
			PostCallback(THISBACK1(processClientException, Format("%d - %s", errno, ex.what())));
			running = false;
		}
		catch(String& ex)
		{
			PostCallback(THISBACK1(processClientException, ex));
			running = false;
		}
	}
	subscriber->close();
	delete subscriber;
}

void ZMQPubSubClient::processClientMessage(const String msg)
{
	WhenMessage(msg);
}

void ZMQPubSubClient::processClientException(const String exc)
{
	WhenException(exc);
}

void ZMQPubSubClient::processClientWarning(const String msg)
{
	WhenWarning(msg);
}








