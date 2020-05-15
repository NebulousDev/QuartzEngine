#pragma once

//////////////////////////////////////////////////////////////
// Quaternion Implementation
//////////////////////////////////////////////////////////////

#include "Quaternion.h"
#include "Vector3.h"
#include "MathUtil.h"

FORCEINLINE Quaternion::Quaternion()
{
	// Nothing
}

FORCEINLINE Quaternion::Quaternion(const Vector3& axis, float angle)
{
	SetAxisAngle(axis, angle);
}

FORCEINLINE Quaternion::Quaternion(const Vector3& euler)
{
	SetEuler(euler);
}

FORCEINLINE Quaternion::Quaternion(float x, float y, float z, float w)
{
	Set(x, y, z, w);
}

FORCEINLINE Quaternion& Quaternion::Set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;

	return *this;
}

FORCEINLINE Quaternion& Quaternion::SetAxisAngle(const Vector3& axis, float angle)
{
	float sinHalfAngle = sinf(angle * 0.5f);
	float cosHalfAngle = cosf(angle * 0.5f);

	this->x = axis.x * sinHalfAngle;
	this->y = axis.y * sinHalfAngle;
	this->z = axis.z * sinHalfAngle;
	this->w = cosHalfAngle;

	return *this;
}

FORCEINLINE Quaternion& Quaternion::SetEuler(const Vector3& euler)
{
	float cx = cosf(x * 0.5f);
	float cy = cosf(y * 0.5f);
	float cz = cosf(z * 0.5f);
	float sx = sinf(x * 0.5f);
	float sy = sinf(y * 0.5f);
	float sz = sinf(z * 0.5f);

	this->x = cx * sy * sz + cy * cz * sx;
	this->y = cx * cz * sy - cy * sx * sz;
	this->z = cx * cy * sz - cz * sx * sy;
	this->w = sx * sy * sz + cx * cy * cz;

	return *this;
}

FORCEINLINE float Quaternion::Magnitude() const
{
	return 1.0f / FastInvsereSquare(MagnitudeSquared());
}

FORCEINLINE float Quaternion::MagnitudeSquared() const
{
	return (x * x) + (y * y) + (z * z) + (w * w);
}

FORCEINLINE Quaternion& Quaternion::Normalize()
{
	float fastInv = FastInvsereSquare(MagnitudeSquared());
	this->x *= fastInv;
	this->y *= fastInv;
	this->z *= fastInv;
	this->w *= fastInv;
	return *this;
}

FORCEINLINE Quaternion Quaternion::Normalized() const
{
	float fastInv = FastInvsereSquare(MagnitudeSquared());
	float rx = x * fastInv;
	float ry = y * fastInv;
	float rz = z * fastInv;
	float rw = w * fastInv;
	return Quaternion(rx, ry, rz, rw);
}

FORCEINLINE float Dot(const Quaternion& quat1, const Quaternion& quat2)
{
	return quat1.x * quat2.x + quat1.y * quat2.y + quat1.z * quat2.z + quat1.w * quat2.w;
}

FORCEINLINE Quaternion Quaternion::Conjugate() const
{
	return Quaternion(-x, -y, -z, w);
}

FORCEINLINE Quaternion Quaternion::Inverse() const
{
	float magsq = MagnitudeSquared();
	float rx = -x / magsq;
	float ry = -y / magsq;
	float rz = -z / magsq;
	float rw = w / magsq;
	return Quaternion(rx, ry, rz, rw);
}

FORCEINLINE Quaternion Quaternion::operator*(const Quaternion& quat) const
{
	float rx = w * quat.x + x * quat.w + y * quat.z - z * quat.y;
	float ry = w * quat.y + y * quat.w + z * quat.x - x * quat.z;
	float rz = w * quat.z + z * quat.w + x * quat.y - y * quat.x;
	float rw = w * quat.w - x * quat.x - y * quat.y - z * quat.z;
	return Quaternion(rx, ry, rz, rw);
}

FORCEINLINE Vector3 Quaternion::operator*(const Vector3& vec3) const
{
	Quaternion res = *this * Quaternion(vec3.x, vec3.y, vec3.z, 0) * Inverse();
	return Vector3(res.x, res.y, res.z);
}

FORCEINLINE Vector3 operator*(const Vector3& vec3, const Quaternion& quat)
{
	return quat * vec3;
}

FORCEINLINE void Quaternion::operator*=(const Quaternion& quat)
{
	*this = *this * quat;
}

FORCEINLINE Quaternion Quaternion::operator*(float value) const
{
	return Quaternion(x * value, y * value, z * value, w * value);
}

FORCEINLINE Quaternion Quaternion::operator*=(float value)
{
	x *= value;
	y *= value;
	z *= value;
	w *= value;
}

FORCEINLINE Quaternion operator*(float value, const Quaternion& quat)
{
	return quat * value;
}

FORCEINLINE Quaternion Quaternion::operator/(const Quaternion& quat) const
{
	return *this * quat.Inverse();
}

FORCEINLINE void Quaternion::operator/=(const Quaternion& quat)
{
	*this = *this * quat.Inverse();
}

FORCEINLINE bool Quaternion::operator==(const Quaternion& quat) const
{
	return x == quat.x && y == quat.y && z == quat.z && w == quat.w;
}

FORCEINLINE bool Quaternion::operator!=(const Quaternion& quat) const
{
	return !(*this == quat);
}