#include "IPAddress.h"
namespace NodelNet
{
	IPAddress::IPAddress(const char* ip)
	{
		if (!ParseIPAddress(*this, ip))
		{
			throw std::exception("InvalidParserInput");
		}
	}

	IPAddress::IPAddress(const std::string& ip)
	{
		if (!ParseIPAddress(*this, ip))
		{
			throw std::exception("InvalidParserInput");
		}
	}

	IPVersion IPAddress::GetVersion() const noexcept
	{
		return version;
	}
	bool NodelNet::IPAddress::IsIPv6() const noexcept
	{
		return version == IPVersion::IPv6 ? true : false;
	}

	bool IPAddress::IsIPv4() const noexcept
	{
		return version == IPVersion::IPv4 ? true : false;
	}

	IPAddressV6& IPAddress::AsIPv6() const
	{
		return addr.ipAddress6;
	}

	IPAddressV4& IPAddress::AsIPv4() const
	{
		return addr.ipAddress4;
	}

	std::string IPAddress::GetStringRepresentation() const
	{
		assert(this->version != IPVersion::Unknown);
		std::stringstream ss;
		if (this->IsIPv4())
		{
			int index = 0;
			for (auto& i : addr.ipAddress4.addr.bytes)
			{
				ss << static_cast<int>(i);
				if (index < 3)
					ss << '.';
				index++;
			}
			return ss.str();
			//return std::string(addr.ipAddress4.addr.bytes.begin(), addr.ipAddress4.addr.bytes.end());
		}
		else if (this->IsIPv6())
		{
			int index = 0;
			for (auto& i : addr.ipAddress6.addr6.bytes)
			{
				ss << static_cast<int>(i);
				if (index < 8)
					ss << ':';
				index++;
			}
			return ss.str();

			//return std::string(addr.ipAddress6.addr6.bytes.begin(), addr.ipAddress6.addr6.bytes.end());
		}
		throw std::exception("InvalidParserInput");
	}

	bool IPAddress::ParseIPAddress(IPAddress& addr, const std::string& ip) noexcept
	{
		if (!IPAddressV4::ParseIPAddressV4(addr.addr.ipAddress4, ip))
		{
			return IPAddressV6::ParseIPAddressV6(addr.addr.ipAddress6, ip);
		}
		return true;
	}

	void IPAddress::Clear() noexcept
	{
		static_assert(sizeof(this->addr.ipAddress6) > sizeof(this->addr.ipAddress4.addr)); //safety check for memset
		memset(&addr.ipAddress6.addr6.bytes[0], 0x0, sizeof(addr.ipAddress6));
		this->version = IPVersion::Unknown;
	}

	IPAddress::IPAddress(const sockaddr_in& addr4) : version(IPVersion::IPv4)
	{
		assert(addr4.sin_family == AF_INET);
		memcpy(static_cast<void*>(&this->addr), &addr4.sin_addr, sizeof(addr4));
	}

	IPAddress::IPAddress(const sockaddr_in6& addr6) : version(IPVersion::IPv6)
	{
		assert(addr6.sin6_family == AF_INET6);
		memcpy(static_cast<void*>(&this->addr), &addr6.sin6_addr, sizeof(addr6));
	}

	IPAddress::IPAddress(const in_addr& addr4) : version(IPVersion::IPv4)
	{
		memcpy(&this->addr.ipAddress4.addr.bytes[0], &addr4, sizeof(addr4));
	}

	IPAddress::IPAddress(const in6_addr& addr6) : version(IPVersion::IPv6)
	{
		memcpy(&this->addr.ipAddress6.addr6.bytes[0], &addr6, sizeof(addr6));
	}

	IPAddress::IPAddress(const IPAddressV4& addr4) noexcept : version(IPVersion::IPv4)
	{
		this->addr.ipAddress4 = addr4;
	}

	IPAddress::IPAddress(const IPAddressV6& addr6) noexcept : version(IPVersion::IPv6)
	{
		this->addr.ipAddress6 = addr6;
	}

