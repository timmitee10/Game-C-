#pragma once
#include <type_traits>

#include "Config.h"

namespace NodelNet
{
	/*
	 * Note IPVersion has overwritten assignment operator to support IPv4OrIPv6 and IPv4AndIPv6
	 */
	enum class IPVersion
	{
		Unknown,
		IPv4 = AF_INET,
		IPv6 = AF_INET6,
		IPv4OrIPv6,
		IPv4AndIPv6,
	};
	typedef std::underlying_type<IPVersion>::type IPVersionUT;

	NODISCARD constexpr bool operator==(IPVersion lhs, IPVersion rhs)
	{

		if (static_cast<IPVersionUT>(lhs) == static_cast<IPVersionUT>(rhs)) return true;
		if (static_cast<IPVersionUT>(lhs) == static_cast<IPVersionUT>(IPVersion::IPv4OrIPv6)
			|| static_cast<IPVersionUT>(rhs) == static_cast<IPVersionUT>(IPVersion::IPv4OrIPv6))
		{
			if (static_cast<IPVersionUT>(lhs) == static_cast<IPVersionUT>(IPVersion::IPv4)
				|| static_cast<IPVersionUT>(lhs) == static_cast<IPVersionUT>(IPVersion::IPv6)) return true;
			if (static_cast<IPVersionUT>(rhs) == static_cast<IPVersionUT>(IPVersion::IPv4)
				|| static_cast<IPVersionUT>(rhs) == static_cast<IPVersionUT>(IPVersion::IPv6)) return true;
		}
		return false;
	}

	NODISCARD constexpr bool operator!=(const IPVersion lhs, const IPVersion rhs)
	{
		return !(lhs == rhs);
	}

	constexpr const char* ToString(IPVersion x)
	{
		switch (x)
		{
		case IPVersion::IPv4: return "IPv4";
		case IPVersion::IPv6: return "IPv6";
		case IPVersion::IPv4AndIPv6: return "IPv4AndIPv6";
		case IPVersion::IPv4OrIPv6: return "IPv4OrIPv6";
		case IPVersion::Unknown: return "Unknown";
		default: return "Invalid";
		};
	}
}
