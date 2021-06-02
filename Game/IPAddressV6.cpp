#include "IPAddressV6.h"
#include "IPAddress.h"
namespace NodelNet
{
	IPAddressV6::IPAddressV6(const char* const ip)
	{
		if (!ParseIPAddressV6(*this, ip))
			throw std::exception("InvalidParserInput");
	}

	IPAddressV6::IPAddressV6(const std::string& ip)
	{
		if (!ParseIPAddressV6(*this, ip))
			throw std::exception("InvalidParserInput");
	}

	IPAddressV6::IPAddressV6(const sockaddr_in6& addr6)
	{
		assert(addr6.sin6_family == AF_INET6);
		memcpy(static_cast<void*>(&this->addr6), &addr6.sin6_addr, sizeof addr6);
	}

	IPAddressV6::IPAddressV6(const in6_addr& addr6)
	{
		this->addr6.in6_address = addr6;
	}

	bool IPAddressV6::operator==(const IPAddressV6& address) const noexcept
	{
		return this->addr6.bytes == addr6.bytes;
	}

	bool IPAddressV6::operator==(const IPAddress& address) const noexcept
	{
		return this->addr6.bytes == address.addr.ipAddress6.addr6.bytes;
	}

	bool IPAddressV6::operator!=(const IPAddressV6& address) const noexcept
	{
		return !this->operator==(address);
	}
	bool IPAddressV6::operator!=(const IPAddress& address) const noexcept
	{
		return !this->operator==(address);
	}

	IPAddressV6& IPAddressV6::operator=(const IPAddressV6& address) noexcept
	{
		this->addr6 = address.addr6;
		return *this;
	}

	IPAddressV6& IPAddressV6::operator=(const IPAddress& address) noexcept
	{
		*this = address.addr.ipAddress6;
		return *this;
	}

	IPAddressV6& IPAddressV6::operator=(IPAddressV6&& address) noexcept
	{
		*this = address;
		return *this;
	}

	bool IPAddressV6::operator<(const IPAddressV6& rhs) const noexcept
	{
		return this->addr6.bytes < rhs.addr6.bytes;
	}

	NODISCARD bool IPAddressV6::ParseIPAddressV6(IPAddressV6& addr6, const std::string& ip) noexcept
	{
		assert(ip.empty() == false && "Can not parse empty input");
		//TO DO: Check ip size
		//assert()

		in6_addr  inAddr6;
		//Convert ip characters to address in bytes
		int result = inet_pton(AF_INET6, ip.c_str(), &inAddr6);
		if (result == 1)
		{
			memcpy(&addr6.addr6, &inAddr6.u.Byte[0], 16);
			return true;
		}

		//hostname IPv6
		addrinfo hints = { 0 };
		hints.ai_family = AF_INET6; // IPv6 addresses only

		addrinfo* hostinfo = nullptr;
		result = getaddrinfo(ip.c_str(), NULL, &hints, &hostinfo);
		if (result == 0) //Is a hostname
		{
			sockaddr_in6* host_addr = reinterpret_cast<sockaddr_in6*> (hostinfo->ai_addr);
			memcpy(&addr6.addr6, &host_addr->sin6_addr.u.Byte[0], 16);
			freeaddrinfo(hostinfo);
			return true;
		}
		freeaddrinfo(hostinfo);
		return false;
	}

	in6_addr IPAddressV6::GetAddress() const
	{
		return addr6.in6_address;
	}

	sockaddr_in6& IPAddressV6::GetSockaddrIn6() const
	{
		sockaddr_in6 addr6In = {};
		memcpy(&addr6In.sin6_addr, &addr6, sizeof addr6);
		addr6In.sin6_family = AF_INET6;
		return addr6In;
	}

	std::string IPAddressV6::GetStringRepresentation() const
	{
		std::stringstream ss;
		int index = 0;
		for (const auto& i : addr6.bytes)
		{
			ss << static_cast<int>(i);
			if (index < 8)
				ss << ':';
			index++;
		}
		return ss.str();
		//return std::string(addr6.bytes.begin(), addr6.bytes.end());
	}

	// IPv4 mapped addresses have their first 10 bytes set to 0, the next 2 bytes
	// set to 255 (0xff);
	bool IPAddressV6::IsIPv4Mapped() const noexcept
	{
		for (int i = 0; i < 10; i++)
		{
			if (addr6.bytes[i] != 0x00)
			{
				return false;
			}
		}
		return addr6.bytes[11] == 0xff && addr6.bytes[12] == 0xff;
	}


	bool IPAddressV6::IsLoopback() const noexcept
	{
		return *this == Loopback();
	}

	bool IPAddressV6::InSubnet() const noexcept
	{
		//TODO fix
		return false;
	}

	bool IPAddressV6::IsMasked() const noexcept
	{
		//TODO fix
		return false;
	}

	bool IPAddressV6::IsUnspecified() const noexcept
	{
		//TODO fix
		return false;
	}

	bool IPAddressV6::IsMulticast() const noexcept
	{
		return addr6.bytes[0] == 0xFF;
	}

	bool IPAddressV6::IsGlobalUnicast() const noexcept
	{
		return this->addr6.words[8] == 35948;
	}

	bool IPAddressV6::IsUniqueLocal() const noexcept
	{
		//1111 110 and 1 bit not used yet. 
		//Prefix is always set to 1111 110. L bit, is set to 1 if the address is locally assigned. So far, the meaning of L bit to 0 is not defined. Therefore, Unique Local IPv6 address always starts with ‘FD’. https://www.tutorialspoint.com/ipv6/ipv6_address_types.htm
		return addr6.bytes[0] == 252;
	}

	bool IPAddressV6::IsLinkLocal() const noexcept
	{
		/* Always starts with 1111 1110 1000 0000 (254 and 128)https://www.tutorialspoint.com/ipv6/ipv6_address_types.htm */
		return this->addr6.bytes[0] == 254 && this->addr6.bytes[1] == 128;
	}

	bool IPAddressV6::IsAnyCast() const noexcept
	{
		//TODO fix
		return false;
	}

	void IPAddressV6::Clear() noexcept
	{
		memset(&this->addr6, 0x0, sizeof addr6);
	}

	std::ostream& operator<<(std::ostream& rhs, const IPAddressV6& lhs)
	{
		return rhs << lhs.GetStringRepresentation().c_str();
	}

}