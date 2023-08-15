//Client code
#define WIN32_LEAN_AND_MEAN

#include <Net/Net.h>

#include <Windows.h>

#include <iostream>
#include <thread>

using namespace Net;

int main()
{
	std::string msg = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
	try
	{
		using namespace std::chrono_literals;

		Network::Initialize();
		Socket socket(IPVersion::IPv4, Protocol::TCP);
		socket.Connect(IPEndpoint("192.168.0.6", 4790));

		socket.SendAll(msg);

		socket.Close();
	}
	catch (const Exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Exception Thrown", MB_OK | MB_ICONERROR);
	}

	Network::Deinitialize();

	system("pause");
	return 0;
}