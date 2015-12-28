#include "ZMQQueueBroker.h"

ZMQQueueBroker::ZMQQueueBroker() : context(1), running(false)
{
}

void ZMQQueueBroker::startBroker()
{
	running = true;
	Thread().Run(THISBACK(brokerLoop));
}

void ZMQQueueBroker::brokerLoop()
{
	try
	{
	    //  Prepare our sockets
	    zmq::socket_t frontend (context, ZMQ_ROUTER);
	    zmq::socket_t backend (context, ZMQ_DEALER);
	
	    frontend.bind("tcp://*:5559");
	    backend.bind("tcp://*:5560");
	
	    //  Initialize poll set
	    zmq::pollitem_t items [] = {
	        { frontend, 0, ZMQ_POLLIN, 0 },
	        { backend,  0, ZMQ_POLLIN, 0 }
	    };
	    
	    //  Switch messages between sockets
	    while (running) {
	        zmq::message_t message;
	        int more;               //  Multipart detection
	
	        zmq::poll (&items [0], 2, 0);
	        
	        if (items [0].revents & ZMQ_POLLIN) {
	            while (running) {
	                //  Process all parts of the message
	                frontend.recv(&message);
	                size_t more_size = sizeof (more);
	                frontend.getsockopt(ZMQ_RCVMORE, &more, &more_size);
	                backend.send(message, more? ZMQ_SNDMORE: 0);
	                
	                if (!more)
	                    break;      //  Last message part
	            }
	        }
	        if (items [1].revents & ZMQ_POLLIN) {
	            while (running) {
	                //  Process all parts of the message
	                backend.recv(&message);
	                size_t more_size = sizeof (more);
	                backend.getsockopt(ZMQ_RCVMORE, &more, &more_size);
	                frontend.send(message, more? ZMQ_SNDMORE: 0);
	                if (!more)
	                    break;      //  Last message part
	            }
	        }
	    }
	}
	catch (zmq::error_t ex)
	{
		DUMP(ex.what());
	}
}










