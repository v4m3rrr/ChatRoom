#pragma once

#include <winsock2.h>

namespace Net
{
	enum class Protocol
	{
		TCP = IPPROTO_TCP,
		UDP = IPPROTO_UDP
	};
}