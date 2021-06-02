#pragma once
#include "IPAddressV4.h"
#include "IPAddressV6.h"
namespace NodelNet
{
	/*
		holds either ipv4 or ipv6 address with unknown address version.
	*/
	struct NetworkAddress
	{
		union {
			IPAddressV4 addr4;
			IPAddressV6 addr6 = { };
		};
	};

	/**
	*	IPAddress class containing either ipv4 or ipv6 address.
	*/
	class IPAddress
	{
		friend class IPEndPoint;
		friend class IPAddressV6;
		friend class IPAddressV4;
	public:
		IPAddress() = default;
		~IPAddress() = default;
		IPAddress(const IPAddress& addr) noexcept { *this = addr; }
		IPAddress(IPAddress&& addr) noexcept { *this = addr; }

		explicit IPAddress(const IPAddressV4& addr4) noexcept;
		explicit IPAddress(const IPAddressV6& addr6) noexcept;

		explicit IPAddress(IPAddressV4&& addr4) noexcept : version(IPVersion::IPv4) { this->addr.ipAddress4 = addr4; }
		explicit IPAddress(IPAddressV6&& addr6) noexcept : version(IPVersion::IPv6) { this->addr.ipAddress6 = addr6; }

		explicit IPAddress(const char* ip);
		explicit IPAddress(const std::string& ip);

		explicit IPAddress(const ByteArray4&& addr4) noexcept : version(IPVersion::IPv4) { addr.ipAddress4.addr.bytes = addr4; }
		explicit IPAddress(const ByteArray4& addr4) noexcept : version(IPVersion::IPv4) { addr.ipAddress4.addr.bytes = addr4; }

		explicit IPAddress(const ByteArray16&& addr6) noexcept : version(IPVersion::IPv6) { addr.ipAddress6.addr6.bytes = addr6; }
		explicit IPAddress(const ByteArray16& addr6) noexcept : version(IPVersion::IPv6) { addr.ipAddress6.addr6.bytes = addr6; }

		explicit IPAddress(const sockaddr_in& addr4);
		explicit IPAddress(const sockaddr_in6& addr6);
		explicit IPAddress(const in_addr& addr4);
		explicit IPAddress(const in6_addr& addr6);

		explicit IPAddress(const sockaddr* addr);
		IPAddress(const sockaddr* addr, socklen_t addrLength);
	public:
		//Comparison operators
		bool operator==(const IPAddress& ipAddr) const noexcept;
		bool operator==(const IPAddressV4& ipAddr4) const noexcept;
		bool operator==(const IPAddressV6& ipAddr6) const noexcept;
		//Comparison operators
		bool operator!=(const IPAddress& ipAddr) const noexcept;
		bool operator!=(const IPAddressV4& ipAddr4) const noexcept;
		bool operator!=(const IPAddressV6& ipAddr6) const noexcept;
		//Assignment operators
		IPAddress& operator=(const IPAddress& ipAddr) noexcept;
		IPAddress& operator=(const IPAddressV4& ipAddr4) noexcept;
		IPAddress& operator=(const IPAddressV6& ipAddr6) noexcept;
		//Move operators
		IPAddress& operator=(IPAddress&& ipAddr) noexcept;
		IPAddress& operator=(IPAddressV4&& ipAddr4) noexcept;
		IPAddress& operator=(IPAddressV6&& ipAddr6) noexcept;
	public:
		NODISCARD static bool ParseIPAddress(IPAddress& addr, const std::string& ip) noexcept;
		/**
		* Clears IPAddress to 0 and IPVersion to unknown
		*/
		void Clear() noexcept;
		IPVersion GetVersion() const noexcept;
		NODISCARD bool IsIPv6() const noexcept;
		NODISCARD bool IsIPv4() const noexcept;
		IPAddressV6& AsIPv6() const;
		IPAddressV4& AsIPv4() const;
		std::string GetStringRepresentation() const;
	public:
		friend std::ostream& operator<<(std::ostream& rhs, const IPAddress& lhs);
	protected:
		union IPAddressStorage
		{
			mutable IPAddressV6 ipAddress6;
			mutable IPAddressV4 ipAddress4;
		} addr = {  };
		IPVersion version = IPVersion::Unknown;
	};
}