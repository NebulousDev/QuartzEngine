#pragma once

//////////////////////////////////////////////////////////////
// Vector2 Implementation
//////////////////////////////////////////////////////////////

#include "Vector2.h"
#include "MathUtil.h"

FORCEINLINE Vector2::Vector2()
{
	// Nothing
}

FORCEINLINE Vector2::Vector2(float fill)
{
	this->x = fill;
	this->y = fill;
}

FORCEINLINE Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

FORCEINLINE Vector2& Vector2::Set(float x, float y)
{
	this->x = x;
	this->y = y;
}

FORCEINLINE float Vector2::Magnitude() const
{
	return 1.0f / FastInvsereSquare(MagnitudeSquared());
}

FORCEINLINE float Vector2::MagnitudeSquared() const
{
	return (x * x) + (y * y);
}

FORCEINLINE float Dot(const Vector2& vec1, const Vector2& vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

FORCEINLINE Vector2& Vector2::Normalize()
{
	float fastInv = FastInvsereSquare(MagnitudeSquared());
	this->x *= fastInv;
	this->y *= fastInv;
	return *this;
}

FORCEINLINE Vector2 Vector2::Normalized() const
{
	Vector2 result;
	float fastInv = FastInvsereSquare(MagnitudeSquared());
	result.x = x * fastInv;
	result.y = y * fastInv;
	return result;
}

FORCEINLINE float& Vector2::operator[](int index)
{
	return e[index];
}

FORCEINLINE float Vector2::operator[](int index) const
{
	return e[index];
}

FORCEINLINE Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

FORCEINLINE Vector2 Vector2::operator+(const Vector2& vec2) const
{
	return Vector2(x + vec2.x, y + vec2.y);
}

FORCEINLINE void Vector2::operator+=(const Vector2& vec2)
{
	x += vec2.x;
	y += vec2.y;
}

FORCEINLINE Vector2 Vector2::operator-(const Vector2& vec2) const
{
	return Vector2(x - vec2.x, y - vec2.y);
}

FORCEINLINE void Vector2::operator-=(const Vector2& vec2)
{
	x -= vec2.x;
	y -= vec2.y;
}

FORCEINLINE Vector2 Vector2::operator*(const Vector2& vec2) const
{
	return Vector2(x * vec2.x, y * vec2.y);
}

FORCEINLINE void Vector2::operator*=(const Vector2& vec2)
{
	x *= vec2.x;
	y *= vec2.y;
}

FORCEINLINE Vector2 Vector2::operator*(float value) const
{
	return Vector2(x * value, y * value);
}

FORCEINLINE Vector2 Vector2::operator*=(float value)
{
	x *= value;
	y *= value;
}

FORCEINLINE Vector2 operator*(float value, const Vector2& vec2)
{
	return vec2 * value;
}

FORCEINLINE Vector2 Vector2::operator/(const Vector2& vec2) const
{
	return Vector2(x / vec2.x, y / vec2.y);
}

FORCEINLINE void Vector2::operator/=(const Vector2& vec2)
{
	x /= vec2.x;
	y /= vec2.y;
}

FORCEINLINE bool Vector2::operator==(const Vector2& vec2) const
{
	return x == vec2.x && y == vec2.y;
}

FORCEINLINE bool Vector2::operator!=(const Vector2& vec2) const
{
	return !(*this == vec2);
}