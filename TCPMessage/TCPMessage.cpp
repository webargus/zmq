#include "TCPMessage.h"

TCPMessage::TCPMessage()
{
	sender = GetComputerName().Cat() << ":" << getpid();
}

TCPMessage::TCPMessage(const String msg)
{
	Value js 	= ParseJSON(msg);
	sender 	 	= js["sender"];
	issuerId 	= js["issuerId"];
	itemId	 	= js["itemId"];
	statement	= js["statement"];
	content		= js["content"];
}

TCPMessage::operator String ()
{
	Json json;
	json("sender", sender)
		("issuerId", issuerId)
		("itemId", itemId)
		("statement", statement)
		("content", content);
	return json.ToString();
}

String TCPMessage::getTCPMsgStatement(const int id) const
{
	String str;
	switch(id) {
		case SERVER_KEEP_ALIVE:
			str = "sinal de conexão 'keep-alive' do servidor";
			break;
		case SERVER_SHUTDOWN:
			str = "servidor em processo de desconexão";
			break;
		case CLIENT_CONNECT_OK:
			str = "cliente conectado";
			break;
		case CLIENT_RECV_RETRY:
			str = "cliente esperando mensagem do servidor novamente";
			break;
		case CLIENT_ABORT:
			str = "cliente encerrou sem êxito espera por mensagem do servidor";
			break;
		case CLIENT_CONNECT_ERROR:
			str = "falha em tentativa de conexão do cliente com o servidor";
			break;
		case CLIENT_ERROR:
			str = "erro interno no cliente";
			break;
		default:
			str = "mensagem não compreendida";
	}
	return Format("código #%d: %s", id, str);
}

