#pragma once
#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <utility>

#include "IPVersion.h"
#include "Protocol.h"
#include "IPEndpoint.h"

namespace Net
{
	typedef SOCKET SocketHandle;

	class Socket
	{
	public:
		enum class Type
		{
			Stream = SOCK_STREAM,
			Datagram = SOCK_DGRAM
		};
		enum class Option
		{
			TCP_NoDelay = TCP_NODELAY //  Disable Nagle's algorithm
		};
	public:
		Socket();
		Socket(IPVersion ipVersion,Protocol protocol);
		Socket(IPVersion ipVersion, Protocol protocol, SocketHandle handle);

		Socket(const Socket&) = delete;
		Socket& operator=(const Socket& other) = delete;

		Socket(Socket&& other)noexcept;
		Socket& operator=(Socket&& other) noexcept;

		void Bind(const IPEndpoint& ipEndpoint);
		void Listen(const IPEndpoint& ipEndpoint, int backlog = 5);
		std::pair<Socket, IPEndpoint> Accept();
		void Connect(const IPEndpoint& ipEndpoint);

		int Send(const std::string& msg);
		void SendAll(const std::string& msg);
		std::string Receive();
		std::string ReceiveUntil(int bytesNum);

		Type GetType() const;
		IPVersion GetIPVersion() const;
		Protocol GetProtocol() const;

		void SetSocketOption(Option option, bool value);
		void Close();
		~Socket() noexcept;
	private:
		std::string Receive(int bufSize);
		int GetOptionLevel(Option option);
	private:
		SocketHandle handle = INVALID_SOCKET;

		IPVersion ipVersion;
		Protocol protocol;
	public:
		constexpr static int MSG_BUF = 512;
	};
}
