#pragma once
#define NOMINMAX
#include "IPEndPoint.h"
#include "NETResult.h"
#include "SocketType.h"
#include "Protocol.h"
#include "SocketOption.h"
#include "SocketIOModels.h"
#include <cassert>
#include "SocketException.h"
namespace NodelNet
{
	/*
		Generic socket wrapper implementing all available operations that can be used from socket.
		Socket is treated as one object that can not be copied but only moved.
	*/
	class Socket
	{
	public:
		Socket(SocketHandle& socketHandle, IPVersion ipVer) : sock(socketHandle), ipVersion(ipVer) {}
		Socket(IPVersion ipVersion = IPVersion::IPv4) noexcept;
		NETResult Create(const Protocol socketProtocol, SocketType socketType = SocketType::Unknown);
		~Socket() noexcept;
		Socket& operator=(const Socket& lhs);
	public:
		NETResult Bind(const IPEndPoint& ipEndPoint);
		NETResult Connect(const IPEndPoint& ipEndPoint);
		NETResult Listen(const int backlog = 32);
#pragma region Send/Recv
		NETResult Send(const void* const data, const int numOfBytes, int& bytesSent);
		NETResult SendTo(const IPEndPoint ip, const void* const data, const int numOfBytes, int& bytesSent);

		NETResult SendAll(const void* data, int numOfBytes);
		NETResult SendAllTo(IPEndPoint ip, const void* data, int numOfBytes);

		NETResult Receive(void* destination, const int numOfBytes, int& bytesReceived);
		NETResult ReceiveFrom(IPEndPoint ip, void* dataBuffer, int numOfBytes, int& bytesReceived);

		NETResult ReceiveAll(void* destination, const int numOfBytes);
#pragma endregion Send and recive methods 
		NETResult Accept(SocketHandle& acceptedHandle);
		NETResult Accept(SocketHandle& acceptedHandle, const IPEndPoint& ipEnd);
		NETResult Accept(Socket& outSocket, IPEndPoint& ipEnd);

		NETResult Close() noexcept;
		NETResult SetBlocking(const bool isBlocking);
		NETResult SetSocketIOModel(const SocketIOModels socketIOModel);
		NETResult SetSocketOption(const SocketOption option, const BOOL value);
		NETResult GetSocketOption(const SocketOption option, BOOL& value) const;
		NODISCARD SocketHandle GetHandle() const;
	protected:
		SocketHandle sock;
		IPVersion ipVersion;
		SocketIOModels socketIOModel = SocketIOModels::Unknown;
		Protocol protocol = Protocol::Unknown;
		BOOL isIPv6Only = FALSE;
	};
}
