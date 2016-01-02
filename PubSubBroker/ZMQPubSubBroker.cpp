#include "ZMQPubSubBroker.h"

ZMQPubSubBroker::ZMQPubSubBroker() : context(1), running(false)
{
}

void ZMQPubSubBroker::startBroker()
{
	if(running)
		return;
	running = true;
	// LESSON learned!
	// Can't have a console app using Thread. App crashes with memory leaks upon
	// initialization. Don't know why for now...
	Thread().Run(THISBACK(brokerLoop));
}

void ZMQPubSubBroker::stopBroker()
{
	running = false;
}

void ZMQPubSubBroker::brokerLoop()
{
	try
	{
		// backend socket faces publishers (PUB), i.e., XSUB subscribes to PUBs
		zmq::socket_t backend (context, ZMQ_XSUB);
		// frontend socket faces subscribers (SUB), i.e., XPUB publishes to SUBs
		zmq::socket_t frontend (context, ZMQ_XPUB);

		backend.bind("tcp://*:5559");
		frontend.bind("tcp://*:5560");

	    //  Initialize poll set
	    zmq::pollitem_t items [] = {
	        { backend, 0, ZMQ_POLLIN, 0 },
	        { frontend, 0, ZMQ_POLLIN, 0 }
	    };
	    
	    while(running) {
	        zmq::message_t message;
	        
	        // poll msgs
	        zmq::poll (&items[0], 2, 0);
	        
	        // receive msgs from publishers and forward them to subscribers
	        if (items [0].revents & ZMQ_POLLIN) {   
	            backend.recv(&message);
	            processTransitMessage("backend#"+String((char*)message.data(), message.size()));
	            frontend.send(message);
	        }
	        
	        // receive messages from subscribers and forward them to publishers
	        if (items [1].revents & ZMQ_POLLIN) {	        
		        frontend.recv(&message);
	            processTransitMessage("frontend#"+String((char*)message.data(), message.size()));
		        backend.send(message);
	        }
	    }
	}
	catch (zmq::error_t ex)
	{
		processBrokerException(ex.what());
	}
}















