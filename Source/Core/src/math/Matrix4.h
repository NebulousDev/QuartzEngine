#pragma once

//////////////////////////////////////////////////////////////
// Matrix4 Definition
//////////////////////////////////////////////////////////////

#include "Defines.h"

struct Vector4;
struct Vector3;
struct Quaternion;

/**
	4x4 floating-point matrix
*/
struct Matrix4
{
	union
	{
		// Row major: accessed m[row][column]
		float m[4][4];

		// Row major: accessed e[row + column * 4]
		float e[16];

		// Row major
		struct
		{
			float m00, m01, m02, m03; // Left
			float m10, m11, m12, m13; // Up
			float m20, m21, m22, m23; // Forward
			float m30, m31, m32, m33; // Position
		};
	};

	/** Construct an uninitialized Matrix4 */
	FORCEINLINE Matrix4();

	/** Construct a Matrix4 from values */
	FORCEINLINE Matrix4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	/** Construct a Matrix4 from vectors */
	FORCEINLINE Matrix4(const Vector4& xvec, const Vector4& yvec, const Vector4& zvec, const Vector4& wvec);

	/** Set to the identity matrix */
	FORCEINLINE Matrix4& SetZero();

	/** Set to the identity matrix */
	FORCEINLINE Matrix4& SetIdentity();

	/** Set to a translation matrix */
	FORCEINLINE Matrix4& SetTranslation(const Vector3& translation);

	/** Set to a rotation matrix */
	FORCEINLINE Matrix4& SetRotation(const Quaternion& rotation);

	/** Set to a scale matrix */
	FORCEINLINE Matrix4& SetScale(const Vector3& scale);

	/** Set to a view matrix */
	FORCEINLINE Matrix4& SetView(const Vector3& right, const Vector3& up, const Vector3& forward, const Vector3& position);

	/** Set to a lookat matrix */
	FORCEINLINE Matrix4& SetLookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

	/** Set to a orthographic matrix */
	FORCEINLINE Matrix4& SetOrthographic(float left, float right, float top, float bottom, float near, float far);

	/** Set to a perspective matrix */
	FORCEINLINE Matrix4& SetPerspective(float fov, float aspect, float zNear, float zFar);

	/** Transpose this matrix */
	FORCEINLINE Matrix4& Transpose();

	/** Get the transposed matrix */
	FORCEINLINE Matrix4 Transposed() const;

	/** Get the floating-point determinant */
	FORCEINLINE float Determinant() const;

	/** Get the inverse of the matrix */
	FORCEINLINE Matrix4 Inverse() const;

	/** Devide each column by a vector */
	FORCEINLINE Matrix4 DivideColumns(const Vector4& col1, const Vector4& col2, const Vector4& col3, const Vector4& col4);

	/** Get the origin vector */
	FORCEINLINE Vector3 GetTranslation() const;

	/** Get the normalized up vector */
	FORCEINLINE Vector3 GetUp() const;

	/** Get the normalized down vector */
	FORCEINLINE Vector3 GetDown() const;

	/** Get the normalized right vector */
	FORCEINLINE Vector3 GetRight() const;

	/** Get the normalized left vector */
	FORCEINLINE Vector3 GetLeft() const;

	/** Get the normalized forward vector */
	FORCEINLINE Vector3 GetForward() const;

	/** Get the normalized backward vector */
	FORCEINLINE Vector3 GetBackward() const;

	/** Get the scaled up vector */
	FORCEINLINE Vector3 GetScaledUp() const;

	/** Get the scaled down vector */
	FORCEINLINE Vector3 GetScaledDown() const;

	/** Get the scaled right vector */
	FORCEINLINE Vector3 GetScaledRight() const;

	/** Get the scaled left vector */
	FORCEINLINE Vector3 GetScaledLeft() const;

	/** Get the scaled forward vector */
	FORCEINLINE Vector3 GetScaledForward() const;

	/** Get the scaled backward vector */
	FORCEINLINE Vector3 GetScaledBackward() const;

	/** Get a component by index */
	FORCEINLINE float& operator[](int index);

	/** Get a component by index */
	FORCEINLINE float operator[](int index) const;

	/** Multiply a matrix to this */
	FORCEINLINE Matrix4 operator*(const Matrix4& mat4) const;

	/** Multiply this by a matrix */
	FORCEINLINE void operator*=(const Matrix4& mat4);

	/** Multiply a Vector4 to this */
	FORCEINLINE Vector4 operator*(const Vector4& vec4) const;

	/** Multiply a Vector4 to this */
	friend FORCEINLINE Vector4 operator*(const Vector4& vec4, const Matrix4& mat4);

	/** Multiply a float to this */
	FORCEINLINE Matrix4 operator*(float value) const;

	/** Multiply a float to this */
	friend FORCEINLINE Matrix4 operator*(float value, const Matrix4& mat4);

	/** Multiply a float to this */
	FORCEINLINE void operator*=(float value);

	/** Check if two matrices are equal */
	FORCEINLINE bool operator==(const Matrix4& mat4) const;

	/** Check if two matrices are not equal */
	FORCEINLINE bool operator!=(const Matrix4& mat4) const;
	
};