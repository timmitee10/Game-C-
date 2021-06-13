#pragma once
#include <array>
#include "IPAddressV6.h"
#define MAX_IPV4_ADDRESS_CHAR_MAX_COUNT 15 //This includes separation with dots but without them its only 12 characters
#define MAX_IPV4_ADDRESS_SIZE_BYTES 4
namespace NodelNet
{
	template <typename T, size_t Size>
	inline std::array<T, Size> mask(const std::array<T, Size>& x, const std::array<T, Size>& y)
	{
		static_assert(Size > 0, "Can not mask empty array");
		std::array<T, Size> res = { {0} };
		const size_t size = x.size();
		for (size_t i = 0; i < size; i++)
		{
			res[i] = x[i] & y[i];
		}
		return res;
	}
	template <typename T>
	inline std::vector<T> mask(const std::vector<T>& x, const std::vector<T>& y)
	{
		static_assert(x.size() == y.size() && "Can not mask vector with different size");
		static_assert(!x.empty() > 0 && "Can not mask empty vector");
		std::vector<T> res;
		res.resize(x.size());
		const size_t size = x.size();
		for (size_t i = 0; i < size; i++)
		{
			res[i] = x[i] & y[i];
		}
		return res;
	}

	inline std::string mask(const std::string& x, const std::string& y)
	{
		assert(x.size() == y.size() && "Can not mask string with different size");
		assert(!x.empty() && "Can not mask empty string");

		std::string res;
		res.resize(x.size());
		const size_t size = x.size();
		for (size_t i = 0; i < size; i++)
		{
			res[i] = x[i] & y[i];
		}
		return res;
	}

	/*
	* Contains the types of IPv4 address classes and is defined by the first octale in an IP-address.
	*/
	enum class IPAddressV4Class
	{
		A,
		B,
		C,
		D, //Multicast
		E, //Experimental
	};
	constexpr const char* ToString(IPAddressV4Class x)
	{
		switch (x)
		{
		case IPAddressV4Class::A: return "A";
		case IPAddressV4Class::B: return "B";
		case IPAddressV4Class::C: return "C";
		case IPAddressV4Class::D: return "D";
		default: return "Unknown";
		};
	};

	typedef std::array<uint8_t, 3> ByteArray3;
	typedef std::array<uint8_t, 4> ByteArray4;
	/*
		Class representing an IPv4-address
	*/
	class IPAddressV4
	{
		friend class IPAddressV6;
		friend class IPAddress;
		friend class IPEndPoint;
	public:
		IPAddressV4() = default;
		~IPAddressV4() = default;
		IPAddressV4(const IPAddressV4& addr4) noexcept = default;
		IPAddressV4(IPAddressV4&& addr4) noexcept = default;
		explicit IPAddressV4(const char* const ip);
		explicit IPAddressV4(const std::string& ip);
		explicit IPAddressV4(const ByteArray4& ip) noexcept { this->addr.bytes = ip; };
		explicit IPAddressV4(const ByteArray4&& ip) noexcept { this->addr.bytes = ip; };
		explicit IPAddressV4(const sockaddr_in& addr);
		explicit IPAddressV4(const in_addr& addr);
		/* 127.0.0.1 */
		static const IPAddressV4 Loopback() { return IPAddressV4(ByteArray4({ 127,0,0,2 })); }
		static IPAddressV4 Any() { return IPAddressV4("0.0.0.0"); }
	public:
		bool operator==(const IPAddressV4& addr4) const noexcept;
		bool operator==(const IPAddress& addr) const noexcept;
		bool operator!=(const IPAddressV4& addr4) const noexcept;
		bool operator!=(const IPAddress& addr) const noexcept;
		IPAddressV4& operator=(const IPAddressV4& addr4) noexcept;
		IPAddressV4& operator=(const IPAddress& addr) noexcept;
		IPAddressV4& operator=(IPAddressV4&& addr4) noexcept;
	protected:
		/* used when sorting IPv4 addresses */
		bool operator<(const IPAddressV4& rhs) const noexcept;
	public:
		/*
		 * Parse an IPv4 address string into an IPAddressV4 object.
		 * ...
		 * @param addr4 [out] result after parsing ip string
		 * @param ip [in] string to be parsed
		 * @return true if it succeeded
		 */
		NODISCARD static bool ParseIPAddressV4(IPAddressV4& addr4, const std::string& ip) noexcept;
		NODISCARD in_addr GetAddress() const;
		//Specific sockaddr for IPv4
		NODISCARD sockaddr_in GetSockaddrIn4() const;
		/* Returns a string that contains each byte in an IP-address separated by a '.' e.g 192.168.1.66 */
		NODISCARD std::string GetStringRepresentation() const;
		NODISCARD size_t GetAddressSize() const;
		NODISCARD bool IsLoopback() const noexcept;
		NODISCARD bool InSubnet() const noexcept;
		/*
		* Link local is used when the host cannot find a DHCP server or are running into communication problems between the DHCP
		* meaning the host can not access the internet but can still communicate with LAN devices.
		*/
		NODISCARD bool IsLinkLocal() const noexcept;
		/*
		* Private IP addresses can not connect directly to the internet.
		* Access to the internet must be brokered by a router or other such devices that supports NAT
		*/
		NODISCARD bool IsPrivate() const noexcept;
		bool InSubnetWithMask(const IPAddressV4& addr, ByteArray4 maskAddr)
		{
			mask<uint8_t, maskAddr.size()>(addr.addr.bytes, maskAddr);
		}
		NODISCARD bool IsMasked() const;
		/* any IPv4 address at all */
		NODISCARD bool IsAny() const { return *this == IPAddressV4::Any(); };
		NODISCARD bool IsRoutableAddress() const;
		NODISCARD IPAddressV6 MapIPv4ToIPv6() const;
		/* Return the IPv4 address class type IPAddressV4Class A, B, C, D or E */
		NODISCARD IPAddressV4Class GetIPAddressClass() const;
		/* Clears IPAddress to 0 */
		void Clear() noexcept;
	public:
		friend std::ostream& operator<<(std::ostream& rhs, const IPAddressV4& lhs);
	private:
		union IPAddressStorageV4
		{
			ByteArray4 bytes;
			in_addr inAddr;
			struct
			{
				ByteArray3 NetworkAddress;
				uint8_t HostAddress;
			};
		}addr = { 0 };
	};
}
