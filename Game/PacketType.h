#pragma once
namespace NodelNet
{
	enum PacketType : uint16_t
	{
		PT_Invalid,
		PT_Move,
		PT_Create,
	};
}