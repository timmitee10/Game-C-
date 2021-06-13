#include "IPAddressV4.h"
#include "IPAddress.h"
namespace NodelNet
{
	IPAddressV4::IPAddressV4(const char* const ip)
	{
		if (!ParseIPAddressV4(*this, ip))
			throw std::exception("InvalidParserInput");
	}

	IPAddressV4::IPAddressV4(const std::string& ip)
	{
		if (!ParseIPAddressV4(*this, ip))
			throw std::exception("InvalidParserInput");
	}

	IPAddressV4::IPAddressV4(const sockaddr_in& addr)
	{
		assert(addr.sin_family == AF_INET);
		memcpy(static_cast<void*>(&this->addr), &addr.sin_addr, sizeof(addr));
	}

	IPAddressV4::IPAddressV4(const in_addr& addr)
	{
		this->addr.inAddr = addr;
	}

	bool IPAddressV4::operator==(const IPAddressV4& addr4) const noexcept
	{
		return this->addr.bytes == addr4.addr.bytes;
	}

	bool IPAddressV4::operator==(const IPAddress& addr) const noexcept
	{
		if (addr.IsIPv4())
		{
			return this->addr.bytes == addr.addr.ipAddress4.addr.bytes;
		}
		else
		{
			return false;
		}
	}

	bool IPAddressV4::operator!=(const IPAddressV4& addr4) const noexcept
	{
		return !(this->operator==(addr4));
	}

	bool IPAddressV4::operator!=(const IPAddress& addr) const noexcept
	{
		return !(this->operator==(addr));
	}

	IPAddressV4& IPAddressV4::operator=(const IPAddressV4& addr4) noexcept
	{
		this->addr = addr4.addr;
		return *this;
	}

	IPAddressV4& IPAddressV4::operator=(const IPAddress& addr) noexcept
	{
		*this = addr.addr.ipAddress4;
		return *this;
	}

	IPAddressV4& IPAddressV4::operator=(IPAddressV4&& addr4) noexcept
	{
		*this = addr4;
		return *this;
	}

	bool IPAddressV4::operator<(const IPAddressV4& rhs) const noexcept
	{
		return this->addr.bytes < rhs.addr.bytes;
	}


	bool IPAddressV4::ParseIPAddressV4(IPAddressV4& addr4, const std::string& ip) noexcept
	{
		assert(ip.size() <= MAX_IPV4_ADDRESS_CHAR_MAX_COUNT && ip.empty() == false);

		//IPv4 4-bytes address(A ULONG 4bytes)
		in_addr  inAddr;
		//Convert ip characters to address in bytes
		int result = inet_pton(AF_INET, ip.c_str(), &inAddr);
		if (result == 1)
		{
			memcpy(&addr4.addr.bytes[0], &inAddr.S_un.S_addr, sizeof(ULONG));
			return true;
		}

		//hostname IPv4
		addrinfo hints = { 0 };
		hints.ai_family = AF_INET; // IPv4 addresses only

		addrinfo* hostinfo = nullptr;


		result = getaddrinfo(ip.c_str(), NULL, &hints, &hostinfo);
		if (result == 0) //Is a hostname
		{
			sockaddr_in* host_addr = reinterpret_cast<sockaddr_in*> (hostinfo->ai_addr);
			memcpy(&addr4.addr.bytes[0], &host_addr->sin_addr.S_un.S_addr, sizeof(ULONG));

			freeaddrinfo(hostinfo);
			return true;
		}
		freeaddrinfo(hostinfo);
		return false;
	}

	in_addr IPAddressV4::GetAddress() const
	{
		return this->addr.inAddr;
	}

	sockaddr_in IPAddressV4::GetSockaddrIn4() const
	{
		sockaddr_in addrIn = {};
		memcpy(&addrIn.sin_addr, &addr, sizeof(addr));
		addrIn.sin_family = AF_INET;
		return addrIn;
	}

	std::string IPAddressV4::GetStringRepresentation() const
	{
		std::stringstream ss;
		int index = 0;
		for (const auto& i : addr.bytes)
		{
			ss << std::to_string(i);
			if (index < 3)
				ss << '.';
			index++;
		}
		return ss.str();
		//return std::string(addr.bytes.begin(), addr.bytes.end());
	}
	size_t IPAddressV4::GetAddressSize() const
	{
		return addr.bytes.size();
	}

