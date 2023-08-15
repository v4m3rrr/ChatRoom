#pragma once
#include "IPVersion.h"

#include <WS2tcpip.h>

#include <string>
#include <vector>

namespace Net
{
	class IPEndpoint
	{
	public:
		IPEndpoint(const std::string& ip, unsigned short port);
		IPEndpoint(sockaddr* addr);

		IPVersion GetIpVersion() const;
		std::string GetHostname() const;
		std::string GetIPString() const;
		std::vector<uint8_t> GetIPBytes() const;
		unsigned short GetPort() const;
		sockaddr_in GetIPv4SockAddr() const;
	private:
		IPVersion ipVersion;
		std::string hostname;
		std::string ip_string;
		std::vector<uint8_t> ip_bytes;
		unsigned short port;
	};
}