	IPAddress::IPAddress(const sockaddr* addr)
	{
		assert(addr != nullptr);
		assert(addr->sa_family == AF_INET || addr->sa_family == AF_INET6);
		if (addr->sa_family == AF_INET)
		{
			memcpy(&this->addr.ipAddress4.addr.bytes[0], addr->sa_data, sizeof(in_addr));
			this->version = IPVersion::IPv4;
			//const struct sockaddr_in* addr = reinterpret_cast<const struct sockaddr_in*>(addr);
		}
		else if (addr->sa_family == AF_INET6)
		{
			//TO DO: test, warning might read 16 out of 14 bytes warning
			memcpy(&this->addr.ipAddress6.addr6.bytes[0], (in6_addr*)&addr->sa_data, sizeof(in6_addr));
			this->version = IPVersion::IPv6;
		}
	}
	//TO DO: Fix so that addr version is chosen based on addrLengt(socklen_t)
	IPAddress::IPAddress(const sockaddr* addr, socklen_t addrLength)
	{
		assert(addr != nullptr);
		assert(addr->sa_family == AF_INET || addr->sa_family == AF_INET6);
		if (addr->sa_family == AF_INET)
		{
			memcpy(&this->addr.ipAddress4.addr.bytes[0], reinterpret_cast<in_addr*>(&addr), sizeof(in_addr));
			//const struct sockaddr_in* addr = reinterpret_cast<const struct sockaddr_in*>(addr);
			this->version = IPVersion::IPv4;
		}
		else if (addr->sa_family == AF_INET6)
		{
			memcpy(&this->addr.ipAddress6.addr6.bytes[0], reinterpret_cast<in6_addr*>(&addr), sizeof(in6_addr));
			this->version = IPVersion::IPv6;
		}
	}
	IPAddress& IPAddress::operator=(const IPAddress& ipAddr) noexcept
	{
		this->version = ipAddr.version;
		this->addr.ipAddress6 = ipAddr.addr.ipAddress6;
		return *this;
	}

	IPAddress& IPAddress::operator=(const IPAddressV4& ipAddr4) noexcept
	{
		this->Clear();
		this->addr.ipAddress4 = ipAddr4;
		return *this;
	}

	IPAddress& IPAddress::operator=(const IPAddressV6& ipAddr6) noexcept
	{
		this->Clear();
		this->addr.ipAddress6 = ipAddr6;
		return *this;
	}

	IPAddress& IPAddress::operator=(IPAddress&& ipAddr) noexcept
	{
		*this = ipAddr;
		return *this;
	}

	IPAddress& IPAddress::operator=(IPAddressV4&& ipAddr4) noexcept
	{
		this->addr.ipAddress4 = ipAddr4;
		return *this;
	}

	IPAddress& IPAddress::operator=(IPAddressV6&& ipAddr6) noexcept
	{
		this->addr.ipAddress6 = ipAddr6;
		return *this;
	}

	bool IPAddress::operator==(const IPAddress& ipAddr) const noexcept
	{
		if (this->IsIPv4() && ipAddr.IsIPv4())
			return this->addr.ipAddress4 == ipAddr.addr.ipAddress4;
		if (this->IsIPv6() && ipAddr.IsIPv6())
			return this->addr.ipAddress6 == ipAddr.addr.ipAddress6;
		return  false; //IPAddress did not match version
	}

	bool IPAddress::operator==(const IPAddressV4& ipAddr4) const noexcept
	{
		if (this->IsIPv4())
		{
			return this->addr.ipAddress4 == ipAddr4;
		}
		return false;
	}

	bool IPAddress::operator==(const IPAddressV6& ipAddr6) const noexcept
	{
		if (this->IsIPv6())
		{
			return this->addr.ipAddress6 == ipAddr6;
		}
		return false;
	}

	bool IPAddress::operator!=(const IPAddress& ipAddr) const noexcept
	{
		return !this->operator==(ipAddr);
	}
	bool IPAddress::operator!=(const IPAddressV4& ipAddr4) const noexcept
	{
		return !this->operator==(ipAddr4);
	}
	bool IPAddress::operator!=(const IPAddressV6& ipAddr6) const noexcept
	{
		return !this->operator==(ipAddr6);
	}
	std::ostream& operator<<(std::ostream& rhs, const IPAddress& lhs)
	{
		assert(lhs.version != IPVersion::Unknown);
		const auto ipVers = lhs.GetVersion();
		if (ipVers == IPVersion::IPv4)
		{
			rhs << lhs.AsIPv4();
		}
		else if (ipVers == IPVersion::IPv6)
		{
			rhs << lhs.AsIPv6();
		}
		return rhs;
	}

}