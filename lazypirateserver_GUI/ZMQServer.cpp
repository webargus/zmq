#include "ZMQServer.h"

ZMQServer::ZMQServer() : context(1), running(false)
{
}

void ZMQServer::serverStart()
{
	if(running)
		return;
	running = true;
	Thread().Run(THISBACK(serverLoop));
}

void ZMQServer::serverStop()
{
	running = false;
}

void ZMQServer::serverLoop()
{
	try
	{
		/*
		* sockets must be defined within the thread loop they run,
		* for sockets are not thread-safe.
		* ZMQ will automatically close them (and all message objs, too)
		* when calling their destructors, before
		* destroying class-scope, socket-wrapping zmq::context_t object, in
		* the correct order. This should avoid memory leaks and program crashes.
		* See http://zeromq.org/whitepapers:0mq-termination
		* for details.
		*/
		zmq::socket_t server(context, ZMQ_REP);
	    //  Configure socket to not wait at close time
	    /*int linger = 0;
	    server.setsockopt (ZMQ_LINGER, &linger, sizeof (linger));*/
	   	server.connect("tcp://NOTEHOPE:5560");
		zmq::message_t in_msg;
		zmq::pollitem_t items[] = {{server, 0, ZMQ_POLLIN, 0}};
		while(running) {
			
			// keep on looping until some msg comes in or app control breaks out from thread
			zmq::poll (&items [0],
					   1,			// no. of zmq::pollitem_t structures in items array
					   0);			// timeout in millisecs; 0 => return immediately
			if(!(items[0].revents & ZMQ_POLLIN))
				continue;
			
			// we got some incoming msg, so fetch it and process it
			server.recv(&in_msg);
			// copy msg to Upp String for convenience
			String req(static_cast<char*>(in_msg.data()), in_msg.size());
			// call overloaded method in derived class to process msg
			req = processMessage(req);
			// send processed response back to client
			zmq::message_t out_msg(req.ToStd().c_str(), req.GetCount());
			if(!server.send(out_msg)) {
				LOG("failed enqueueing outbound msg in server socket");
				DUMP(zmq_strerror(errno));
			}
		}
	}
	catch (zmq::error_t ex)
	{
		// Can't DUMP here in a real app because of threading
		DUMP(ex.what());
	}
}



