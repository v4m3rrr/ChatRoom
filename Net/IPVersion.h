#pragma once

#include <winsock2.h>

namespace Net
{
	enum class IPVersion
	{
		IPv4 = AF_INET,
		IPv6 = AF_INET6
	};
}