#pragma once


#include <exception>
#include <string>

namespace Net
{
	class Exception : public std::exception
	{
	public:
		Exception(int line, const char* file);
		virtual const char* what() const noexcept override = 0;
		virtual ~Exception() override = default;
	protected:
		mutable std::string whatBuffer;

		int line;
		const char* file;
	};

	class WinsockException : public Exception
	{
	public:
		WinsockException(int errorCode, int line, const char* file);
		virtual const char* what() const noexcept override;
		const char* GetType() const;
		int GetErrorCode()const;
		virtual ~WinsockException() override = default;
	private:
		std::string GetLastErrorDesc() const;
	protected:
		int errorCode;
	};
	class FormatIPAddressException : public Exception
	{
	public:
		FormatIPAddressException(int line, const char* file);
		virtual const char* what() const noexcept override;
		const char* GetType() const;
		virtual ~FormatIPAddressException() override = default;
	};
	class TimeLimitExceededException : public Exception
	{
	public:
		TimeLimitExceededException(int line, const char* file);
		virtual const char* what() const noexcept override;
		const char* GetType() const;
		virtual ~TimeLimitExceededException() override = default;
	};
}

#define THROW_NET_WINSOCK_EXCEPTION(code) throw Net::WinsockException(code, __LINE__, __FILE__)
#define THROW_NET_LAST_WINSOCK_EXCEPTION() int error = WSAGetLastError();throw Net::WinsockException(error, __LINE__, __FILE__)
#define THROW_NET_FORMAT_IPADDRESS_EXCEPTION() throw Net::FormatIPAddressException(__LINE__, __FILE__)
#define THROW_NET_TIME_LIMIT_EXCEEDED_EXCEPTION() throw Net::TimeLimitExceededException(__LINE__, __FILE__)

