#pragma once

//////////////////////////////////////////////////////////////
// Vector4 Implementation
//////////////////////////////////////////////////////////////

#include "Vector4.h"
#include "Vector3.h"
#include "MathUtil.h"

FORCEINLINE Vector4::Vector4()
{
	// Nothing
}

FORCEINLINE Vector4::Vector4(float fill)
{
	this->x = fill;
	this->y = fill;
	this->z = fill;
	this->w = fill;
}

FORCEINLINE Vector4::Vector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

FORCEINLINE Vector4::Vector4(const Vector3& vec3, float w)
{
	this->x = vec3.x;
	this->y = vec3.y;
	this->z = vec3.z;
	this->w = w;
}

FORCEINLINE Vector4& Vector4::Set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

FORCEINLINE float Vector4::Magnitude() const
{
	return 1.0f / FastInvsereSquare(MagnitudeSquared());
}

FORCEINLINE float Vector4::MagnitudeSquared() const
{
	return (x * x) + (y * y) + (z * z) + (w * w);
}

FORCEINLINE Vector4& Vector4::Normalize()
{
	float fastInv = FastInvsereSquare(MagnitudeSquared());
	this->x *= fastInv;
	this->y *= fastInv;
	this->z *= fastInv;
	this->w *= fastInv;
	return *this;
}

FORCEINLINE Vector4 Vector4::Normalized() const
{
	Vector4 result;
	float fastInv = FastInvsereSquare(MagnitudeSquared());
	result.x = x * fastInv;
	result.y = y * fastInv;
	result.z = z * fastInv;
	result.w = w * fastInv;
	return result;
}

FORCEINLINE float Dot(const Vector4& vec1, const Vector4& vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w;
}

FORCEINLINE Vector3 Vector4::Xyz() const
{
	return Vector3(x, y, z);
}

FORCEINLINE float& Vector4::operator[](int index)
{
	return e[index];
}

FORCEINLINE float Vector4::operator[](int index) const
{
	return e[index];
}

FORCEINLINE Vector4 Vector4::operator-() const
{
	return Vector4(-x, -y, -z, -w);
}

FORCEINLINE Vector4 Vector4::operator+(const Vector4& vec4) const
{
	return Vector4(x + vec4.x, y + vec4.y, z + vec4.z, w + vec4.w);
}

FORCEINLINE void Vector4::operator+=(const Vector4& vec4)
{
	x += vec4.x;
	y += vec4.y;
	z += vec4.z;
	w += vec4.w;
}

FORCEINLINE Vector4 Vector4::operator-(const Vector4& vec4) const
{
	return Vector4(x - vec4.x, y - vec4.y, z - vec4.z, w - vec4.w);
}

FORCEINLINE void Vector4::operator-=(const Vector4& vec4)
{
	x -= vec4.x;
	y -= vec4.y;
	z -= vec4.z;
	w -= vec4.w;
}

FORCEINLINE Vector4 Vector4::operator*(const Vector4& vec4) const
{
	return Vector4(x * vec4.x, y * vec4.y, z * vec4.z, w * vec4.w);
}

FORCEINLINE void Vector4::operator*=(const Vector4& vec4)
{
	x *= vec4.x;
	y *= vec4.y;
	z *= vec4.z;
	w *= vec4.w;
}

FORCEINLINE Vector4 Vector4::operator*(float value) const
{
	return Vector4(x * value, y * value, z * value, w * value);
}

FORCEINLINE Vector4 Vector4::operator*=(float value)
{
	x *= value;
	y *= value;
	z *= value;
	w *= value;
}

FORCEINLINE Vector4 operator*(float value, const Vector4& vec4)
{
	return vec4 * value;
}

FORCEINLINE Vector4 Vector4::operator/(const Vector4& vec4) const
{
	return Vector4(x / vec4.x, y / vec4.y, z / vec4.z, w / vec4.w);
}

FORCEINLINE void Vector4::operator/=(const Vector4& vec4)
{
	x /= vec4.x;
	y /= vec4.y;
	z /= vec4.z;
	w /= vec4.w;
}

FORCEINLINE bool Vector4::operator==(const Vector4& vec4) const
{
	return x == vec4.x && y == vec4.y && z == vec4.z && w == vec4.w;
}

FORCEINLINE bool Vector4::operator!=(const Vector4& vec4) const
{
	return !(*this == vec4);
}