#pragma once

#include "WinHeader.h"
#include "ZenException.h"
#include <string>
#include <memory>
#include <assert.h>

class Surface
{
public:
	class Color
	{
	public:
		unsigned int dword;
	public:
		constexpr Color() noexcept : dword()
		{}
		constexpr Color(const Color& color) noexcept : dword(color.dword)
		{}
		constexpr Color(unsigned char x, unsigned char r, unsigned char g, unsigned char b) noexcept
		{
			dword = (x << 24) | (r << 16) | (g << 8) | b;
		}
		constexpr Color(unsigned char r, unsigned char g, unsigned char b) noexcept
		{
			dword = (r << 16) | (g << 8) | b;
		}
		constexpr Color(unsigned char x, Color color) noexcept
		{
			dword = (x << 24) | color.dword;
		}
		Color& operator=(Color color) noexcept
		{
			dword = color.dword;
			return *this;
		}
		Color& operator=(DWORD color) noexcept
		{
			dword = color;
			return *this;
		}
		
		constexpr unsigned char GetX() const noexcept
		{
			return dword >> 24u;
		}
		constexpr unsigned char GetA() const noexcept
		{
			return GetX();
		}
		constexpr unsigned char GetR() const noexcept
		{
			return (dword >> 16u) & 0xFFu;
		}
		constexpr unsigned char GetG() const noexcept
		{
			return (dword >> 8u) & 0xFFu;
		}
		constexpr unsigned char GetB() const noexcept
		{
			return dword & 0xFFu;
		}
		void SetX(unsigned char x) noexcept
		{
			dword = (dword & 0xFFFFFFu) | (x << 24u);
		}
		void SetA(unsigned char a) noexcept
		{
			SetX(a);
		}
		void SetR(unsigned char r) noexcept
		{
			dword = (dword & 0xFF00FFFFu) | (r << 16u);
		}
		void SetG(unsigned char g) noexcept
		{
			dword = (dword & 0xFFFF00FFu) | (g << 8u);
		}
		void SetB(unsigned char b) noexcept
		{
			dword = (dword & 0xFFFFFF00u) | b;
		}
	};
public:
	class Exception : public ZenException
	{
	public:
		Exception(int line, const char* file, std::string note) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		const std::string GetNote() const noexcept;
	private:
		std::string note;
	};
public:
	Surface(unsigned int width, unsigned int height, unsigned int pitch) noexcept;
	Surface(unsigned int width, unsigned int height) noexcept;
	Surface(Surface&& source) noexcept;
	Surface(Surface&) = delete;
	Surface& operator=(Surface&& source) noexcept;
	Surface& operator=(Surface&) = delete;
	~Surface();
	void Clear(Color fillColor) noexcept;
	void PutPixel(unsigned int x, unsigned int y, Color pixelColor) noexcept(!IS_DEBUG);
	Color GetPixel(unsigned int x, unsigned int y) const noexcept(!IS_DEBUG);
	unsigned int GetWidth() const noexcept;
	unsigned int GetHeight() const noexcept;
	const Color* GetBufferPtr() const noexcept;
	static Surface FromFile(const std::string& file);
	void Save(const std::string& file) const;
	void Copy(const Surface& src) noexcept(!IS_DEBUG);
	Color* GetBufferPtr() noexcept;

private:
	Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam) noexcept;
private:
	std::unique_ptr<Color[]> pColorBuffer;
	unsigned int width;
	unsigned int height;

};

