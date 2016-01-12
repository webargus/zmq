#include "ZMQPubSubClient.h"

ZMQPubSubClient::ZMQPubSubClient()
	: server("localhost"), port("5560"), context(1), running(false)
{
}

ZMQPubSubClient::ZMQPubSubClient(const String& srv, const String& p)
	: server(srv), port(p), context(1), running(false)
{
}

void ZMQPubSubClient::setServerName(const String& srv)
{
	server = srv;
	if(running) {
		stopClient();
		startClient();
	}
}

void ZMQPubSubClient::setPort(const String& p)
{
	port = p;
	if(running) {
		stopClient();
		startClient();
	}
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
	TCPMessage tcp_msg;
	tcp_msg.issuerId = TCPMessage::CLIENT;
	zmq::socket_t * subscriber;
	try
	{
    	subscriber = new zmq::socket_t(context, ZMQ_SUB);
    	
		/* MANUAL's ii:
		Caution: All options, with the exception of 
		ZMQ_SUBSCRIBE, ZMQ_UNSUBSCRIBE, ZMQ_LINGER, ZMQ_ROUTER_MANDATORY and
		ZMQ_XPUB_VERBOSE only take effect for subsequent socket bind/connects.*/
		
	    int rcvbuf = 16384;		// 16K
	    subscriber->setsockopt(ZMQ_RCVBUF, &rcvbuf, sizeof rcvbuf);
 
	   	subscriber->connect((String("tcp://").Cat() << server << ":" << port).Begin());

	    //  zero length filter string subscribes to all publishers
	    const char *filter = "";
	    subscriber->setsockopt(ZMQ_SUBSCRIBE, filter, strlen (filter));
	    
	}
	catch(zmq::error_t ex)
	{
		running = false;
		subscriber->close();
		delete subscriber;
		tcp_msg.statement = TCPMessage::CLIENT_CONNECT_ERROR;
		tcp_msg.content = Format("%d - %s", errno, ex.what());
		PostCallback(THISBACK1(processClientException, tcp_msg));
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
			
			// check if running again in case control app stopped client while polling
			if(!running)
				break;
			
			if(items[0].revents & ZMQ_POLLIN) {
				// we got some incoming msg, so fetch and process it
				subscriber->recv(&msg, 0);
				PostCallback(THISBACK1(processClientMessage,
							 String(static_cast<char*>(msg.data()), msg.size())));
				retries = REQUEST_RETRIES;
				// warn control app we're in business, 
				//in case we reconnected or just connected
				if(expecting_msg) {
					tcp_msg.statement = TCPMessage::CLIENT_CONNECT_OK;
					tcp_msg.content = "client connected";
					PostCallback(THISBACK1(processClientWarning, tcp_msg));
					expecting_msg = false;
				}
				continue;
			}
			
			// no msg polled, so let's try again
			expecting_msg = true;
			
			// max retries reached, abort
			if(--retries == 0)
				throw(String("servidor não responde, abortando procedimento."));
			
			// warn control app we're about to try polling msgs again
			tcp_msg.statement = TCPMessage::CLIENT_RECV_RETRY;
			tcp_msg.itemId = REQUEST_TIMEOUT; // pass timeout in msg itemId
			tcp_msg.content = "polling again";
			PostCallback(THISBACK1(processClientWarning, tcp_msg));
		}
		catch(zmq::error_t ex)
		{
			running = false;
			subscriber->close();
			delete subscriber;
			tcp_msg.statement = TCPMessage::CLIENT_ERROR;
			tcp_msg.content = Format("%d - %s", errno, ex.what());
			PostCallback(THISBACK1(processClientException, tcp_msg));
			return;
		}
		catch(String& ex)
		{
			subscriber->close();
			delete subscriber;
			running = false;
			tcp_msg.statement = TCPMessage::CLIENT_ABORT;
			tcp_msg.content = ex;
			PostCallback(THISBACK1(processClientException, tcp_msg));
			return;
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








