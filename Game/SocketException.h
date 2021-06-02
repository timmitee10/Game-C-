#pragma once
#include <exception>
#include <string>
#include "SocketAPI.h"
#include <WinBase.h>

namespace NodelNet
{
#define NODEL_NET_EXCEPTION 1

#if NODEL_NET_EXCEPTION  == 1

#define NETCHECK_ARG(argument) \
	{ \
		int errorCode = 0; \
		errorCode = WSAGetLastError(); \
		throw SocketExeption(errorCode, __LINE__, __FILE__, __func__,(const char*)argument);\
	}

#define NETCHECK() { \
	int errorCode = 0; \
	errorCode = WSAGetLastError(); \
	if (errorCode != 0) \
	{ \
		throw SocketExeption(errorCode,__LINE__,__FILE__, __func__, "");\
	} \
	}
#endif
	//The return value will be a system error code(winerror.h)
#define POLL_ERR(SocketDescriptor,errorCode) \
{ \
	int errorCodeLength = sizeof((int)errorCode); \
	DWORD POLL_ERROR_CODE = getsockopt((SOCKET)SocketDescriptor, SOL_SOCKET, SO_ERROR, (char*)&errorCode, &errorCodeLength); \
}
	class SocketExeption : public std::exception
	{

	public:
		SocketExeption(int errorCode, int line, const char* file, const char* functionName, const char* argument) : errorCode(errorCode)
		{
#ifdef _WIN32
			std::string errorMessage;
			if (errorCode != 0)
			{
				LPSTR tempErrorMessage = NULL;
				//TOO DO: FIX
				size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, errorCode,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPSTR)&tempErrorMessage, 0, NULL);
				//f
				(stderr, "%S\n", errorMessage);
				errorMessage = std::string(tempErrorMessage, size);
				LocalFree(tempErrorMessage);
			}

#elif OS_UNIX_SYSTEM
			char* errorMessage;
			errorMessage = strerror(errorCode);
#endif

			errorStr.append("Network library error has occurred. Argument: ");
			errorStr.append(argument);

			errorStr.append(" Line:" + std::to_string(line) +
				" File: " + file +
				" Function: " + functionName + "()");
			if (errorCode != 0)
			{
				errorStr.append(" Error message: ");
				errorStr.append(errorMessage);
			}
			fprintf(stderr, "text %s", errorStr.c_str());
			//TOO DO: FIX
			//MessageBox(NULL, (LPCSTR)errorStr.c_str(), (LPCSTR)L"Network library error", MB_OK);
		}

	public:
		std::string GetError()
		{
			return errorStr;
		}
	private:
		wchar_t* CharToWChar(const char* ptr)
		{
			const size_t newsize = strlen(ptr) + 1;
			auto* wcstring = new wchar_t[newsize];
			size_t convertedChars = 0;
			mbstowcs_s(&convertedChars, wcstring, newsize, ptr, _TRUNCATE);
			return wcstring;
		}
		std::string errorStr;
		int errorCode;
	};

}