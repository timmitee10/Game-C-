#include "Socket.h"

namespace NodelNet
{
	Socket::~Socket()
	{
		if (Close() != NETResult::Success)
			std::terminate();
	}

	Socket& Socket::operator=(const Socket& lhs)
	{
		this->ipVersion = lhs.ipVersion;
		this->isIPv6Only = lhs.isIPv6Only;
		this->protocol = lhs.protocol;
		this->sock = lhs.sock;
		this->socketIOModel = lhs.socketIOModel;
		return *this;
	}

	Socket::Socket(IPVersion ipVersion) noexcept : ipVersion(ipVersion), sock(INVALID_SOCKET)
	{
		assert(ipVersion == IPVersion::IPv4 || ipVersion == IPVersion::IPv6);
	}

	NETResult Socket::Create(const Protocol socketProtocol, SocketType socketType)
	{
		assert(ipVersion == IPVersion::IPv4 || ipVersion == IPVersion::IPv6);
		if (sock != INVALID_SOCKET)
		{
			NETCHECK_ARG("Socket handle was invalid");
			return NETResult::GenericError;
		}
		switch (socketProtocol)
		{
		case Protocol::TCP:
			if (socketType == SocketType::Unknown)
				socketType = SocketType::STREAM;
			break;
		case Protocol::UDP:
			if (socketType == SocketType::Unknown)
				socketType = SocketType::DGRAM;
			break;
		case Protocol::ICMP:
			if (socketType == SocketType::Unknown)
				socketType = SocketType::RAW;
			if (ipVersion == IPVersion::IPv4)
			{

			}
			else
			{
				NETCHECK_ARG("The specified protocol does not support this IP version");
				return NETResult::GenericError;
			}
			break;
		case Protocol::ICMPv6:
			if (socketType == SocketType::Unknown)
				socketType = SocketType::RAW;
			if (ipVersion == IPVersion::IPv6)
			{
			}
			else
			{
				NETCHECK_ARG("The specified protocol does not support this IP version");
				return NETResult::GenericError;
			}
			break;
		default:
			NETCHECK_ARG("Socket does not support this protocol or the protocol was invalid");
			return NETResult::GenericError;
			break;
		}
		sock = socket(ipVersion == IPVersion::IPv4 ? AF_INET : AF_INET6, static_cast<unsigned int>(socketType), static_cast<int>(socketProtocol));

		if (sock == INVALID_SOCKET)
		{
			NETCHECK_ARG("Failed to create socket");
			return NETResult::GenericError;
		}
		return NETResult::Success;
	}


	NETResult Socket::Bind(const IPEndPoint& ipEndPoint)
	{
		assert(ipVersion == IPVersion::IPv4 || ipVersion == IPVersion::IPv6);
		if (sock == INVALID_SOCKET)
		{
			NETCHECK_ARG("Invalid socket handle");
			return NETResult::GenericError;
		}
		IPVersion ipEndPointVersion = ipEndPoint.GetVersion();
		int result = 0;
		if (ipVersion == IPVersion::IPv4 && isIPv6Only == FALSE)
		{
			sockaddr_in addr = ipEndPoint.GetAddressIPv4();
			result = bind(sock, reinterpret_cast<struct sockaddr*>(&addr), ipVersion == IPVersion::IPv4 ? sizeof(sockaddr_in) : sizeof(sockaddr_in6));
		}
		if (ipVersion == IPVersion::IPv6)
		{
			sockaddr_in6 addr = ipEndPoint.GetAddressIPv6();
			result = bind(sock, reinterpret_cast<struct sockaddr*>(&addr), ipVersion == IPVersion::IPv4 ? sizeof(sockaddr_in) : sizeof(sockaddr_in6));
		}
		if (result != 0)
		{
			NETCHECK();
			return NETResult::GenericError;
		}
		return NETResult::Success;
	}

