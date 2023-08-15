#include "IPEndpoint.h"

#include "NetException.h"

#include <assert.h>

namespace Net
{
	IPEndpoint::IPEndpoint(const std::string& ip, unsigned short port)
		:port(port)
	{
		IN_ADDR addr;
		if (inet_pton(AF_INET, ip.data(), &addr) == 1)
		{
			hostname = ip;
			ip_string = ip;
			ipVersion = IPVersion::IPv4;

			ip_bytes.resize(sizeof(ULONG));
			memcpy(&ip_bytes[0], &addr.s_addr,sizeof(ULONG));

			return;
		}

		//If its not doted ipv4 string try if it is hostname
		addrinfo hints = {};
		hints.ai_family = AF_INET;
		addrinfo* pResult = nullptr;
		if (getaddrinfo(ip.data(), NULL, &hints, &pResult) == 0)
		{
			in_addr hostAddr = reinterpret_cast<sockaddr_in*>(pResult->ai_addr)->sin_addr;
			hostname = ip;

			ip_string.resize(INET_ADDRSTRLEN);
			inet_ntop(AF_INET, &hostAddr, &ip_string[0], INET_ADDRSTRLEN);

			ip_bytes.resize(sizeof(ULONG));
			memcpy(&ip_bytes[0], &hostAddr.s_addr, sizeof(ULONG));

			ipVersion = IPVersion::IPv4;

			freeaddrinfo(pResult);
			return;
		}

		THROW_NET_FORMAT_IPADDRESS_EXCEPTION();
	}
	IPEndpoint::IPEndpoint(sockaddr* addr)
	{
		assert(addr->sa_family == (int)IPVersion::IPv4);

		sockaddr_in* addrv4 = (sockaddr_in*)addr;
		ipVersion = (IPVersion)addrv4->sin_family;
		port = ntohs(addrv4->sin_port);

		ip_string.resize(INET_ADDRSTRLEN);
		inet_ntop((int)ipVersion, &addrv4->sin_addr, &ip_string[0], INET_ADDRSTRLEN);

		ip_bytes.resize(sizeof(ULONG));
		memcpy(&ip_bytes[0], &addrv4->sin_addr.s_addr, sizeof(ULONG));

		//Try to retrive hostname if exists
		char hostBuf[NI_MAXHOST];
		if (getnameinfo(reinterpret_cast<sockaddr*>(addrv4), sizeof(sockaddr_in), hostBuf, sizeof(hostBuf), NULL, 0, 0) != 0)
		{
			//If failed
			WinsockException exception(WSAGetLastError(), __LINE__, __FILE__);
			OutputDebugStringA(exception.what());
			hostname = ip_string;
		}
		else
		{
			hostname = hostBuf;
		}
		
	}
	IPVersion Net::IPEndpoint::GetIpVersion() const
	{
		return ipVersion;
	}

	std::string IPEndpoint::GetHostname() const
	{
		return hostname;
	}

	std::string IPEndpoint::GetIPString() const
	{
		return ip_string;
	}

	std::vector<uint8_t> IPEndpoint::GetIPBytes() const
	{
		return ip_bytes;
	}

	unsigned short IPEndpoint::GetPort() const
	{
		return port;
	}

	sockaddr_in IPEndpoint::GetIPv4SockAddr() const
	{
		assert(ipVersion == IPVersion::IPv4);

		sockaddr_in addr = {};
		addr.sin_family = (int)ipVersion;
		addr.sin_port = htons(port);
		memcpy(&addr.sin_addr, ip_bytes.data(), ip_bytes.size());

		return addr;
	}

}
