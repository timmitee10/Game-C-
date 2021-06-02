#pragma once
#include <WinSock2.h>
namespace NodelNet
{
	enum class SocketType
	{
		Unknown,
		RAW = SOCK_RAW,
		DGRAM = SOCK_DGRAM,
		STREAM = SOCK_STREAM,
		RDM = SOCK_RDM,
		SEQPACKET = SOCK_SEQPACKET
	};
}