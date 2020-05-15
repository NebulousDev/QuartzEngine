#pragma once

//////////////////////////////////////////////////////////////
// Quaternion Definition
//////////////////////////////////////////////////////////////

#include "Defines.h"

#define QUAT_ALIGN ALIGN(16)

struct Vector3;

/**
	4 floating-point quaternion
*/
struct Quaternion
{
	union
	{
		QUAT_ALIGN float e[4];

		QUAT_ALIGN struct
		{
			float x; // Quaternion's imaginary X component
			float y; // Quaternion's imaginary Y component
			float z; // Quaternion's imaginary Z component
			float w; // Quaternion's real W component
		};
	};

	/** Construct an uninitialized Quaternion */
	FORCEINLINE Quaternion();

	/** Construct a Quaternion from axis and angle */
	FORCEINLINE Quaternion(const Vector3& axis, float angle);

	/** Construct a Quaternion from euler angles */
	FORCEINLINE Quaternion(const Vector3& euler);

	/** Construct a Quaternion from values */
	FORCEINLINE Quaternion(float x, float y, float z, float w);

	/** Set component values */
	FORCEINLINE Quaternion& Set(float x, float y, float z, float w);

	/** Set a Quaternion from axis and angle */
	FORCEINLINE Quaternion& SetAxisAngle(const Vector3& axis, float angle);

	/** Set a Quaternion from euler angles */
	FORCEINLINE Quaternion& SetEuler(const Vector3& euler);

	/** Get the magnitude this quaternion */
	FORCEINLINE float Magnitude() const;

	/** Get the squared magnitude this quaternion */
	FORCEINLINE float MagnitudeSquared() const;

	/** Get the dot product of two quaternion */
	friend FORCEINLINE float Dot(const Quaternion& quat1, const Quaternion& quat2);

	/** Get the conjugate of the quaternion */
	FORCEINLINE Quaternion Conjugate() const;

	/** Get the inverse of the quaternion */
	FORCEINLINE Quaternion Inverse() const;

	/** Normalize this quaternion */
	FORCEINLINE Quaternion& Normalize();

	/** Get the normalized quaternion */
	FORCEINLINE Quaternion Normalized() const;

	/** Multiply a quaternion to this */
	FORCEINLINE Quaternion operator*(const Quaternion& quat) const;

	/** Multiply a quaternion to a Vector3 */
	FORCEINLINE Vector3 operator*(const Vector3& vec3) const;

	/** Multiply a Vector3 by a quaternion */
	friend FORCEINLINE Vector3 operator*(const Vector3& vec3, const Quaternion& quat);

	/** Multiply a quaternion to this */
	FORCEINLINE void operator*=(const Quaternion& quat);

	/** Multiply a float to this */
	FORCEINLINE Quaternion operator*(float value) const;

	/** Multiply a float to this */
	friend FORCEINLINE Quaternion operator*(float value, const Quaternion& quat);

	/** Multiply a float to this */
	FORCEINLINE Quaternion operator*=(float value);

	/** Divide a quaternion from this */
	FORCEINLINE Quaternion operator/(const Quaternion& quat) const;

	/** Divide a quaternion from this */
	FORCEINLINE void operator/=(const Quaternion& quat);

	/** Check if two matrices are equal */
	FORCEINLINE bool operator==(const Quaternion& quat) const;

	/** Check if two matrices are not equal */
	FORCEINLINE bool operator!=(const Quaternion& quat) const;
};