#include "PubSubIFaceBase.h"

ZMQPubSubBroker PubSubIFaceBase::broker;
ZMQPubSubClient PubSubIFaceBase::client;

PubSubIFaceBase::PubSubIFaceBase()
{
	// ** WARNING!
	// derived objs should override ReceiveMessage instead of
	// diverting messages to their methods through WhenMessage
	client.WhenMessage   = THISBACK(ReceiveMessage);
}

/*PubSubIFaceBase::~PubSubIFaceBase()
{
	client.stopClient();
	broker.stopBroker();
}
*/















