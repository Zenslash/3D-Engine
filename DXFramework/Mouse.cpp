
#include "WinHeader.h"
#include "Mouse.h"


std::pair<int, int> Mouse::GetPos() const noexcept
{
	return {x, y};
}

int Mouse::GetPosX() const noexcept
{
	return x;
}

int Mouse::GetPosY() const noexcept
{
	return y;
}

bool Mouse::IsInWindow() const noexcept
{
	return isInWindow;
}

bool Mouse::IsLeftPressed() const noexcept
{
	return leftIsPressed;
}

bool Mouse::IsRightPressed() const noexcept
{
	return rightIsPressed;
}

Mouse::Event Mouse::Read() noexcept
{
	if(!buffer.empty())
	{
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	else
	{
		return Mouse::Event();
	}
}

void Mouse::Flush() noexcept
{
	buffer = std::queue<Mouse::Event>();
}

void Mouse::OnMouseMove(int x, int y) noexcept
{
	this->x = x;
	this->y = y;

	buffer.push(Mouse::Event(Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y) noexcept
{
	leftIsPressed = true;

	buffer.push(Mouse::Event(Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
	leftIsPressed = false;

	buffer.push(Mouse::Event(Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept
{
	rightIsPressed = true;

	buffer.push(Mouse::Event(Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
	rightIsPressed = false;

	buffer.push(Mouse::Event(Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
	isInWindow = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
	isInWindow = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
	wheelDeltaCarry += delta;
	while(wheelDeltaCarry >= WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while(wheelDeltaCarry <= -WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}

void Mouse::TrimBuffer() noexcept
{
	while(buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}
