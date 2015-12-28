#include "ZMQClient.h"

zmq::socket_t * ZMQClient::s_client_socket()
{
    zmq::socket_t * client = new zmq::socket_t (context, ZMQ_REQ);
    client->connect ("tcp://NOTEHOPE:5559");

    //  Configure socket to not wait at close time
    int linger = 0;
    client->setsockopt (ZMQ_LINGER, &linger, sizeof (linger));
    return client;
}

ZMQClient::ZMQClient() : context(1) {}

void ZMQClient::sendMessage(String msg)
{
	static int threads = 0;
	++threads;
	String thread_id = Format("Thread #%d", threads);
    int retries_left = REQUEST_RETRIES;
	
	try
	{
	    zmq::socket_t * client = s_client_socket ();
	    while (retries_left) {
			zmq::message_t out_msg(msg.ToStd().c_str(), msg.GetCount());
	        client->send(out_msg);
	        Sleep (1);
	
	        bool expect_reply = true;
	        while (expect_reply) {
	            //  Poll socket for a reply, with timeout
	            zmq::pollitem_t items[] = { { *client, 0, ZMQ_POLLIN, 0 } };
	            zmq::poll (&items[0], 1, REQUEST_TIMEOUT);
	
	            //  If we got a reply, process it
	            if (items[0].revents & ZMQ_POLLIN) {
	                zmq::message_t in_msg;
	                client->recv(&in_msg);
	                String reply(static_cast<char*>(in_msg.data()), in_msg.size());
	                String report = thread_id;
	                report.Cat() << ": server replied OK";
	                processReportMessage(report);
	                processClientMessage(reply);
	                retries_left = 0;
	                break;
	            }
	            else	// no more retries left, so give it up
	            if (--retries_left == 0) {
	                String report = thread_id;
	                report.Cat() << " E: server seems to be offline, abandoning";
	                processReportMessage(report);
	                expect_reply = false;
	                break;
	            }
	            else {	// try again
	                String report = thread_id;
	                report.Cat() << " W: no response from server, retrying…";
	                processReportMessage(report);
	                //  Old socket will be confused; close it and open a new one
	                delete client;
	                client = s_client_socket ();
	                //  Send request again, on new socket;
	                //  need to redefine outgoing msg,
	                //  for previous send blanks it out
					zmq::message_t out_msg(msg.ToStd().c_str(), msg.GetCount());
	                client->send(out_msg);
	            }
	        }
	    }
	    threads--;
	    delete client;
	}
	catch (zmq::error_t ex)
	{
		DUMP(ex.what());
	}
}











