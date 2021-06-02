#pragma once
namespace NodelNet
{
	enum class SocketOption
	{
		IPv6_Only,
		TCP_NoDelay,
		TTL,
		//SO
		Broadcast,
		ConditionalAccept,
		Debug,
		DontLinger,
		DontRoute,
		GroupPriority,
		KeepAlive,
		Linger,
		OOBinLine,
		RcvBuf,
		ReuseAddr,
		ExclusiveAddrrUse,
		RcvTimeo,
		Sndbuf,
		SndTimeo,
		UpdateAcceptContext,
		//PVD
		Config,
	};
}