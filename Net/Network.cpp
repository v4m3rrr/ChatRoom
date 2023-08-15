#define WIN32_LEAN_AND_MEAN

#include "Network.h"
#include "NetException.h"

#include <winsock2.h>

namespace Net
{
	void Network::Initialize()
	{
		int result;

		WSAData wsaData = {};
		if ((result = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
		{
			THROW_NET_WINSOCK_EXCEPTION(result);
		}
	}

	void Network::Deinitialize() 
	{
		WSACleanup();
	}
}