	//"The loopback address is guaranteed to be 127.x.x.x(any will work, 0.0.1 is standard) on any IPv4 machine and ::1 on any IPv6 machine."
	bool IPAddressV4::IsLoopback() const noexcept
	{
		//127.0.0.0 to 127.255.255.255 reserved for loopback addresses according to https://en.wikipedia.org/wiki/Reserved_IP_addresses
		//It is therefore only neccesery to check if it starts with 127.x.x.x.
		return addr.inAddr.S_un.S_un_b.s_b1 == static_cast<UCHAR>(127);
	}
	//https://support.microsoft.com/en-us/help/164015/understanding-tcp-ip-addressing-and-subnetting-basics
	bool IPAddressV4::InSubnet() const noexcept
	{
		static IPAddressV4 loopBackAddress("127.0.0.0");
		//169.254.0.0�169.254.255.255 reserved for Subnet according to https://en.wikipedia.org/wiki/Reserved_IP_addresses
		return addr.inAddr.S_un.S_un_b.s_b1 == static_cast<UCHAR>(169) && addr.inAddr.S_un.S_un_b.s_b2 == static_cast<UCHAR>(254);
	}

	bool IPAddressV4::IsLinkLocal() const noexcept
	{
		/* 169.254.0.0 - 169.254.255.255 or 169.254.0.0/16 */
		return addr.bytes[0] == 169 && addr.bytes[1] == 254;
	}

	bool IPAddressV4::IsPrivate() const noexcept
	{
		/*
		* Private Addresses
		* Class A: 10.0.0.0 - 10.255.255.255
		* Class B: 172.16.0.0 - 172.31.255.255
		* Class C: 192.168.0.0 - 192.168.255.255
		*/
		return (addr.bytes[0] == 10) ||
			(addr.bytes[0] == 172 && addr.bytes[1] >= 16 && addr.bytes[1] <= 31) ||
			(addr.bytes[0] == 192 && addr.bytes[1] == 168);
	}

	bool IPAddressV4::IsRoutableAddress() const
	{
		//TODO implement
		return false;
	}

	//The IPv4 to IPv6 formant is specified in rfc4291
	//The format for IPv4-mapped IPv6 addresses is as followed 
	/*
		|                80 bits               | 16 |      32 bits        |
		+--------------------------------------+--------------------------+
		|0000..............................0000|FFFF|    IPv4 address     |
		+--------------------------------------+----+---------------------+
						 word[5]
	*/
	IPAddressV6 IPAddressV4::MapIPv4ToIPv6() const
	{
		IPAddressV6 ipv6;
		memcpy(&ipv6.addr6.in6_address.u.Word[6], &this->addr.bytes, sizeof(addr));
		memset(&ipv6.addr6.in6_address.u.Word[5], 0xff, sizeof(USHORT));
		return ipv6;
	}

	IPAddressV4Class IPAddressV4::GetIPAddressClass() const
	{
		/*
		*  https://www.youtube.com/watch?v=vcArZIAmnYQ&list=PLSNNzog5eydt_plAtt3k_LYuIXrAS4aDZ&ab_channel=SunnyClassroom
		* X is any combination!
		*	Class A   0 - 127  0XXXXXXX
		*	Class B 128 - 191  10XXXXXX
		*	Class C 192 - 223  110XXXXX
		*	Class D 224 - 239  1110XXXX
		*	Class E 240 - 255  1111XXXX
		* Note* that even 127 is class A, even though it's loopback.
		*/
		if (CHECK_BIT(this->addr.bytes[0], 7))
			return IPAddressV4Class::B;
		if (CHECK_BIT(this->addr.bytes[0], 6))
			return IPAddressV4Class::C;
		if (CHECK_BIT(this->addr.bytes[0], 5))
			return IPAddressV4Class::D;
		if (CHECK_BIT(this->addr.bytes[0], 4))
			return IPAddressV4Class::E;
		return IPAddressV4Class::A;
	}

	void IPAddressV4::Clear() noexcept
	{
		memset(&this->addr, 0x0, sizeof(this->addr));
	}

	std::ostream& operator<<(std::ostream& rhs, const IPAddressV4& lhs)
	{
		return rhs << lhs.GetStringRepresentation();
	}

}