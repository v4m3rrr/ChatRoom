#include "Socket.h"

#include "NetException.h"

#include <assert.h>

namespace Net
{
	Net::Socket::Socket()
		:Socket(IPVersion::IPv4, Protocol::TCP)
	{
	}

	Net::Socket::Socket(IPVersion ipVersion, Protocol protocol)
		:ipVersion(ipVersion), protocol(protocol)
	{
		assert(ipVersion == IPVersion::IPv4);
		assert(protocol == Protocol::TCP);

		if ((handle = socket((int)ipVersion, (int)GetType(), (int)protocol)) == INVALID_SOCKET)
		{
			THROW_NET_LAST_WINSOCK_EXCEPTION();
		}

		// Dont fail when option is not possible
		// Let user change this option and resolve issue
		try
		{
			SetSocketOption(Option::TCP_NoDelay, true);
		}
		catch(const Exception& e)
		{
			OutputDebugStringA(e.what());
		}
	}

	Socket::Socket(IPVersion ipVersion, Protocol protocol, SocketHandle handle)
		:ipVersion(ipVersion), protocol(protocol), handle(handle)
	{
	}

	Socket::Socket(Socket&& other) noexcept
		:handle(std::move(other.handle)),
		ipVersion(std::move(other.ipVersion)),
		protocol(std::move(other.protocol))
	{
		other.handle = INVALID_SOCKET;
	}

	Socket& Socket::operator=(Socket&& other) noexcept
	{
		if (handle != INVALID_SOCKET)
			closesocket(handle);

		handle = std::move(other.handle);
		ipVersion = std::move(other.ipVersion);
		protocol = std::move(other.protocol);

		other.handle = INVALID_SOCKET;

		return *this;
	}

	void Socket::Bind(const IPEndpoint& ipEndpoint)
	{
		assert(ipVersion == ipEndpoint.GetIpVersion());
		assert(ipVersion == IPVersion::IPv4);

		sockaddr_in sockAddr = ipEndpoint.GetIPv4SockAddr();

		if (bind(handle, (sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
		{
			THROW_NET_LAST_WINSOCK_EXCEPTION();
		}
	}

	void Socket::Listen(const IPEndpoint& ipEndpoint, int backlog)
	{
		Bind(ipEndpoint);

		if (listen(handle, backlog) == SOCKET_ERROR)
		{
			THROW_NET_LAST_WINSOCK_EXCEPTION();
		}
	}

	std::pair<Socket,IPEndpoint> Socket::Accept()
	{
		sockaddr_in sockAddr = {};
		int len = sizeof(sockAddr);

		SocketHandle connHandle = accept(handle, (sockaddr*)&sockAddr, &len);
		if (connHandle==INVALID_SOCKET)
		{
			THROW_NET_LAST_WINSOCK_EXCEPTION();
		}

		return std::make_pair(Socket(IPVersion::IPv4, protocol, connHandle), IPEndpoint((sockaddr*)&sockAddr));
	}

	void Socket::Connect(const IPEndpoint& ipEndpoint)
	{
		sockaddr_in sockAddr = ipEndpoint.GetIPv4SockAddr();
		if (connect(handle, (sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
		{
			THROW_NET_LAST_WINSOCK_EXCEPTION();
		}
	}

	int Socket::Send(const std::string& msg)
	{
		int numBytes = send(handle, msg.data(), msg.size() > MSG_BUF ? MSG_BUF : msg.size(), NULL);
		if (numBytes == SOCKET_ERROR)
		{
			THROW_NET_LAST_WINSOCK_EXCEPTION();
		}

		return numBytes;
	}

	void Socket::SendAll(const std::string& msg)
	{
		int sentBytes = 0;
		while (sentBytes < msg.size())
		{
			sentBytes += Send(msg.substr(sentBytes));
		}
	}

	std::string Socket::Receive()
	{
		std::vector<char> buffer(MSG_BUF);
		int bytesRecv = recv(handle, buffer.data(), MSG_BUF, 0);
		if (bytesRecv==SOCKET_ERROR)
		{
			THROW_NET_LAST_WINSOCK_EXCEPTION();
		}

		return std::string(buffer.data(), bytesRecv);
	}

	std::string Socket::ReceiveUntil(int bytesNum)
	{
		std::string msg, prev;
		do
		{
			msg = Receive(bytesNum - msg.size());
			if (msg.size() == 0)
			{
				//Graceful disconnection
				msg = prev;
				break;
			}
			prev = msg;

		} while (msg.size() < bytesNum);

		return msg;
	}

	Socket::Type Net::Socket::GetType() const
	{
		switch (protocol)
		{
		case Protocol::TCP:
			return Type::Stream;
		case Protocol::UDP:
			return Type::Datagram;
		default: //To get rid of stupid warning
			return Type::Datagram; 
		}
	}

	IPVersion Net::Socket::GetIPVersion() const
	{
		return ipVersion;
	}

	Protocol Net::Socket::GetProtocol() const
	{
		return protocol;
	}

	void Socket::SetSocketOption(Option option, bool value)
	{
		BOOL w_val = value ? TRUE : FALSE;
		if (setsockopt(handle, GetOptionLevel(option), (int)option, (const char*)&w_val, sizeof(w_val)) == SOCKET_ERROR)
		{
			THROW_NET_LAST_WINSOCK_EXCEPTION();
		}
	}

	void Net::Socket::Close()
	{
		if (closesocket(handle) == SOCKET_ERROR)
		{
			THROW_NET_LAST_WINSOCK_EXCEPTION();
		}
	}

	Net::Socket::~Socket() noexcept
	{
		if(handle!=INVALID_SOCKET)
			closesocket(handle);
	}

	std::string Socket::Receive(int bufSize)
	{
		std::vector<char> buffer(bufSize);
		int bytesRecv = recv(handle, buffer.data(), bufSize, 0);
		if (bytesRecv == SOCKET_ERROR)
		{
			THROW_NET_LAST_WINSOCK_EXCEPTION();
		}

		return std::string(buffer.data(), bytesRecv);
	}

	int Socket::GetOptionLevel(Option option)
	{
		switch (option)
		{
		case Option::TCP_NoDelay:
			return IPPROTO_TCP;
		default: //To get rid of stupid warning
			return IPPROTO_TCP;
		}
	}

}


