#define WIN32_LEAN_AND_MEAN

#include "NetException.h"

#include <Windows.h>
#include <sstream>


namespace Net
{
	Exception::Exception(int line, const char* file)
		:line(line), file(file)
	{
	}

	WinsockException::WinsockException(int errorCode, int line, const char* file)
		:Exception(line, file), errorCode(errorCode)
	{
	}

	const char* WinsockException::what() const noexcept
	{
		std::ostringstream oss;
		oss << "Exception thrown" << std::endl;
		oss << "Type: " << GetType() << std::endl;
		oss << "Description: " << GetLastErrorDesc();
		oss << "File: " << file << std::endl;
		oss << "Line: " << line << std::endl;

		whatBuffer = std::move(oss.str());
		return whatBuffer.c_str();
	}

	const char* WinsockException::GetType() const
	{
		return "Winsock Exception";
	}

	int WinsockException::GetErrorCode() const
	{
		return errorCode;
	}

	std::string WinsockException::GetLastErrorDesc() const
	{
		char* buf = nullptr;
		DWORD bufSize = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, errorCode, 0,
			reinterpret_cast<LPSTR>(&buf),
			0,
			nullptr
		);

		if (bufSize == 0)
		{
			return "Unidentified error\n";
		}

		std::string msg = buf;
		LocalFree(buf);

		return msg;
	}

	FormatIPAddressException::FormatIPAddressException(int line, const char* file)
		:Exception(line, file)
	{
	}

	const char* FormatIPAddressException::what() const noexcept
	{
		std::ostringstream oss;
		oss << "Exception thrown" << std::endl;
		oss << "Type: " << GetType() << std::endl;
		oss << "Description: The pAddrBuf parameter points to a string" <<
			" that is not a valid IPv4 dotted-decimal string or a" <<
			" valid IPv6 address string." << std::endl;
		oss << "File: " << file << std::endl;
		oss << "Line: " << line << std::endl;

		whatBuffer = std::move(oss.str());
		return whatBuffer.c_str();
	}

	const char* FormatIPAddressException::GetType() const
	{
		return "Server Format IPAddress Exception";
	}

	TimeLimitExceededException::TimeLimitExceededException(int line, const char* file)
		:Exception(line, file)
	{
	}

	const char* TimeLimitExceededException::what() const noexcept
	{
		std::ostringstream oss;
		oss << "Exception thrown" << std::endl;
		oss << "Type: " << GetType() << std::endl;
		oss << "Description: The pAddrBuf parameter points to a string" <<
			" that is not a valid IPv4 dotted-decimal string or a" <<
			" valid IPv6 address string." << std::endl;
		oss << "File: " << file << std::endl;
		oss << "Line: " << line << std::endl;

		whatBuffer = std::move(oss.str());
		return whatBuffer.c_str();
	}

	const char* TimeLimitExceededException::GetType() const
	{
		return "Server Time Limit Exceeded Exception";
	}
}