#pragma once

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;

template<typename T>
constexpr auto Square(const T& x)
{
	return x * x;
}