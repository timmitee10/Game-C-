#include "Connection.h"
namespace NodelNet
{

	Connection::Connection(Socket& socket, IPEndPoint ipEnd) : sock(socket) , ipEnd(ipEnd)
	{
	}
	void Connection::Close()
	{
		sock.Close();
	}
	Socket& Connection::GetSocket() 
	{ 
		return sock; 
	}
	
	IPEndPoint Connection::GetIPEndPoint() const 
	{
		return ipEnd; 
	}
}