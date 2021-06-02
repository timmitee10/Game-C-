#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <cstdint>

//WINDOWS
#if defined(_WIN32)  //32-bit and 64-bit 
#define OS_WINDOWS 1
#include <vcruntime.h>
#elif defined(_WIN64) // 64-bit
#define OS_WINDOWS 1
#endif

//LINUX OR MAC
#if defined(unix) || defined(__unix__) || defined(__unix)
#define OS_UNIX_SYSTEM 1
#elif defined (__linux__)
#define OS_LINUX 1
#elif defined(__APPLE__) || defined(__MACH__)
#define OS_MAC 1
#elif defined(__FreeBSD__)
#define OS_FREEBSD 1
#endif

//COMPILE INFO
#if (_MSC_VER)
#define MSVC
#elif (__GNUC__)
#define GCC
#elif (__clang__)
#define CLANG
#elif (__EMSCRIPTEN__)
#error EMSCRIPTEN compiler not supported
#define EMSCRIPTEN
#elif (__MINGW32__)
#error MinGW compiler not supported
#elif(__MINGW32__)
#error MinGW compiler not supported
#elif(__MINGW64__)
#error MinGW compiler not supported
#endif

//TO DO: Should be defined by CMake.

/* BUILD DLL */
#define NODEL_BUILD_DLL 1
/* CONSUME DLL */
#define NODEL_CONSUME_DLL 1

#if defined(_WIN32) && defined(NODEL_BUILD_DLL)
	//Build NodelNet as a DLL.
#define NODEL_API __declspec(dllexport)
#elif defined(_WIN32) && defined(NODEL_DLL)
	//Use NodelNet as a DLL.
#define NODEL_API __declspec(dllimport)
#elif defined(__GNUC__) && defined(NODEL_BUILD_DLL)
	//Building NodelNet as a shared library
#define NODEL_API __attribute__((visibility("default")))
#else
#define NODEL_API
#endif

#ifdef _NODISCARD
#define NODISCARD [[nodiscard]]
#endif


#if defined NODEL_SERVER && !defined NODEL_CLIENT
#define NODEL_SERVER_ONLY 1
#endif

#if defined NODEL_CLIENT && !defined NODEL_SERVER
#define NODEL_CLIENT_ONLY 1
#endif

#ifdef NODEL_SERVER_ONLY
#define NODEL_SERVER_OR_CLIENT_ONLY 1
#elif defined NODEL_CLIENT_ONLY
#define NODEL_SERVER_OR_CLIENT_ONLY 1
#endif


#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
#define OBJECT_TYPE(x) (static_cast<decltype(this) > (x)>) 
/*
 * Return 0 or 1 depending on wheter the bit is enabled
 *@param var variable
 *@param pos starts counting from 0
 */
#define CHECK_BIT(var,pos) (((var)>>(pos)) & 1)

#ifdef OS_WINDOS
typedef DWORD system_error_t
#elif defined OS_LINUX
#include <errno.h>
typedef errno system_error_t
#endif

/*https://stackoverflow.com/questions/2324658/how-to-determine-the-version-of-the-c-standard-used-by-the-compiler
 * C++ pre-C++98: __cplusplus is 1.
 * C++98: __cplusplus is 199711L.
 * C++98 + TR1: This reads as C++98 and there is no way to check that I know of.
 * C++11: __cplusplus is 201103L.
 * C++14: __cplusplus is 201402L.
 * C++17: __cplusplus is 201703L.
 * C++20: Undefined but greater than 201703L
 */

#if __cplusplus == 1 //Before C++98
#define CONSTEXPR const
#endif

#if __cplusplus 199711L //C++98
#define CONSTEXPR const
#endif

#if __cplusplus == 201103L //C++11
#define CONSTEXPR constexpr
#endif

#if __cplusplus == 201402L //C++14
#define CONSTEXPR constexpr
#endif

#if __cplusplus == 201703L //C++17
#define CONSTEXPR constexpr
#endif 

#ifdef OS_LINUX
#include <unistd.h>
	typedef pid_t processId_t;
typedef GetLastError() errno
#elif defined OS_WINDOWS
 //#include <processthreadsapi.h>
	typedef DWORD processId_t;
#endif