	NETResult Socket::Connect(const IPEndPoint& ipEndPoint)
	{
		assert(ipVersion == IPVersion::IPv4 || ipVersion == IPVersion::IPv6);
		if (sock == INVALID_SOCKET)
		{
			NETCHECK_ARG("Invalid socket handle");
			return NETResult::GenericError;
		}
		int result = 0;
		if (ipVersion == IPVersion::IPv4 && isIPv6Only == FALSE && (ipEndPoint.GetVersion() == IPVersion::IPv4)) //ipEndPoint must be IPv4, IPv6 can not be mapped to IPv4.
		{
			auto addr = ipEndPoint.GetAddressIPv4();
			result = connect(sock, reinterpret_cast<struct sockaddr*>(&addr), sizeof(sockaddr_in));

		}
		else if (ipVersion == IPVersion::IPv6) //ipEndPoint can be either IPv4 and IPv6
		{
			auto addr = ipEndPoint.GetAddressIPv6();
			result = connect(sock, reinterpret_cast<struct sockaddr*>(&addr), sizeof(sockaddr_in6));

		}
		else
		{
			NETCHECK_ARG("Failed to connect to specified address");
			return NETResult::GenericError;
		}
		if (result != 0)
		{

			NETCHECK_ARG(std::string("Failed to connect to " + ipEndPoint.GetStringRepresentation()).c_str());
			return NETResult::GenericError;
		}
#ifndef NDEBUG
		std::cout << "Connected to: " << ipEndPoint.GetStringRepresentation() << std::endl;
#endif
		return NETResult::Success;
	}

	NETResult Socket::Listen(const int backlog)
	{
		if (ipVersion == IPVersion::IPv6)
		{
			if (SetSocketOption(SocketOption::IPv6_Only, FALSE) != NETResult::Success)
			{
				return NETResult::GenericError;
			}
		}
		int result = 0;
		result = listen(sock, backlog);
		if (result != 0)
		{
			NETCHECK();
			return NETResult::GenericError;
		}
		return NETResult::Success;
	}

	NETResult Socket::Send(const void* const dataBuffer, const int numOfBytes, int& bytesSent)
	{
		assert(sock != INVALID_SOCKET);
		bytesSent = send(sock, static_cast<const char*>(dataBuffer), numOfBytes, 0);
#ifndef NDEBUG
		std::cout << "Sent:" << bytesSent << " bytes" << std::endl;
#endif
		if (bytesSent <= 0)
		{
			return NETResult::GenericError;
		}
		return NETResult::Success;
	}

	NETResult Socket::SendTo(const IPEndPoint ip, const void* const data, const int numOfBytes, int& bytesSent)
	{
		if (ip.IsIPv4())
		{
			sockaddr_in addr = ip.GetAddressIPv4();
			sendto(sock, (const char*)data, numOfBytes, NULL, (sockaddr*)&addr, sizeof(addr));
		}
		else if (ip.IsIPv6())
		{
			sockaddr_in6 addr6 = ip.GetAddressIPv6();
			sendto(sock, (const char*)data, numOfBytes, NULL, (sockaddr*)&addr6, sizeof(addr6));
		}
		return NETResult::Success;
	}

	NETResult Socket::SendAll(const void* data, int numOfBytes)
	{
		int byteSent = 0;
		NETResult result;
		while (byteSent < numOfBytes)
		{
			if ((result = Send(data, numOfBytes, byteSent)) != NETResult::Success)
			{
				return result;
			}
		}
		return NETResult::Success;
	}

	NETResult Socket::SendAllTo(IPEndPoint ip, const void* data, int numOfBytes)
	{
		int byteSent = 0;
		NETResult result;
		while (byteSent < numOfBytes)
		{
			if ((result = SendTo(ip, data, numOfBytes, byteSent)) != NETResult::Success)
			{
				return result;
			}
		}
		return NETResult::Success;
	}

