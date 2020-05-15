#pragma once

//////////////////////////////////////////////////////////////
// Vector2 Definition
//////////////////////////////////////////////////////////////

#include "Defines.h"

/**
	2 floating-point vector
*/
struct Vector2
{
	union
	{
		float e[2];

		struct
		{
			float x; // Vector's X component
			float y; // Vector's Y component
		};
	};

	/** Construct an uninitialized Vector2 */
	FORCEINLINE Vector2();

	/** Construct an filled Vector2 */
	FORCEINLINE Vector2(float fill);

	/** Construct a Vector2 from values */
	FORCEINLINE Vector2(float x, float y);

	/** Set component values */
	FORCEINLINE Vector2& Set(float x, float y);

	/** Get the magnitude this vector */
	FORCEINLINE float Magnitude() const;

	/** Get the squared magnitude this vector */
	FORCEINLINE float MagnitudeSquared() const;

	/** Normalize this vector */
	FORCEINLINE Vector2& Normalize();

	/** Get the normalized vector */
	FORCEINLINE Vector2 Normalized() const;

	/** Get the dot product of two vectors */
	friend FORCEINLINE float Dot(const Vector2& vec1, const Vector2& vec2);

	/** Get a component by index */
	FORCEINLINE float& operator[](int index);

	/** Get a component by index */
	FORCEINLINE float operator[](int index) const;

	/** Get the negated vector */
	FORCEINLINE Vector2 operator-() const;

	/** Add a vector to this */
	FORCEINLINE Vector2 operator+(const Vector2& vec2) const;

	/** Add a vector to this */
	FORCEINLINE void operator+=(const Vector2& vec2);

	/** Subtract a vector to this */
	FORCEINLINE Vector2 operator-(const Vector2& vec2) const;

	/** Subtract a vector to this */
	FORCEINLINE void operator-=(const Vector2& vec2);

	/** Multiply a vector to this */
	FORCEINLINE Vector2 operator*(const Vector2& vec2) const;

	/** Multiply a vector to this */
	FORCEINLINE void operator*=(const Vector2& vec2);

	/** Multiply a float to this */
	FORCEINLINE Vector2 operator*(float value) const;

	/** Multiply a float to this */
	friend FORCEINLINE Vector2 operator*(float value, const Vector2& vec2);

	/** Multiply a float to this */
	FORCEINLINE Vector2 operator*=(float value);

	/** Divide a vector from this */
	FORCEINLINE Vector2 operator/(const Vector2& vec2) const;

	/** Divide a vector from this */
	FORCEINLINE void operator/=(const Vector2& vec2);

	/** Check if two matrices are equal */
	FORCEINLINE bool operator==(const Vector2& vec2) const;

	/** Check if two matrices are not equal */
	FORCEINLINE bool operator!=(const Vector2& vec2) const;
};