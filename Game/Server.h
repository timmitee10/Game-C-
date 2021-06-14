#pragma once
#include "Connection.h"
#include "Packet.h"
namespace NodelNet
{
	class Server
	{
	public:
		bool Initialize(IPEndPoint ip);
		void Frame();
	protected:
		virtual void OnConnect(Connection& newConnection);
		virtual void OnDisconnect(Connection& lostConnection, std::string reason);
		void CloseConnection(int connectionIndex, std::string reason);
		virtual bool ProcessPacket(std::shared_ptr<Packet> packet);
		unsigned int clientCount = 0;
		Socket listeningSocket;
		std::vector<Connection> connections;
		std::vector<WSAPOLLFD> master_fd;
		std::vector<WSAPOLLFD> use_fd;
	};
}