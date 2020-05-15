#pragma once

//////////////////////////////////////////////////////////////
// Vector3 Definition
//////////////////////////////////////////////////////////////

#include "Defines.h"

/**
	3 floating-point vector
*/
struct Vector3
{
	union
	{
		float e[3];

		struct
		{
			float x; // Vector's X component
			float y; // Vector's Y component
			float z; // Vector's Z component
		};
	};

	/** Zero vector (0,0,0) */
	static const Vector3 Zero;

	/** One vector (1,1,1) */
	static const Vector3 One;

	/** Up vector (0,1,0) */
	static const Vector3 Up;

	/** Down vector (0,-1,0) */
	static const Vector3 Down;

	/** Forward vector (0,0,1) */
	static const Vector3 Forward;

	/** Backward vector (0,0,-1) */
	static const Vector3 Backward;

	/** Right vector (1,0,0) */
	static const Vector3 Right;

	/** Left vector (-1,0,0) */
	static const Vector3 Left;

	/** Construct an uninitialized Vector3 */
	FORCEINLINE Vector3();

	/** Construct an filled Vector3 */
	FORCEINLINE Vector3(float fill);
	
	/** Construct a Vector3 from values */
	FORCEINLINE Vector3(float x, float y, float z);

	/** Construct a Vector3 from a Vector2 and z component */
	FORCEINLINE Vector3(const Vector2& vec2, float z);

	/** Set component values */
	FORCEINLINE Vector3& Set(float x, float y, float z);

	/** Get the magnitude this vector */
	FORCEINLINE float Magnitude() const;

	/** Get the squared magnitude this vector */
	FORCEINLINE float MagnitudeSquared() const;

	/** Get the dot product of two vectors */
	friend FORCEINLINE float Dot(const Vector3& vec1, const Vector3& vec2);

	/** Get the cross product of two vectors */
	friend FORCEINLINE Vector3 Cross(const Vector3& vec1, const Vector3& vec2);

	/** Normalize this vector */
	FORCEINLINE Vector3& Normalize();

	/** Get the normalized vector */
	FORCEINLINE Vector3 Normalized() const;

	/** Get xy values as a Vector2 */
	FORCEINLINE Vector2 Xy() const;

	/** Get a component by index */
	FORCEINLINE float& operator[](int index);

	/** Get a component by index */
	FORCEINLINE float operator[](int index) const;

	/** Get the negated vector */
	FORCEINLINE Vector3 operator-() const;

	/** Add a vector to this */
	FORCEINLINE Vector3 operator+(const Vector3& vec3) const;

	/** Add a vector to this */
	FORCEINLINE void operator+=(const Vector3& vec3);

	/** Subtract a vector to this */
	FORCEINLINE Vector3 operator-(const Vector3& vec3) const;

	/** Subtract a vector to this */
	FORCEINLINE void operator-=(const Vector3& vec3);

	/** Multiply a vector to this */
	FORCEINLINE Vector3 operator*(const Vector3& vec3) const;

	/** Multiply a vector to this */
	FORCEINLINE void operator*=(const Vector3& vec3);

	/** Multiply a float to this */
	FORCEINLINE Vector3 operator*(float value) const;

	/** Multiply a float to this */
	friend FORCEINLINE Vector3 operator*(float value, const Vector3& vec3);

	/** Multiply a float to this */
	FORCEINLINE Vector3 operator*=(float value);

	/** Divide a vector from this */
	FORCEINLINE Vector3 operator/(const Vector3& vec3) const;

	/** Divide a vector from this */
	FORCEINLINE void operator/=(const Vector3& vec3);

	/** Check if two matrices are equal */
	FORCEINLINE bool operator==(const Vector3& vec3) const;

	/** Check if two matrices are not equal */
	FORCEINLINE bool operator!=(const Vector3& vec3) const;
};