#include "ZenException.h"
#include <sstream>

ZenException::ZenException(int line, const char* file) noexcept
	: line(line), file(file)
{}

const char* ZenException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* ZenException::GetType() const noexcept
{
	return "ZenException";
}

int ZenException::GetLine() const noexcept
{
	return line;
}

const std::string& ZenException::GetFile() const noexcept
{
	return file;
}

std::string ZenException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}