	NETResult Socket::Receive(void* dataBuffer, const int numOfBytes, int& bytesReceived)
	{
		assert(sock != INVALID_SOCKET);
		bytesReceived = recv(sock, static_cast<char*>(dataBuffer), numOfBytes, 0);
#ifndef NDEBUG
		std::cout << "Received:" << bytesReceived << " bytes" << std::endl;
#endif
		if (bytesReceived < 0)
		{
			return NETResult::GenericError;
		}
		if (bytesReceived == 0)
		{
			//Disconnect
			return NETResult::GenericError;
		}

		return NETResult::Success;
	}
	NETResult Socket::ReceiveFrom(IPEndPoint ip, void* dataBuffer, int numOfBytes, int& bytesReceived)
	{

		int addrSize = 0;
		sockaddr* addr;
		sockaddr_in addr4; 
		sockaddr_in6 addr6; 
		switch (this->ipVersion)
		{
		case IPVersion::IPv4:
			if (ip.IsIPv4())
			{
				addr4 = ip.GetAddressIPv4();
				addrSize = sizeof(sockaddr_in);
				addr = (sockaddr*)&addr4;
			}
			break;
		case IPVersion::IPv6:
			if (ip.IsIPv6())
			{
				addr6 = ip.GetAddressIPv6();
				addrSize = sizeof(sockaddr_in6);
				addr = (sockaddr*)&addr6;
			}
			break;
		default:
		{
			NETCHECK_ARG("Socket did not have a IPVersion specified")
				return NETResult::GenericError;
		}
		}
		bytesReceived = recvfrom(sock, static_cast<char*>(dataBuffer), numOfBytes, 0, (sockaddr*)&addr, &addrSize);
#ifndef NDEBUG
		std::cout << "Received:" << bytesReceived << " bytes" << std::endl;
#endif
		if (bytesReceived == SOCKET_ERROR)
		{
			NETCHECK();
			return NETResult::GenericError;
		}
		if (bytesReceived == 0)
		{
			//Disconnect
			return NETResult::Disconnect;
		}
		return NETResult::Success;
	}

	NETResult Socket::ReceiveAll(void* destination, const int numOfBytes)
	{
		int totalBytesReceived = 0;
		while (totalBytesReceived < numOfBytes)
		{
			int bytesRemaining = numOfBytes - totalBytesReceived;
			int byteReceived = 0;
			char* bufferOffset = (char*)destination + totalBytesReceived;
			NETResult result = Receive(bufferOffset, bytesRemaining, byteReceived);
			if (result != NETResult::Success)
			{
				return NETResult::GenericError;
			}
			totalBytesReceived += byteReceived;
		}
		return NETResult::Success;
	}
	NETResult Socket::Accept(SocketHandle& acceptedHandle)
	{
		if (ipVersion == IPVersion::IPv4)
		{

			sockaddr_in addr;
			int len = sizeof(addr);
			acceptedHandle = accept(sock, (sockaddr*)(&addr), &len);
		}
		else //IPv6
		{
			sockaddr_in6 addr;
			int len = sizeof(addr);
			acceptedHandle = accept(sock, (sockaddr*)(&addr), &len);
		}

		if (acceptedHandle == INVALID_SOCKET)
		{
			NETCHECK();
			return NETResult::GenericError;
		}
		return NETResult::Success;
	}
	NETResult Socket::Accept(SocketHandle& acceptedHandle, const IPEndPoint& ipEnd)
	{
		assert(ipVersion == IPVersion::IPv4 || ipVersion == IPVersion::IPv6);
		switch (this->ipVersion)
		{
		case IPVersion::IPv4:
		{
			int len = sizeof(sockaddr_in);
			sockaddr_in addr4 = ipEnd.GetAddressIPv4();
			acceptedHandle = accept(sock, (sockaddr*)&addr4, &len);
			if (acceptedHandle == INVALID_SOCKET)
			{
				NETCHECK_ARG("Invalid socket, could not accept connection, accepted connection handle INVALID_SOCKET");
				return NETResult::GenericError;
			}
#ifndef NDEBUG
			std::cout << "New IPv4 connection accepted: " << ipEnd.GetStringRepresentation() << '.' << '\n';
#endif
		}
		break;
		case IPVersion::IPv6:
		{
			int len = sizeof(sockaddr_in6);
			sockaddr_in6 addr6= ipEnd.GetAddressIPv6();
			acceptedHandle = accept(sock,(sockaddr*)&addr6 , &len);
			if (acceptedHandle == INVALID_SOCKET)
			{
				NETCHECK_ARG("Invalid socket, could not accept connection, accepted connection handle INVALID_SOCKET");
				return NETResult::GenericError;
			}
#ifndef NDEBUG
			std::cout << "New IPv6 connection accepted: " << ipEnd.GetStringRepresentation() << '.' << '\n';
#endif
		}
		break;
		default:
			throw std::exception("The socket does not have a IPVersion assigned");
			break;
		}
		return NETResult::Success;
	}
	NETResult Socket::Accept(Socket& outSocket, IPEndPoint& ipEnd)
	{
		if (outSocket.GetHandle() != INVALID_SOCKET)//Check if connection has already been initialized with socket handle
		{
			//Close connection in order to not 
#ifndef NDEBUG
			std::cout << "Socket.Accept had a outSocket that was already initialized with SocketHandle consider closing this connection before calling Accept" << '\n';
#endif
			NETResult res = outSocket.Close();
			if (res != NETResult::Success)
			{
				return res;
			}
		}
		SocketHandle handle = INVALID_SOCKET;
		Accept(handle, ipEnd);
		outSocket = Socket(handle, ipEnd.GetVersion());
		return NETResult::Success;
	}
	NETResult Socket::Close() noexcept
	{
		if (sock == INVALID_SOCKET)
		{
			//There were no resource to close.
			return NETResult::Success;
		}
#ifdef OS_WINDOWS
		int result = closesocket(sock);
#elif defined(OS_SYSTEM_UNIX)
		int result = close(sock);
#endif // WINDOWS / UNIX
		if (result != 0)
		{
			NETCHECK_ARG("Failed to close socket");
			return NETResult::GenericError;
		}
		sock = INVALID_SOCKET;
		return NETResult::Success;
	}

