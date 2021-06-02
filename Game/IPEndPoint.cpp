#include "IPEndPoint.h"
namespace NodelNet
{
	IPEndPoint::IPEndPoint(const sockaddr* addr, socklen_t addrLength)
	{
		assert(addr != nullptr);
		assert(addr->sa_family == AF_INET || addr->sa_family == AF_INET6);
		if (addr->sa_family == AF_INET)
		{
			const auto* addr4 = reinterpret_cast<const sockaddr_in*>(&addr);
			memcpy(&this->addr.ipAddress4.addr.bytes[0], &addr4->sin_addr, sizeof(in_addr));
			this->port = ntohs(addr4->sin_port);
			this->version = IPVersion::IPv4;
		}
		else if (addr->sa_family == AF_INET6)
		{
			const auto* addr6 = reinterpret_cast<const sockaddr_in6*>(&addr);
			memcpy(&this->addr.ipAddress6.addr6.bytes[0], &addr6->sin6_addr, sizeof(in6_addr));
			this->port = ntohs(addr6->sin6_port);
			this->version = IPVersion::IPv6;
		}
	}
	bool IPEndPoint::operator==(const IPEndPoint& rhs) const
	{
		if (this->port != rhs.port)
			return false;
		if (static_cast<IPAddress>(*this) != static_cast<IPAddress>(rhs))
			return false;
		return true;
	}

	bool IPEndPoint::operator==(const port_host_byte_order_t rhs) const
	{
		return this->port == rhs;
	}

	bool IPEndPoint::operator==(const port_network_byte_order_t rhs) const
	{
		return this->port == to_host_byte_order(rhs);
	}

	bool IPEndPoint::operator!=(const IPEndPoint& rhs) const
	{
		return !this->operator==(rhs);
	}

	bool IPEndPoint::operator!=(const port_host_byte_order_t rhs) const
	{
		return !this->operator==(rhs);
	}

	bool IPEndPoint::operator!=(const port_network_byte_order_t rhs) const
	{
		return !this->operator==(rhs);
	}

	IPEndPoint& IPEndPoint::operator=(const IPEndPoint& rhs)
	{
		this->port = rhs.port;
		std::copy(this->addr.ipAddress6.addr6.bytes.begin(), this->addr.ipAddress6.addr6.bytes.end(), rhs.addr.ipAddress6.addr6.bytes.begin());
		return *this;
	}

	IPEndPoint& IPEndPoint::operator=(const port_host_byte_order_t& rhs)
	{
		this->port = rhs;
		return *this;
	}

	IPEndPoint& IPEndPoint::operator=(const port_network_byte_order_t& rhs)
	{
		this->port = to_host_byte_order(rhs);
		return *this;
	}

	IPEndPoint& IPEndPoint::operator=(IPEndPoint&& rhs) noexcept
	{
		*this = rhs;
		return *this;
	}

	bool IPEndPoint::operator<(const IPEndPoint& lhs) const
	{
		//IPv4 first then IPv6 address.
		if (static_cast<int>(this->version) != static_cast<int>(lhs.version))
		{
			return static_cast<int>(this->version) < static_cast<int>(lhs.version);
		}
		if (this->addr.ipAddress6.addr6.bytes < lhs.addr.ipAddress6.addr6.bytes)
			return true;
		return this->port < lhs.port;
	}

	port_host_byte_order_t IPEndPoint::GetPort() const
	{
		return port;
	}

	port_network_byte_order_t IPEndPoint::GetPortNetworkByteOrder() const
	{
		return to_network_byte_order(port);
	}
	IPAddress IPEndPoint::GetIPAddress() const
	{
		IPAddress ip(this->addr.ipAddress6);
		ip.version = this->GetVersion();
		return ip;
	}

	sockaddr_in IPEndPoint::GetAddressIPv4() const
	{
		assert(version == IPVersion::IPv4);
		sockaddr_in addrIn = {};
		memcpy(&addrIn.sin_addr.S_un.S_addr, &addr.ipAddress4, sizeof(addr.ipAddress4));
		addrIn.sin_port = to_integer(to_network_byte_order(port)); // host to network-byte order
		addrIn.sin_family = AF_INET;
		return addrIn;
	}

	sockaddr_in6 IPEndPoint::GetAddressIPv6() const
	{
		assert(version == IPVersion::IPv6 || version == IPVersion::IPv4);
		sockaddr_in6 addr6In = {};
		memcpy(&addr6In.sin6_addr.u.Byte[0], &addr.ipAddress6, sizeof(addr.ipAddress6));
		addr6In.sin6_port = to_integer(to_network_byte_order(port)); // host to network-byte order
		addr6In.sin6_family = AF_INET6;
		return addr6In;
	}
	std::string IPEndPoint::GetStringRepresentationWithPort() const
	{
		std::string str = GetStringRepresentation();
		str.append(":" + std::to_string(port));
		return str;
	}

	void IPEndPoint::Clear() noexcept
	{
		static_assert(sizeof(this->addr.ipAddress6) > sizeof(this->addr.ipAddress4)); //safety check for memset
		memset(&addr.ipAddress6.addr6.bytes[0], 0x0, sizeof(addr.ipAddress6));
		this->version = IPVersion::Unknown;
		this->port = DEFAULT_IP_ENDPOINT_PORT;
	}

	std::ostream& operator<<(std::ostream& rhs, const IPEndPoint& lhs)
	{
		return rhs << lhs.GetStringRepresentationWithPort();
	}
}

