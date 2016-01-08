#include "ZMQPubSubServer.h"

ZMQPubSubServer::ZMQPubSubServer()
	: server("localhost"), srvport("5559"), lclport("5555"), context(1), running(false)
{
}

ZMQPubSubServer::ZMQPubSubServer(const String& srv, const String& sp, const String& lp)
	: server(srv), srvport(sp), lclport(lp), context(1), running(false)
{
}

void ZMQPubSubServer::stopServer()
{
	if(!running)
		return;
	running = false;
	worker.Wait();
}

void ZMQPubSubServer::setServerParams(const String& srv, const String& sp)
{
	server 	= srv;
	srvport = sp;
	
	if(running) {
		stopServer();
		running = true;
		worker.Run(THISBACK(publisherLoop));
	}
}

void ZMQPubSubServer::sendMessage(const String msg)
{
	if(!running) {
		running = true;
		worker.Run(THISBACK(publisherLoop));
	}

	try {
		zmq::socket_t pusher = zmq::socket_t (context, ZMQ_PUSH);
		pusher.bind(String("tcp://*:").Cat() << lclport);
		zmq::message_t message(msg.Begin(), msg.GetCount());
		pusher.send(message);
	} catch (zmq::error_t ex) {
		processServerException(Format("%d - %s", errno, ex.what()));
	}
}

void ZMQPubSubServer::publisherLoop()
{
	try {
		zmq::socket_t puller = zmq::socket_t(context, ZMQ_PULL);
		puller.connect(String("tcp://localhost:").Cat() << lclport);
		zmq::socket_t publisher = zmq::socket_t (context, ZMQ_PUB);
		publisher.connect(String("tcp://").Cat() << server << ":" << srvport);
		
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
	WhenException(exc);
}








