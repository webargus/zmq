#include "TCPMessage.h"

TCPMessage::TCPMessage(const String& msg)
{
	Value js 	= ParseJSON(msg);
	sender 	 	= js["sender"];
	issuerId 	= js["issuerId"];
	itemId	 	= js["itemId"];
	statement	= js["statement"];
}

TCPMessage::operator String ()
{
	Json json;
	json("sender", sender)
		("issuerId", issuerId)
		("itemId", itemId)
		("statement", statement);
	return json.ToString();
}

const String TCPMessage::getTCPMsgStatement(const int id)
{
	String str;
	switch(id) {
		case SERVER_KEEP_ALIVE:
			str = "sinal de conexão 'keep-alive' do servidor";
			break;
		case SERVER_SHUTDOWN:
			str = "servidor em processo de desconexão";
		default:
			str = "mensagem não compreendida";
	}
	return Format("código #%d: %s", id, str);
}


