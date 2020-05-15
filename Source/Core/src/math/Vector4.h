#pragma once

//////////////////////////////////////////////////////////////
// Vector4 Definition
//////////////////////////////////////////////////////////////

#include "Defines.h"

#define VEC4_ALIGN ALIGN(16)

struct Vector3;

/**
	4 floating-point vector
*/
struct Vector4
{
	union
	{
		VEC4_ALIGN float e[4];

		VEC4_ALIGN struct
		{
			float x; // Vector's X component
			float y; // Vector's Y component
			float z; // Vector's Z component
			float w; // Vector's W component
		};
	};

	/** Construct an uninitialized Vector4 */
	FORCEINLINE Vector4();

	/** Construct an filled Vector4 */
	FORCEINLINE Vector4(float fill);

	/** Construct a Vector4 from values */
	FORCEINLINE Vector4(float x, float y, float z, float w);

	/** Construct a Vector4 from a Vector3 and w component */
	FORCEINLINE Vector4(const Vector3& vec3, float w);

	/** Set component values */
	FORCEINLINE Vector4& Set(float x, float y, float z, float w);

	/** Get the magnitude this vector */
	FORCEINLINE float Magnitude() const;

	/** Get the squared magnitude this vector */
	FORCEINLINE float MagnitudeSquared() const;

	/** Get the dot product of two vectors */
	friend FORCEINLINE float Dot(const Vector4& vec1, const Vector4& vec2);

	/** Normalize this vector */
	FORCEINLINE Vector4& Normalize();

	/** Get the normalized vector */
	FORCEINLINE Vector4 Normalized() const;

	/** Get xyz values as a Vector3 */
	FORCEINLINE Vector3 Xyz() const;

	/** Get a component by index */
	FORCEINLINE float& operator[](int index);

	/** Get a component by index */
	FORCEINLINE float operator[](int index) const;

	/** Get the negated vector */
	FORCEINLINE Vector4 operator-() const;

	/** Add a vector to this */
	FORCEINLINE Vector4 operator+(const Vector4& vec4) const;

	/** Add a vector to this */
	FORCEINLINE void operator+=(const Vector4& vec4);

	/** Subtract a vector to this */
	FORCEINLINE Vector4 operator-(const Vector4& vec4) const;

	/** Subtract a vector to this */
	FORCEINLINE void operator-=(const Vector4& vec4);

	/** Multiply a vector to this */
	FORCEINLINE Vector4 operator*(const Vector4& vec4) const;

	/** Multiply a vector to this */
	FORCEINLINE void operator*=(const Vector4& vec4);

	/** Multiply a float to this */
	FORCEINLINE Vector4 operator*(float value) const;

	/** Multiply a float to this */
	friend FORCEINLINE Vector4 operator*(float value, const Vector4& vec4);

	/** Multiply a float to this */
	FORCEINLINE Vector4 operator*=(float value);

	/** Divide a vector from this */
	FORCEINLINE Vector4 operator/(const Vector4& vec4) const;

	/** Divide a vector from this */
	FORCEINLINE void operator/=(const Vector4& vec4);

	/** Check if two matrices are equal */
	FORCEINLINE bool operator==(const Vector4& vec4) const;

	/** Check if two matrices are not equal */
	FORCEINLINE bool operator!=(const Vector4& vec4) const;
};