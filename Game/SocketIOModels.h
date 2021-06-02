#pragma once
namespace NodelNet
{
	//RE-DEFINE USING BETTER NAMING
	enum class SocketIOModels
	{
		Unknown,
		Blocking, //BLOCKING
		Select, //BOTH BLOCKING AND NON-BLOCKING
		Polling, //NON-BLOCKING
		OverlappedIO // N/A (Only Windows)
	};
}