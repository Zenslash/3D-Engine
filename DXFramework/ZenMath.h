#pragma once

#include <math.h>

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;

template<typename T>
constexpr auto Square(const T& x)
{
	return x * x;
}

template<typename T>
constexpr float WrapAngle(T angle)
{
	const T result = fmod(angle, (T)2.0 * (T)PI_D);
	return (result > (T)PI_D) ?
		(result - (T)2.0 * (T)PI_D) :
		result;
}