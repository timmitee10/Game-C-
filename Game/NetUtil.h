#pragma once
#include "Config.h"
#include <cstdint>

#include <memory>

#include <locale>
#include <codecvt>
#include <stringapiset.h>
#include <cassert>
#include <WinBase.h>

namespace NodelNet
{
	typedef uint64_t connection_unique_id_t;

	/*
		Each session contains Connections and session data.
		The session can be transfered to a new host using the ServerToServerCommunication protocol.
		Each session can run on a separate thread[FUTURE]. [CURRENT] single threaded.
	*/
	typedef uint64_t session_id_t;

	template<typename T>
	inline void MemWrite(void* ptr, const T& val)
	{
		memcpy(ptr, val, sizeof(T));
	}

	//https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
	//https://stackoverflow.com/questions/105252/how-do-i-convert-between-big-endian-and-little-endian-values-in-c
	/* 8 bit-value dont need to be converted! */
	inline constexpr uint8_t ByteSwap(uint8_t x)
	{
		return x;
	}

	inline uint16_t ByteSwap(uint16_t x)
	{
#ifdef MSVC
		return _byteswap_ushort(x);
#elif OS_MAC
		return OSSwapInt16(x)
#elif OS_FREEBSD
		return swap16(x);
#else
		return bswap_16(x);
#endif
	}

	inline uint32_t ByteSwap(uint32_t x)
	{
#ifdef MSVC
		return _byteswap_ulong(x);
#elif OS_MAC
		return OSSwapInt32(x)
#elif OS_FREEBSD
		return swap32(x);
#else
		return bswap_32(x);
#endif
	}

	inline uint64_t ByteSwap(uint64_t x)
	{
#ifdef MSVC
		return _byteswap_uint64(x);
#elif OS_MAC
		return OSSwapInt64(x)
#elif OS_FREEBSD
		return swap64(x);
#else
		return bswap_64(x);
#endif
	}

	/*inline uint8_t NetToHost8(uint8_t x)
	{
		return ByteSwap(x);
	}*/

	inline uint16_t NetToHost16(uint16_t x)
	{
		return ByteSwap(x);
	}
	inline uint32_t NetToHost32(uint32_t x)
	{
		return ByteSwap(x);
	}
	inline uint64_t NetToHost64(uint64_t x)
	{
		return ByteSwap(x);
	}

	//inline uint8_t HostToNet8(uint8_t x)
	//{
	//	return ByteSwap(x);
	//}

	inline uint16_t HostToNet16(uint16_t x)
	{
		return ByteSwap(x);
	}

	inline uint32_t HostToNet32(uint32_t x)
	{
		return ByteSwap(x);
	}

	inline uint64_t HostToNet64(uint64_t x)
	{
		return ByteSwap(x);
	}

	/**
	* Safely converts a wstirng to string
	* @param [in] wstr wstring to convert.
	* @param [out] result resulting string from convertion
	* @return 0 on success
	*/
	inline int WStringToStringS(const std::wstring& wstr, std::string& result)
	{
		assert(result.size() == 0 && "Resulting string might be overwritten");
		//Check size needed
		const int size = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wstr.c_str(), wstr.size(), (char*)result.c_str(), 0, NULL, NULL);
		if (size == 0)
			return -1;
		result.resize(size);
		if (const int res = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wstr.c_str(), wstr.size(), (char*)result.c_str(), result.size(), NULL, NULL); res == 0)
		{
			auto retErr = GetLastError();
			switch (retErr)
			{
				/*All error codes from WideCharToMultiByte */
			case ERROR_INSUFFICIENT_BUFFER:
			case ERROR_INVALID_FLAGS:
			case ERROR_INVALID_PARAMETER:
			case ERROR_NO_UNICODE_TRANSLATION:
				return -1;
			default: return -1;
			}

		}
	}

	/**
	* Unsafely converts a wstirng to string
	* @param [in] wstr wstring to convert.
	* @param [out] result resulting string from convertion
	* @return string on success
	*/
	inline std::string WStringToString(const std::wstring& wstr)
	{
		std::string result;
		//Check size needed
		const int size = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wstr.c_str(), wstr.size(), (char*)result.c_str(), 0, NULL, NULL);
		result.resize(size);
		//Convert wstring to string
		WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wstr.c_str(), wstr.size(), (char*)result.c_str(), result.size(), NULL, NULL);
		return result;
	}

	/**
	* Safely converts a string to wstring
	* @param [in] str string to convert
	* @param [out] result resulting wstring from convertion
	* @return 0 on success
	*/
	inline int StringToWStringS(const std::string& str, std::wstring& result)
	{
		assert(result.size() == 0 && "Resulting wstring might be overwritten");
		//Check size needed
		const int size = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(), str.size(), const_cast<wchar_t*>(result.c_str()), 0);
		if (size == 0)
			return -1;
		result.resize(size);
		if (const int res = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(), str.size(), const_cast<wchar_t*>(result.c_str()), result.size()); res == 0)
		{
			switch (const auto retErr = GetLastError(); retErr)
			{
				/*All error codes from MultiByteToWideChar */
			case ERROR_INSUFFICIENT_BUFFER:
			case ERROR_INVALID_FLAGS:
			case ERROR_INVALID_PARAMETER:
			case ERROR_NO_UNICODE_TRANSLATION:
				return -1;
			default: return -1;
			}
		}
	}
	/**
	* Unsafely converts a string to wstring
	* @param [in] str string to convert
	* @return wstring on success.
	*/
	inline std::wstring StringToWString(const std::string& str)
	{
		std::wstring result;
		//Check size needed
		const int size = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(), str.size(), const_cast<wchar_t*>(result.c_str()), 0);
		result.resize(size);
		//Convert string to wstirng
		MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(), str.size(), const_cast<wchar_t*>(result.c_str()), result.size());
		return result;
	}
}