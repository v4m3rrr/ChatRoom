//Server code
#define WIN32_LEAN_AND_MEAN

#include <Net/Net.h>

#include <Windows.h>

#include <iostream>

using namespace Net;

void PrintIPEndpointInfo(const IPEndpoint& ipEndpoint);
int main()
{
	try
	{
		Network::Initialize();
		Socket socket(IPVersion::IPv4, Protocol::TCP);
		socket.Listen(IPEndpoint("192.168.0.6", 4790));

		std::pair<Socket, IPEndpoint> connection = socket.Accept();
		Socket& connSocket = connection.first;
		IPEndpoint& connEndpoint = connection.second;
		PrintIPEndpointInfo(connEndpoint);

		std::string msg;
		while ((msg = connSocket.ReceiveUntil(97)).size() != 0)
		{
			static int i = 0;
			std::cout << ++i << " Message from client: " << msg << std::endl;
		}

		connSocket.Close();
		socket.Close();
	}
	catch (const Exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Exception Thrown", MB_OK | MB_ICONERROR);
	}

	Network::Deinitialize();
	return 0;
}

void PrintIPEndpointInfo(const IPEndpoint& ipEndpoint)
{
	switch (ipEndpoint.GetIpVersion())
	{
	case IPVersion::IPv4:
		std::cout << "IP Version: IPv4" << std::endl;
		break;
	case IPVersion::IPv6:
		std::cout << "IP Version: IPv6" << std::endl;
		break;
	default:
		std::cout << "IP Version: Unknown" << std::endl;
	}
	std::cout << "Hostname: " << ipEndpoint.GetHostname() << std::endl;
	std::cout << "IP: " << ipEndpoint.GetIPString() << std::endl;
	std::cout << "Port: " << ipEndpoint.GetPort() << std::endl;
	std::cout << "IP bytes..." << std::endl;
	for (auto& digit : ipEndpoint.GetIPBytes())
	{
		std::cout << (int)digit << std::endl;
	}
}