inline processId_t NGetCurrentProcessId()
{
#ifdef OS_WINDOWS
	return GetCurrentProcessId();
#elif defined OS_LINUX
	return getpid();
#endif
}
typedef SOCKET SocketHandle;



 //#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || \
 //    defined(__BIG_ENDIAN__) || \
 //    defined(__ARMEB__) || \
 //    defined(__THUMBEB__) || \
 //    defined(__AARCH64EB__) || \
 //    defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
 //#define BIG_ENDIAN
 //// It's a big-endian target architecture
 //#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || \
 //    defined(__LITTLE_ENDIAN__) || \
 //    defined(__ARMEL__) || \
 //    defined(__THUMBEL__) || \
 //    defined(__AARCH64EL__) || \
 //    defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)
 //#define LITTLE_ENDIAN
 //#else
 //#error "Can not determen edian!"
 //#endif

 //
 ////https://codereview.stackexchange.com/questions/226631/overload-c-enum-class-operators-and-enable-check-value-set
 //#define OVERLOAD_ENUM_OPERATORS(x) \
 //class EnumBool##x \
 //{ \
 //    x enum_value; \
 //public: \
 //    constexpr EnumBool##x(const x e_val) : enum_value(e_val) {} \
 //    constexpr operator x() const { return enum_value; } \
 //    explicit operator bool() const { return (std::underlying_type_t<x>)enum_value != 0; } \
 //}; \
 //inline constexpr EnumBool##x operator&(const x lhs, const x rhs) \
 //{ \
 //    return EnumBool##x((x)((std::underlying_type_t<x>)lhs & (std::underlying_type_t<x>)rhs)); \
 //} \
 //inline constexpr x operator|(const x lhs, const x rhs) \
 //{ \
 //    return (x)((std::underlying_type_t<x>)lhs | (std::underlying_type_t<x>)rhs); \
 //} \
 //inline constexpr x operator^(const x lhs, const x rhs) \
 //{ \
 //    return (x)((std::underlying_type_t<x>)lhs ^ (std::underlying_type_t<x>)rhs);\
 //} \
 //inline constexpr x operator~(const x lhs) \
 //{ \
 //    return (x)(~(std::underlying_type_t<x>)lhs);\
 //} \
 //inline constexpr x& operator|=(x& lhs, const x rhs) \
 //{ \
 //    lhs = (x)((std::underlying_type_t<x>)lhs | (std::underlying_type_t<x>)rhs); \
 //    return lhs; \
 //} \
 //inline constexpr x& operator&=(x& lhs, const x rhs) \
 //{ \
 //    lhs = (x)((std::underlying_type_t<x>)lhs & (std::underlying_type_t<x>)rhs); \
 //    return lhs; \
 //} \
 //inline constexpr x& operator^=(x& lhs, const x rhs) \
 //{ \
 //    lhs = (x)((std::underlying_type_t<x>)lhs ^ (std::underlying_type_t<x>)rhs); \
 //    return lhs; \
 //} \
 //inline constexpr bool operator==(const x lhs, const x rhs) \
 //{ \
 //    return (std::underlying_type_t<x>)lhs == (std::underlying_type_t<x>)rhs; \
 //} \
 //inline constexpr bool operator!=(const x lhs, const x rhs) \
 //{ \
 //    return (std::underlying_type_t<x>)lhs != (std::underlying_type_t<x>)rhs; \
 //} \
 //inline constexpr bool operator>(const x lhs, const x rhs) \
 //{ \
 //    return (std::underlying_type_t<x>)lhs > (std::underlying_type_t<x>)rhs; \
 //} \
 //inline constexpr bool operator<(const x lhs, const x rhs) \
 //{ \
 //    return (std::underlying_type_t<x>)lhs < (std::underlying_type_t<x>)rhs; \
 //} \
 //inline constexpr bool operator>=(const x lhs, const x rhs) \
 //{ \
 //    return (std::underlying_type_t<x>)lhs >= (std::underlying_type_t<x>)rhs; \
 //} \
 //inline constexpr bool operator<=(const x lhs, const x rhs) \
 //{ \
 //    return (std::underlying_type_t<x>)lhs <= (std::underlying_type_t<x>)rhs; \
 //}