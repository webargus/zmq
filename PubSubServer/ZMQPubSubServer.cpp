#include "ZMQPubSubServer.h"

ZMQPubSubServer::ZMQPubSubServer() : context(1), running(false)
{
}

void ZMQPubSubServer::sendMessage(const String msg)
{
	if(!running) {
		running = true;
		Thread().Run(THISBACK(publisherLoop));
	}

	try {
		zmq::socket_t pusher = zmq::socket_t (context, ZMQ_PUSH);
		pusher.bind("tcp://*:5555");
		zmq::message_t message(msg.Begin(), msg.GetCount());
		pusher.send(message);
	} catch (zmq::error_t ex) {
		PostCallback(THISBACK1(processServerException, Format("%d - %s", errno, ex.what())));
	}
}

void ZMQPubSubServer::publisherLoop()
{
	try {
		zmq::socket_t puller = zmq::socket_t(context, ZMQ_PULL);
		puller.connect("tcp://localhost:5555");
		zmq::socket_t publisher = zmq::socket_t (context, ZMQ_PUB);
		publisher.connect("tcp://NOTEHOPE:5559");
		
		zmq::pollitem_t	items[] = { {puller, 0, ZMQ_POLLIN, 0} };
		zmq::message_t msg;
		while(running) {
			zmq::poll (&items[0], 1, 3000);
			if(items[0].revents & ZMQ_POLLIN) {
				puller.recv(&msg);
				//  Send message to broker
		    	publisher.send(msg);
			}
		}
	}
	catch (zmq::error_t ex)
	{
		running = false;
		PostCallback(THISBACK1(processServerException, Format("%d - %s", errno, ex.what())));
	}
}

void ZMQPubSubServer::processServerException(const String exc)
{
	DUMP(exc);
	PromptOK(exc);
}








