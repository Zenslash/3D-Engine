#include "Surface.h"

namespace Gdiplus
{
	using std::min;
	using std::max;
}

#include <gdiplus.h>
#include <sstream>
#include <algorithm>

#pragma comment( lib,"gdiplus.lib" )

Surface::Exception::Exception(int line, const char* file, std::string note) noexcept
	: ZenException(line, file)
{
	this->note = note;
}

const char* Surface::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << ZenException::what() << std::endl
		<< "[Note] " << GetNote();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Surface::Exception::GetType() const noexcept
{
	return "ZenSurface Exception";
}

const std::string Surface::Exception::GetNote() const noexcept
{
	return note;
}

Surface::Surface(unsigned int width, unsigned int height, unsigned int pitch) noexcept
{
	this->width = width;
	this->height = height;
	pColorBuffer = std::make_unique<Color[]>(pitch * height);
}

Surface::Surface(unsigned int width, unsigned int height) noexcept
	: Surface(width, height, width)
{}

Surface::Surface(Surface&& source) noexcept
	:
	width(source.width),
	height(source.height),
	pColorBuffer(std::move(source.pColorBuffer))
{}

Surface& Surface::operator=(Surface&& source) noexcept
{
	width = source.width;
	height = source.height;
	pColorBuffer = std::move(source.pColorBuffer);
	source.pColorBuffer = nullptr;
	return *this;
}

Surface::~Surface()
{}

void Surface::Clear(Color fillColor) noexcept
{
	memset(pColorBuffer.get(), fillColor.dword, width * height * sizeof(Surface::Color));
}

void Surface::PutPixel(unsigned int x, unsigned int y, Color pixelColor) noexcept(!IS_DEBUG)
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x < width);
	assert(y < height);
	pColorBuffer[y * width + x] = pixelColor;
}

Surface::Color Surface::GetPixel(unsigned int x, unsigned int y) const noexcept(!IS_DEBUG)
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x < width);
	assert(y < height);
	return pColorBuffer[y * width + x];
}

unsigned int Surface::GetWidth() const noexcept
{
	return width;
}

unsigned int Surface::GetHeight() const noexcept
{
	return height;
}

const Surface::Color* Surface::GetBufferPtr() const noexcept
{
	return pColorBuffer.get();
}


Surface Surface::FromFile(const std::string& file)
{
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int pitch = 0;
	std::unique_ptr<Color[]> pColorBuffer = nullptr;

	//Loading part
	{
		//Convert string to wide string
		wchar_t wideName[512];
		mbstowcs_s(nullptr, wideName, file.c_str(), _TRUNCATE);

		//Load file
		Gdiplus::Bitmap bitmap(wideName);
		//Handle error
		if (bitmap.GetLastStatus() != Gdiplus::Status::Ok)
		{
			std::stringstream ss;
			ss << "Loading image [" << file << "] failed";
			throw Exception(__LINE__, __FILE__, ss.str());
		}

		height = bitmap.GetHeight();
		width = bitmap.GetWidth();
		pColorBuffer = std::make_unique<Color[]>(width * height);

		for (unsigned int x = 0; x < width; x++)
		{
			for (unsigned int y = 0; y < height; y++)
			{
				Gdiplus::Color c;
				bitmap.GetPixel(x, y, &c);
				pColorBuffer[y * width + x] = c.GetValue();
			}
		}
	}

	return Surface(width, height, std::move(pColorBuffer));
}

void Surface::Save(const std::string& file) const
{
	//Find encoder
	auto GetEncoderClsid = [&file](const WCHAR* format, CLSID* pClsid) -> void
	{
		UINT num = 0;		//image encoder count
		UINT size = 0;		//size of the image encoder array in bytes

		Gdiplus::ImageCodecInfo* pImageCodecInfo = nullptr;

		Gdiplus::GetImageEncodersSize(&num, &size);
		if (size == 0)
		{
			std::stringstream ss;
			ss << "Saving surface to [" << file << "]: failed to get encoder; size == 0.";
			throw Exception(__LINE__, __FILE__, ss.str());
		}

		pImageCodecInfo = static_cast<Gdiplus::ImageCodecInfo*>(malloc(size));
		if (pImageCodecInfo == nullptr)
		{
			std::stringstream ss;
			ss << "Saving surface to [" << file << "]: failed to alloc memory for encoder;";
			throw Exception(__LINE__, __FILE__, ss.str());
		}

		Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

		for (UINT i = 0; i < num; i++)
		{
			if (wcscmp(pImageCodecInfo[i].MimeType, format) == 0)
			{
				*pClsid = pImageCodecInfo[i].Clsid;
				free(pImageCodecInfo);
				return;
			}
		}

		free(pImageCodecInfo);
		std::stringstream ss;
		ss << "Saving surface to [" << file <<
			"]: failed to get encoder; failed to find matching encoder.";
		throw Exception(__LINE__, __FILE__, ss.str());
	};

	CLSID bmpID;
	GetEncoderClsid(L"image/bmp", &bmpID);

	//Convert string to wide string
	wchar_t wideName[512];
	mbstowcs_s(nullptr, wideName, file.c_str(), _TRUNCATE);

	Gdiplus::Bitmap bitmap(width, height, width * sizeof(Color), PixelFormat32bppARGB, reinterpret_cast<BYTE*>(pColorBuffer.get()));
	//Save bitmap
	if (bitmap.Save(wideName, &bmpID, nullptr) != Gdiplus::Status::Ok)
	{
		std::stringstream ss;
		ss << "Saving surface to [" << file << "]: failed to save.";
		throw Exception(__LINE__, __FILE__, ss.str());
	}
}

void Surface::Copy(const Surface& src) noexcept(!IS_DEBUG)
{
	assert(width == src.width);
	assert(height == src.height);
	memcpy(pColorBuffer.get(), src.pColorBuffer.get(), width * height * sizeof(Color));
}

Surface::Color* Surface::GetBufferPtr() noexcept
{
	return pColorBuffer.get();
}

Surface::Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam) noexcept
	:width(width),
	height(height),
	pColorBuffer(std::move(pBufferParam))
{

}
