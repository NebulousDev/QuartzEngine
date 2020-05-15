#pragma once

//////////////////////////////////////////////////////////////
// Vector3 Implementation
//////////////////////////////////////////////////////////////

#include "Vector3.h"
#include "MathUtil.h"

const Vector3 Vector3::Zero		= {  0.0f,  0.0f,  0.0f };
const Vector3 Vector3::One		= {  1.0f,  1.0f,  1.0f };
const Vector3 Vector3::Up		= {  0.0f,  1.0f,  0.0f };
const Vector3 Vector3::Down		= {  0.0f, -1.0f,  0.0f };
const Vector3 Vector3::Forward	= {  0.0f,  0.0f,  1.0f };
const Vector3 Vector3::Backward = {  0.0f,  0.0f, -1.0f };
const Vector3 Vector3::Right	= {  1.0f,  0.0f,  0.0f };
const Vector3 Vector3::Left		= { -1.0f,  0.0f,  0.0f };

FORCEINLINE Vector3::Vector3()
{
	// Nothing
}

FORCEINLINE Vector3::Vector3(float fill)
{
	this->x = fill;
	this->y = fill;
	this->z = fill;
}

FORCEINLINE Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

FORCEINLINE Vector3::Vector3(const Vector2& vec2, float z)
{
	this->x = vec2.x;
	this->y = vec2.y;
	this->z = z;
}

FORCEINLINE Vector3& Vector3::Set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

FORCEINLINE float Vector3::Magnitude() const
{
	return 1.0f / FastInvsereSquare(MagnitudeSquared());
}

FORCEINLINE float Vector3::MagnitudeSquared() const
{
	return (x * x) + (y * y) + (z * z);
}

FORCEINLINE float Dot(const Vector3& vec1, const Vector3& vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

FORCEINLINE Vector3 Cross(const Vector3& vec1, const Vector3& vec2)
{
	float x = vec1.y * vec2.z - vec1.z * vec2.y;
	float y = vec1.z * vec2.x - vec1.x * vec2.z;
	float z = vec1.x * vec2.y - vec1.y * vec2.x;
	return Vector3(x, y, z);
}

FORCEINLINE Vector3& Vector3::Normalize()
{
	float fastInv = FastInvsereSquare(MagnitudeSquared());
	this->x *= fastInv;
	this->y *= fastInv;
	this->z *= fastInv;
	return *this;
}

FORCEINLINE Vector3 Vector3::Normalized() const
{
	Vector3 result;
	float fastInv = FastInvsereSquare(MagnitudeSquared());
	result.x = x * fastInv;
	result.y = y * fastInv;
	result.z = z * fastInv;
	return result;
}

FORCEINLINE Vector2 Vector3::Xy() const
{
	return Vector2(x, y);
}

FORCEINLINE float& Vector3::operator[](int index)
{
	return e[index];
}

FORCEINLINE float Vector3::operator[](int index) const
{
	return e[index];
}

FORCEINLINE Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

FORCEINLINE Vector3 Vector3::operator+(const Vector3& vec3) const
{
	return Vector3(x + vec3.x, y + vec3.y, z + vec3.z);
}

FORCEINLINE void Vector3::operator+=(const Vector3& vec3)
{
	x += vec3.x;
	y += vec3.y;
	z += vec3.z;
}

FORCEINLINE Vector3 Vector3::operator-(const Vector3& vec3) const
{
	return Vector3(x - vec3.x, y - vec3.y, z - vec3.z);
}

FORCEINLINE void Vector3::operator-=(const Vector3& vec3)
{
	x -= vec3.x;
	y -= vec3.y;
	z -= vec3.z;
}

FORCEINLINE Vector3 Vector3::operator*(const Vector3& vec3) const
{
	return Vector3(x * vec3.x, y * vec3.y, z * vec3.z);
}

FORCEINLINE void Vector3::operator*=(const Vector3& vec3)
{
	x *= vec3.x;
	y *= vec3.y;
	z *= vec3.z;
}

FORCEINLINE Vector3 Vector3::operator*(float value) const
{
	return Vector3(x * value, y * value, z * value);
}

FORCEINLINE Vector3 Vector3::operator*=(float value)
{
	x *= value;
	y *= value;
	z *= value;
}

FORCEINLINE Vector3 operator*(float value, const Vector3& vec3)
{
	return vec3 * value;
}

FORCEINLINE Vector3 Vector3::operator/(const Vector3& vec3) const
{
	return Vector3(x / vec3.x, y / vec3.y, z / vec3.z);
}

FORCEINLINE void Vector3::operator/=(const Vector3& vec3)
{
	x /= vec3.x;
	y /= vec3.y;
	z /= vec3.z;
}

FORCEINLINE bool Vector3::operator==(const Vector3& vec3) const
{
	return x == vec3.x && y == vec3.y && z == vec3.z;
}

FORCEINLINE bool Vector3::operator!=(const Vector3& vec3) const
{
	return !(*this == vec3);
}