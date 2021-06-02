#pragma once
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "IPAddress.h"
#include "NETResult.h"
#include "SocketAPI.h"
#include "Constants.h"
#include "NetUtil.h"
//#include "IPAddressStruct.h"
//The primary form : The preferred form is x : x:x:x:x:x:x:x, 
//where the "x"s are the hexadecimal value of the eight 16 - bit pieces of the address.Two examples :
//  fe80 : 0 : 0 : 0 : 207 : 30ee : edcb : d05d
//  1080 : 0 : 0 : 0 : 1 : 700 : 200B : 417C
namespace NodelNet
{
	typedef uint16_t port_t;
	typedef port_t port_host_byte_order_t;

	//different underlying type to allow unique operator overloading for port_network_byte_order_t and port_host_byte_order!
	enum class _port_network_byte_order_t : uint16_t;

	typedef _port_network_byte_order_t port_network_byte_order_t;
	/* convert port_t to port_network_byte_order_t */
	inline NODISCARD port_network_byte_order_t to_network_byte_order(const port_host_byte_order_t arg) noexcept {
		return static_cast<port_network_byte_order_t>(HostToNet16(arg));
	}
	/* convert _port16_t to port_host_byte_order_t */
	inline NODISCARD port_host_byte_order_t to_host_byte_order(const _port_network_byte_order_t arg) noexcept {
		return static_cast<port_host_byte_order_t>(NetToHost16(static_cast<uint16_t>(arg)));
	}
	/* convert _port16_t to integer, should be avoided as it can now be interpreted as port_t and port_host_byte_order_t */
	inline NODISCARD constexpr uint16_t to_integer(const _port_network_byte_order_t arg) noexcept {
		return static_cast<uint16_t>(arg);
	}
	/* IPEndPoint an IPAddress and a port number stored in host byte order. */
	class IPEndPoint final : public IPAddress
	{
		friend class IPAddress;
	public:
		IPEndPoint() = default;
		~IPEndPoint() = default;
		IPEndPoint(const IPEndPoint& ipEnd) = default;
		IPEndPoint(IPEndPoint&& ipEnd) = default;

		explicit IPEndPoint(const char* const ip) : IPAddress(ip) {}
		explicit IPEndPoint(const std::string& ip) : IPAddress(ip) {}
		IPEndPoint(const std::string& ip, const port_host_byte_order_t port) : IPAddress(ip), port(port) {}
		IPEndPoint(const std::string& ip, const port_network_byte_order_t port) : IPAddress(ip), port(to_host_byte_order(port)) {}

		explicit IPEndPoint(const ByteArray4& addr4, const port_host_byte_order_t port) noexcept : IPAddress(addr4), port(port) {}
		explicit IPEndPoint(const ByteArray4&& addr4, const port_host_byte_order_t port) noexcept : IPAddress(addr4), port(port) {}
		explicit IPEndPoint(const ByteArray16& addr6, const port_host_byte_order_t port) noexcept : IPAddress(addr6), port(port) {}
		explicit IPEndPoint(const ByteArray16&& addr6, const port_host_byte_order_t port) noexcept : IPAddress(addr6), port(port) {}

		explicit IPEndPoint(const ByteArray4& addr4, const port_network_byte_order_t port) noexcept : IPAddress(addr4), port(to_host_byte_order(port)) {}
		explicit IPEndPoint(const ByteArray4&& addr4, const port_network_byte_order_t port) noexcept : IPAddress(addr4), port(to_host_byte_order(port)) {}
		explicit IPEndPoint(const ByteArray16& addr6, const port_network_byte_order_t port) noexcept : IPAddress(addr6), port(to_host_byte_order(port)) {}
		explicit IPEndPoint(const ByteArray16&& addr6, const port_network_byte_order_t port) noexcept : IPAddress(addr6), port(to_host_byte_order(port)) {}

		explicit IPEndPoint(const ByteArray4& addr4) noexcept : IPAddress(addr4) {}
		explicit IPEndPoint(const ByteArray16& addr6) noexcept : IPAddress(addr6) {}

		explicit IPEndPoint(const sockaddr_in& addr4) : IPAddress(addr4), port(NetToHost16(addr4.sin_port)) {}
		explicit IPEndPoint(const sockaddr_in6& addr6) : IPAddress(addr6), port(NetToHost16(addr6.sin6_port)) {}

		IPEndPoint(const IPAddressV4& addr4, const port_host_byte_order_t port) noexcept : IPAddress(addr4), port(port) {}
		IPEndPoint(const IPAddressV6& addr6, const port_host_byte_order_t port) noexcept : IPAddress(addr6), port(port) {}
		IPEndPoint(const IPAddress& addr, const port_host_byte_order_t port) : IPAddress(addr), port(port) {}

		IPEndPoint(const IPAddressV4& addr4, const port_network_byte_order_t port) noexcept : IPAddress(addr4), port(to_host_byte_order(port)) {}
		IPEndPoint(const IPAddressV6& addr6, const port_network_byte_order_t port) noexcept : IPAddress(addr6), port(to_host_byte_order(port)) {}
		IPEndPoint(const IPAddress& addr, const port_network_byte_order_t port) : IPAddress(addr), port(to_host_byte_order(port)) {}

		explicit IPEndPoint(const IPAddressV4& addr4) noexcept : IPAddress(addr4) {}
		explicit IPEndPoint(const IPAddressV6& addr6) noexcept : IPAddress(addr6) {}
		explicit IPEndPoint(const IPAddress& addr) : IPAddress(addr) {}

		explicit IPEndPoint(const sockaddr* addr) : IPAddress(addr) {};
		explicit IPEndPoint(const sockaddr* addr, socklen_t addrLength);
	public:

		bool operator==(const IPEndPoint& rhs) const;
		bool operator==(const port_host_byte_order_t rhs) const;
		bool operator==(const port_network_byte_order_t rhs) const;

		bool operator!=(const IPEndPoint& rhs) const;
		bool operator!=(const port_host_byte_order_t rhs) const;
		bool operator!=(const port_network_byte_order_t rhs) const;

		IPEndPoint& operator=(const IPEndPoint& rhs);
		IPEndPoint& operator=(const port_host_byte_order_t& rhs);
		IPEndPoint& operator=(const port_network_byte_order_t& rhs);

		IPEndPoint& operator=(IPEndPoint&& rhs) noexcept;
	protected:
		//Used for sorting with IPEndPoints.
		bool operator<(const IPEndPoint& lhs) const;
	public:
		/* Port in host-byte order */
		port_host_byte_order_t GetPort() const;
		/* Port in network byte order */
		port_network_byte_order_t GetPortNetworkByteOrder() const;
		//Slices the class into a IPAddress
		IPAddress GetIPAddress() const;
		sockaddr_in GetAddressIPv4() const;
		sockaddr_in6 GetAddressIPv6() const;
		//FORMAT: x.x.x.x:port
		std::string GetStringRepresentationWithPort() const;
		/**
		* Clears IPAddress to 0x0, IPVersion to Unknown and sets port to DEFAULT_IP_ENDPOINT_PORT (default values)
		*/
		void Clear() noexcept;
	public:
		//Use GetStringRepresentationWithPort format
		friend std::ostream& operator<<(std::ostream& rhs, const IPEndPoint& lhs);
	private:
		port_host_byte_order_t port = DEFAULT_IP_ENDPOINT_PORT;
	};
}