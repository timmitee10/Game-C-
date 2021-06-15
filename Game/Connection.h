#pragma once
#include "Socket.h"
#include "PacketManager.h"
namespace NodelNet
{

	class Connection
	{
	public:
		Connection(Socket& socket, IPEndPoint ipEnd);
		void Close();
		Socket& GetSocket();
		IPEndPoint GetIPEndPoint() const;
		PacketManager pm_incoming;
		PacketManager pm_outgoing;
		char buffer[Constants::max_packet_size];
	private:


		Socket sock;
		IPEndPoint ipEnd;
	};
}