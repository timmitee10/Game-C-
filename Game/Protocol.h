#pragma once
#include <WinSock2.h>
namespace NodelNet
{
	enum class Protocol
	{
		TCP = IPPROTO_TCP,
		UDP = IPPROTO_UDP,
		ICMP = IPPROTO_ICMP,
		ICMPv6 = IPPROTO_ICMPV6,
		//Own protocol implementation that use end to end encryption to encrypt sent messages with an overhead of one RTT
		//Also support user-name, password and unique_user_identifier sign in with one RTT. Will in both cases return the chosen encryption schema.
		//The protocol is intended for use within games and the performance is therefore a key aspect.
		NodelProtocol,
		/* Using OpenSLL implementation for SSL/TLS - protocol. */
		SSL,
		TLS,
		Unknown
	};
}