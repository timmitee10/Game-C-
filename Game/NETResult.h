#pragma once
#include "Config.h"
#include <string>
namespace NodelNet
{
	enum class NODISCARD NETResult : uint32_t
	{
		Success = 0,
		/* unknown error */
		GenericError = 1,
		Overflow,
		/* when connection could not be established */
		CouldNotEstablishConnection,
		/* when connection can not be reached anymore after connection has already been established */
		CanNotReach,
		/* IPVerssion did not match or was not supported */
		ConnectionIPVersionMismatch,
		/* Successful Disconnect */
		Disconnect,
		InvalidRespons,
		InvalidRequest,
		NotImplemented,
		WindowsError,
		LinuxError,
		VersionMissmatch,
	};
	constexpr const char* ToString(NETResult x)
	{
		switch (x) {
		case NETResult::Success: return "Success";
		case NETResult::GenericError: return "Generic Error";
		case NETResult::Overflow: return "Overflow";
		case NETResult::CouldNotEstablishConnection: return "Could not establish connection";
		case NETResult::CanNotReach: return "Can Not Reach";
		case NETResult::ConnectionIPVersionMismatch: return "ConnectionIPVersionMismatch";
		case NETResult::Disconnect: return "Disconnect";
		case NETResult::InvalidRespons: return "Invalid Respons";
		case NETResult::InvalidRequest: return "Invalid Request";
		case NETResult::NotImplemented: return "Not Implemented";
		case NETResult::WindowsError: return "Windows Error";
		case NETResult::LinuxError: return "Linux Error";
		case NETResult::VersionMissmatch: return "Version Missmatch";
		default: return "Unknown";
		}
	}
}