#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <array>
#include <cassert>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <WS2tcpip.h>
#include "IPVersion.h"
namespace NodelNet
{
	/*
	*	<-INFO->
	*	Technically, an IPv6 address is case-insensitive, but RFC 5952, A Recommendation for IPv6 Address Text Representation,
	*	Section 4.3 Lowercase says: The characters "a", "b", "c", "d", "e", and "f" in an IPv6 address MUST be represented in lowercase
	*	Resource http://v6decode.com/
	*
	*	Unicast is one to one
	*	Multicast is ont to many
	*	Anycast is one to anyone of a group
	*/
#define MAX_IPV6_ADDRESS_SIZE_BYTES 16
#define MAX_IPV6_ADDRESS_CHAR_MAX_COUNT 39 //includes separation with colon 
	typedef std::array<uint8_t, 16> ByteArray16;
	typedef std::array<uint16_t, 8> WordArray8;
	class IPAddress;
	class IPAddressV4;
	//https://www.cisco.com/c/en/us/support/docs/ip/routing-information-protocol-rip/13788-3.html


	/* IPAddressV6 */
	class IPAddressV6
	{
		friend class IPAddressV4;
		friend class IPAddress;
		friend class IPEndPoint;
	public:
		IPAddressV6() = default;
		~IPAddressV6() = default;
		IPAddressV6(const IPAddressV6& addr6) noexcept = default;
		IPAddressV6(IPAddressV6&& addr6) noexcept = default;
		explicit IPAddressV6(const char* const ip);
		explicit IPAddressV6(const std::string& ip);
		explicit IPAddressV6(const ByteArray16& ip) noexcept { this->addr6.bytes = ip; };
		explicit IPAddressV6(const ByteArray16&& ip) noexcept { this->addr6.bytes = ip; };
		explicit IPAddressV6(const sockaddr_in6& addr6);
		explicit IPAddressV6(const in6_addr& addr6);
		//Is the same as ::1 and that can be used instead. 
		static const IPAddressV6 Loopback() { return IPAddressV6(ByteArray16({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 })); }
		static const IPAddressV6 DefaultRoute() { return IPAddressV6("::"); };
	public:
		bool operator==(const IPAddressV6& address) const noexcept;
		bool operator==(const IPAddress& address) const noexcept;

		bool operator!=(const IPAddressV6& address) const noexcept;
		bool operator!=(const IPAddress& address) const noexcept;

		IPAddressV6& operator=(const IPAddressV6& address) noexcept;
		IPAddressV6& operator=(const IPAddress& address) noexcept;

		IPAddressV6& operator=(IPAddressV6&& address) noexcept;
	protected:
		/* used when sorting IPv6 addresses */
		bool operator<(const IPAddressV6& rhs) const noexcept;
	public:
		/*
		 * Parse an IPV6 string into a IPAdddressV6 object
		 * ...
		 * @param addr6 [out] result after parsing ip string
		 * @param ip [in] string to be parsed
		 * @return true if successful
		 */
		NODISCARD static bool ParseIPAddressV6(IPAddressV6& addr6, const std::string& ip) noexcept;

		NODISCARD in6_addr GetAddress() const;
		//Specific sockaddr for IPv6
		NODISCARD sockaddr_in6& GetSockaddrIn6() const;

		NODISCARD std::string GetStringRepresentation() const;

		NODISCARD bool IsMulticast() const noexcept;

		NODISCARD bool IsGlobalUnicast() const noexcept;

		NODISCARD bool IsUniqueLocal() const noexcept;

		NODISCARD bool IsLinkLocal() const noexcept;

		NODISCARD bool IsAnyCast() const noexcept;

		NODISCARD bool IsIPv4Mapped() const noexcept;
		/* ::/128 */
		NODISCARD bool IsLoopback() const noexcept;

		NODISCARD bool InSubnet() const noexcept;

		NODISCARD bool IsMasked() const noexcept;
		/* ::/128 */
		NODISCARD bool IsUnspecified() const noexcept;
		/* Clears IPv6 address to 0 */
		void Clear() noexcept;
	public:
		friend std::ostream& operator<<(std::ostream& rhs, const IPAddressV6& lhs);
	private:
		union IPAddressStorageV6
		{
			ByteArray16 bytes;
			WordArray8 words;
			struct
			{
				union
				{
					struct
					{
						uint8_t prefix : 7;
						uint8_t localBit : 1;
						std::array<uint8_t, 5> globalId;
						uint16_t subnetId;
					};
					uint64_t val;
				}networkInterface; //64 bit
				uint64_t interfaceId; //64 bit
			} ipv6;
			in6_addr in6_address;
		} addr6 = { 0 };
	};
}