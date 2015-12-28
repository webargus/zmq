#ifndef _lazypirateserver_GUI_ZMQServer_h_
#define _lazypirateserver_GUI_ZMQServer_h_

#include <Core/Core.h>
//#define ZMQ_STATIC
#include "zmq.hpp"

using namespace Upp;

class ZMQServer {
	
	public:
		typedef ZMQServer CLASSNAME;
		ZMQServer();
		~ZMQServer() { serverStop(); }
		void serverStart();
		void serverStop();
		void serverLoop();
		virtual String processMessage(String msg) = 0;

	private:
		zmq::context_t context;
		bool running;
};

#endif
