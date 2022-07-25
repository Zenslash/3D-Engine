#pragma once

#include <queue>

class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};
	private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		int x;
		int y;
	public:
		Event() noexcept
		: type(Type::Invalid), leftIsPressed(false), rightIsPressed(false), x(0), y(0) {}
		Event(Type type, const Mouse& parent) noexcept
		: type(type), leftIsPressed(parent.leftIsPressed),
		rightIsPressed(parent.rightIsPressed),
		x(parent.x), y(parent.y) {}

		[[nodiscard]] bool IsValid() const noexcept
		{
			return type != Type::Invalid;
		}
		[[nodiscard]] Type GetType() const noexcept
		{
			return type;
		}
		[[nodiscard]] std::pair<int, int> GetPos() const noexcept
		{
			return { x, y };
		}
		[[nodiscard]] int GetPosX() const noexcept
		{
			return x;
		}
		[[nodiscard]] int GetPosY() const noexcept
		{
			return y;
		}
		[[nodiscard]] bool IsLeftPressed() const noexcept
		{
			return leftIsPressed;
		}
		[[nodiscard]] bool IsRightPressed() const noexcept
		{
			return rightIsPressed;
		}

	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	[[nodiscard]] std::pair<int, int> GetPos() const noexcept;
	[[nodiscard]] int GetPosX() const noexcept;
	[[nodiscard]] int GetPosY() const noexcept;
	[[nodiscard]] bool IsInWindow() const noexcept;
	[[nodiscard]] bool IsLeftPressed() const noexcept;
	[[nodiscard]] bool IsRightPressed() const noexcept;

	[[nodiscard]] bool IsEmpty() const noexcept
	{
		return buffer.empty();
	}

	Mouse::Event Read() noexcept;
	void Flush() noexcept;
private:
	void OnMouseMove(int x, int y) noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;

	void TrimBuffer() noexcept;
private:
	static constexpr unsigned int bufferSize = 16u;
	std::queue<Mouse::Event> buffer;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	bool isInWindow = false;
	int wheelDeltaCarry = 0;
	int x;
	int y;
};