	NETResult Socket::SetBlocking(const bool isBlocking)
	{
		unsigned long nonBlocking = 1;
		unsigned long blocking = 0;
#ifdef OS_WINDOWS
		int result = ioctlsocket(sock, FIONBIO, isBlocking ? &blocking : &nonBlocking);
#elif defined(OS_SYSTEM_UNIX)
		int result = ioctl(sock, FIONBIO, isBlocking ? &blocking : &nonBlocking);
#endif

		if (result != SOCKET_ERROR)
		{
			NETCHECK();
			return NETResult::GenericError;
		}

		return NETResult::Success;
	}

	NETResult Socket::SetSocketIOModel(const SocketIOModels socketIOModel)
	{
		Close();
		if (socketIOModel == SocketIOModels::Unknown)
		{
			assert("Invalid SocketIOModel specified");
			return NETResult::GenericError;
		}
		switch (socketIOModel)
		{
		case SocketIOModels::Blocking:

			break;
		case SocketIOModels::OverlappedIO:
		{ //Must be a WINSOCK or it is not supported

		}
		break;
		case SocketIOModels::Polling:
			return NETResult::GenericError;
			break;
		case SocketIOModels::Select:
			return NETResult::GenericError;
			break;
		default:
			return NETResult::GenericError;
			break;
		}
		this->socketIOModel = socketIOModel;
		return NETResult::Success;
	}

	NETResult Socket::SetSocketOption(const SocketOption option, const BOOL value) //value can also be a int(BOOL == int) should not be a bool it can return WSAPROTOCOL_INFOA which is bigger fix
	{
		int result = 0;
		switch (option)
		{
		case SocketOption::IPv6_Only:
		{
			if (ipVersion == IPVersion::IPv4)
			{
				NETCHECK_ARG("Can not set socket option to only use IPv6 because the socket is IPv4");
				return NETResult::GenericError;
			}
			result = setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<const char*>(&value), sizeof(value));
			if (result == 0)
			{
				isIPv6Only = value;
				ipVersion = IPVersion::IPv6;
			}
		}
		break;
		case NodelNet::SocketOption::TTL:
			result = setsockopt(sock, IPPROTO_IP, IP_TTL, reinterpret_cast<const char*>(value), sizeof(value));
			break;
		default:
			NETCHECK_ARG("Invalid option specified");
			return NETResult::GenericError;
			break;
		}

		if (result != 0)
		{
			NETCHECK_ARG("Failed to set socket option using sockopt()");
			return NETResult::GenericError;
		}
		return NETResult::Success;
	}
	NETResult Socket::GetSocketOption(const SocketOption option, BOOL& value) const
	{
		int result = 0;
		int optionLenght = sizeof(value);
		switch (option)
		{
		case NodelNet::SocketOption::IPv6_Only:
			result = getsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<char*>(value), &optionLenght);
			break;
		default:
			NETCHECK_ARG("Option not specified");
			return NETResult::GenericError;
			break;
		}
		if (result != 0)
		{
			NETCHECK_ARG("Failed to retrieve socket option");
			return NETResult::GenericError;
		}

		return NETResult::Success;
	}
	SocketHandle Socket::GetHandle() const
	{
		return sock;
	}
}
