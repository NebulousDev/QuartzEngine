#pragma once

//////////////////////////////////////////////////////////////
// Matrix4 Implementation
//////////////////////////////////////////////////////////////

#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"

FORCEINLINE Matrix4::Matrix4()
{
	// Nothing
}

FORCEINLINE Matrix4::Matrix4(float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
{
	this->m00 = m00; this->m01 = m01; this->m02 = m02; this->m03 = m03;
	this->m10 = m10; this->m11 = m11; this->m12 = m12; this->m13 = m13;
	this->m20 = m20; this->m21 = m21; this->m22 = m22; this->m23 = m23;
	this->m30 = m30; this->m31 = m31; this->m32 = m32; this->m33 = m33;
}

FORCEINLINE Matrix4::Matrix4(const Vector4& xvec, const Vector4& yvec, const Vector4& zvec, const Vector4& wvec)
{
	this->m00 = xvec.x; this->m01 = xvec.y; this->m02 = xvec.z; this->m03 = xvec.w;
	this->m10 = yvec.x; this->m11 = yvec.y; this->m12 = yvec.z; this->m13 = yvec.w;
	this->m20 = zvec.x; this->m21 = zvec.y; this->m22 = zvec.z; this->m23 = zvec.w;
	this->m30 = wvec.x; this->m31 = wvec.y; this->m32 = wvec.z; this->m33 = wvec.w;
}

FORCEINLINE Matrix4& Matrix4::SetZero()
{
	for (int i = 0; i < 16; i++) e[i] = 0.0f;
	return *this;
}

FORCEINLINE Matrix4& Matrix4::SetIdentity()
{
	m00 = 1; m01 = 0; m02 = 0; m03 = 0;
	m10 = 0; m11 = 1; m12 = 0; m13 = 0;
	m20 = 0; m21 = 0; m22 = 1; m23 = 0;
	m30 = 0; m31 = 0; m32 = 0; m33 = 1;
	return *this;
}

FORCEINLINE Matrix4& Matrix4::SetTranslation(const Vector3& translation)
{
	m00 = 1; m01 = 0; m02 = 0; m03 = 0;
	m10 = 0; m11 = 1; m12 = 0; m13 = 0;
	m20 = 0; m21 = 0; m22 = 1; m23 = 0;
	m30 = translation.x; 
	m31 = translation.y; 
	m32 = translation.z; 
	m33 = 1;
	return *this;
}

FORCEINLINE Matrix4& Matrix4::SetRotation(const Quaternion& rotation)
{
	float qx = rotation.x;
	float qy = rotation.y;
	float qz = rotation.z;
	float qw = rotation.w;

	m00 = 1.0f - 2.0f * ((qy * qy) + (qz * qz));
	m01 =		 2.0f * ((qx * qy) + (qz * qw));
	m02 =		 2.0f * ((qx * qz) - (qy * qw));
	m03 =		 0.0f;

	m10 =		 2.0f * ((qx * qy) - (qz * qw));
	m11 = 1.0f - 2.0f * ((qx * qx) + (qz * qz));
	m12 =		 2.0f * ((qy * qz) + (qx * qw));
	m13 =		 0.0f;

	m20 =		 2.0f * ((qx * qz) + (qy * qw));
	m21 =		 2.0f * ((qy * qz) - (qx * qw));
	m22 = 1.0f - 2.0f * ((qx * qx) + (qy * qy));
	m23 =		 0.0f;

	m30 = 0.0f;
	m31 = 0.0f;
	m32 = 0.0f;
	m33 = 1.0f;

	return *this;
}

FORCEINLINE Matrix4& Matrix4::SetScale(const Vector3& scale)
{
	m00 = scale.x;	m01 = 0;		m02 = 0;		m03 = 0;
	m10 = 0;		m11 = scale.y;	m12 = 0;		m13 = 0;
	m20 = 0;		m21 = 0;		m22 = scale.z;	m23 = 0;
	m30 = 0;		m31 = 0;		m32 = 0;		m33 = 1;
	return *this;
}

FORCEINLINE Matrix4& Matrix4::SetView(const Vector3& right, const Vector3& up, const Vector3& forward, const Vector3& position)
{
	m00 = right.x;		m01 = right.y;		m02 = right.z;		m03 = 0.0f;
	m10 = up.x;			m11 = up.y;			m12 = up.z;			m13 = 0.0f;
	m20 = forward.x;	m21 = forward.y;	m22 = forward.z;	m23 = 0.0f;
	m30 = position.x;	m31 = position.y;	m32 = position.z;	m33 = 1.0f;
	return *this;
}

FORCEINLINE Matrix4& Matrix4::SetLookAt(const Vector3& eye, const Vector3& target, const Vector3& globalUp)
{
	Vector3 forward = (target - eye).Normalize();
	Vector3 right = Cross(globalUp, forward).Normalize();
	Vector3 up = Cross(forward, right);

	Vector3 position(Dot(right, eye), Dot(up, eye), Dot(forward, eye));

	return SetView(-right, up, forward, -position);
}

FORCEINLINE Matrix4& Matrix4::SetOrthographic(float left, float right, float top, float bottom, float near, float far)
{
	SetZero();
	return *this;
}

FORCEINLINE Matrix4& Matrix4::SetPerspective(float fov, float aspect, float zNear, float zFar)
{
#if 0
	float fovY = 1.0f / tanf(fov * 0.5f);
	float range = (zNear - zFar);

	SetZero();
	m00 = fovY / aspect;
	m11 = fovY;
	m22 = (zFar + zNear) / range;
	m23 = -1.0f;
	m32 = 2.0f * zFar * zNear / range;
	return *this;
#elif 1
	float fovY = 1.0f / tanf(fov * 0.5f);
	float range = (zFar - zNear);

	SetZero();
	m00 = fovY / aspect;
	m11 = fovY;
	m22 = -(zNear + zFar) / range;
	m23 = -1.0f;
	m32 = (-2.0f * zFar * zNear) / range;
	return *this;
#else
	float fovY = 1.0f / tanf(fov * 0.5f);
	float range = (zNear - zFar);

	SetZero();
	m00 = fovY / -aspect;
	m11 = fovY;
	m22 = (-zNear - zFar) / range;
	m23 = 1.0f;
	m32 = 2.0f * zFar * zNear / range;
	return *this;
#endif
}

FORCEINLINE Matrix4 Matrix4::Transposed() const
{
	Matrix4 result;
	result.m00 = m00; result.m01 = m10; result.m02 = m20; result.m03 = m30;
	result.m10 = m01; result.m11 = m11; result.m12 = m21; result.m13 = m31;
	result.m20 = m02; result.m21 = m12; result.m22 = m22; result.m23 = m32;
	result.m30 = m03; result.m31 = m13; result.m32 = m23; result.m33 = m33;
	return result;
}

FORCEINLINE Matrix4& Matrix4::Transpose()
{
	return *this = Transposed();
}

// Modified from UnrealEngine's FMatrix::Determinant()
FORCEINLINE float Matrix4::Determinant() const
{
	return	
		  m00 * ( m11 * (m22 * m33 - m23 * m32) - m21 * (m12 * m33 - m13 * m32) + m31 * (m12 * m23 - m13 * m22))
		- m10 * ( m01 * (m22 * m33 - m23 * m32) - m21 * (m02 * m33 - m03 * m32) + m31 * (m02 * m23 - m03 * m22))
		+ m20 * ( m01 * (m12 * m33 - m13 * m32) - m11 * (m02 * m33 - m03 * m32) + m31 * (m02 * m13 - m03 * m12))
		- m30 * ( m01 * (m12 * m23 - m13 * m22) - m11 * (m02 * m23 - m03 * m22) + m21 * (m02 * m13 - m03 * m12));
}

// Modified from https://stackoverflow.com/questions/2624422/efficient-4x4-matrix-inverse-affine-transform
FORCEINLINE Matrix4 Matrix4::Inverse() const
{
	Matrix4 result;

	float s0 = m00 * m11 - m10 * m01;
	float s1 = m00 * m12 - m10 * m02;
	float s2 = m00 * m13 - m10 * m03;
	float s3 = m01 * m12 - m11 * m02;
	float s4 = m01 * m13 - m11 * m03;
	float s5 = m02 * m13 - m12 * m03;

	float c5 = m22 * m33 - m32 * m23;
	float c4 = m21 * m33 - m31 * m23;
	float c3 = m21 * m32 - m31 * m22;
	float c2 = m20 * m33 - m30 * m23;
	float c1 = m20 * m32 - m30 * m22;
	float c0 = m20 * m31 - m30 * m21;

	// Should check for 0 determinant
	float invdet = 1.0 / (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);

	result.m00 = ( m11 * c5 - m12 * c4 + m13 * c3) * invdet;
	result.m01 = (-m01 * c5 + m02 * c4 - m03 * c3) * invdet;
	result.m02 = ( m31 * s5 - m32 * s4 + m33 * s3) * invdet;
	result.m03 = (-m21 * s5 + m22 * s4 - m23 * s3) * invdet;

	result.m10 = (-m10 * c5 + m12 * c2 - m13 * c1) * invdet;
	result.m11 = ( m00 * c5 - m02 * c2 + m03 * c1) * invdet;
	result.m12 = (-m30 * s5 + m32 * s2 - m33 * s1) * invdet;
	result.m13 = ( m20 * s5 - m22 * s2 + m23 * s1) * invdet;

	result.m20 = ( m10 * c4 - m11 * c2 + m13 * c0) * invdet;
	result.m21 = (-m00 * c4 + m01 * c2 - m03 * c0) * invdet;
	result.m22 = ( m30 * s4 - m31 * s2 + m33 * s0) * invdet;
	result.m23 = (-m20 * s4 + m21 * s2 - m23 * s0) * invdet;

	result.m30 = (-m10 * c3 + m11 * c1 - m12 * c0) * invdet;
	result.m31 = ( m00 * c3 - m01 * c1 + m02 * c0) * invdet;
	result.m32 = (-m30 * s3 + m31 * s1 - m32 * s0) * invdet;
	result.m33 = ( m20 * s3 - m21 * s1 + m22 * s0) * invdet;

	return result;
}

// TODO: probably flipped
FORCEINLINE Matrix4 Matrix4::DivideColumns(const Vector4& col1, const Vector4& col2, const Vector4& col3, const Vector4& col4)
{
	Matrix4 result;

	result.m00 = m00 / col1.x;
	result.m10 = m10 / col1.y;
	result.m20 = m20 / col1.z;
	result.m30 = m30 / col1.w;

	result.m01 = m01 / col2.x;
	result.m11 = m11 / col2.y;
	result.m21 = m21 / col2.z;
	result.m31 = m31 / col2.w;

	result.m02 = m02 / col3.x;
	result.m12 = m12 / col3.y;
	result.m22 = m22 / col3.z;
	result.m32 = m32 / col3.w;

	result.m03 = m03 / col4.x;
	result.m13 = m13 / col4.y;
	result.m23 = m23 / col4.z;
	result.m33 = m33 / col4.w;

	return result;
}

FORCEINLINE Vector3 Matrix4::GetTranslation() const
{
	return Vector3(m30, m31, m32);
}

FORCEINLINE Vector3 Matrix4::GetUp() const
{
	return Vector3(m10, m11, m12).Normalized();
}

FORCEINLINE Vector3 Matrix4::GetDown() const
{
	return Vector3(-m10, -m11, -m12).Normalized();
}

FORCEINLINE Vector3 Matrix4::GetRight() const
{
	return Vector3(m00, m01, m02).Normalized();
}

FORCEINLINE Vector3 Matrix4::GetLeft() const
{
	return Vector3(-m00, -m01, -m02).Normalized();
}

FORCEINLINE Vector3 Matrix4::GetForward() const
{
	return Vector3(m20, m21, m22).Normalized();
}

FORCEINLINE Vector3 Matrix4::GetBackward() const
{
	return Vector3(-m20, -m21, -m22).Normalized();
}

FORCEINLINE Vector3 Matrix4::GetScaledUp() const
{
	return Vector3(m10, m11, m12);
}

FORCEINLINE Vector3 Matrix4::GetScaledDown() const
{
	return Vector3(-m10, -m11, -m12);
}

FORCEINLINE Vector3 Matrix4::GetScaledRight() const
{
	return Vector3(m00, m01, m02);
}

FORCEINLINE Vector3 Matrix4::GetScaledLeft() const
{
	return Vector3(-m00, -m01, -m02);
}

FORCEINLINE Vector3 Matrix4::GetScaledForward() const
{
	return Vector3(m20, m21, m22);
}

FORCEINLINE Vector3 Matrix4::GetScaledBackward() const
{
	return Vector3(-m20, -m21, -m22);
}

FORCEINLINE void MultiplyMatrix(const Matrix4& m1, const Matrix4 m2, Matrix4& result)
{
	result.m00 = m1.m00 * m2.m00 + m1.m01 * m2.m10 + m1.m02 * m2.m20 + m1.m03 * m2.m30;
	result.m01 = m1.m00 * m2.m01 + m1.m01 * m2.m11 + m1.m02 * m2.m21 + m1.m03 * m2.m31;
	result.m02 = m1.m00 * m2.m02 + m1.m01 * m2.m12 + m1.m02 * m2.m22 + m1.m03 * m2.m32;
	result.m03 = m1.m00 * m2.m03 + m1.m01 * m2.m13 + m1.m02 * m2.m23 + m1.m03 * m2.m33;

	result.m10 = m1.m10 * m2.m00 + m1.m11 * m2.m10 + m1.m12 * m2.m20 + m1.m13 * m2.m30;
	result.m11 = m1.m10 * m2.m01 + m1.m11 * m2.m11 + m1.m12 * m2.m21 + m1.m13 * m2.m31;
	result.m12 = m1.m10 * m2.m02 + m1.m11 * m2.m12 + m1.m12 * m2.m22 + m1.m13 * m2.m32;
	result.m13 = m1.m10 * m2.m03 + m1.m11 * m2.m13 + m1.m12 * m2.m23 + m1.m13 * m2.m33;

	result.m20 = m1.m20 * m2.m00 + m1.m21 * m2.m10 + m1.m22 * m2.m20 + m1.m23 * m2.m30;
	result.m21 = m1.m20 * m2.m01 + m1.m21 * m2.m11 + m1.m22 * m2.m21 + m1.m23 * m2.m31;
	result.m22 = m1.m20 * m2.m02 + m1.m21 * m2.m12 + m1.m22 * m2.m22 + m1.m23 * m2.m32;
	result.m23 = m1.m20 * m2.m03 + m1.m21 * m2.m13 + m1.m22 * m2.m23 + m1.m23 * m2.m33;

	result.m30 = m1.m30 * m2.m00 + m1.m31 * m2.m10 + m1.m32 * m2.m20 + m1.m33 * m2.m30;
	result.m31 = m1.m30 * m2.m01 + m1.m31 * m2.m11 + m1.m32 * m2.m21 + m1.m33 * m2.m31;
	result.m32 = m1.m30 * m2.m02 + m1.m31 * m2.m12 + m1.m32 * m2.m22 + m1.m33 * m2.m32;
	result.m33 = m1.m30 * m2.m03 + m1.m31 * m2.m13 + m1.m32 * m2.m23 + m1.m33 * m2.m33;
}

FORCEINLINE void MultiplyVectorByMatrix(const Vector4 v, const Matrix4 m, Vector4& result)
{
	result.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30 * v.w;
	result.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31 * v.w;
	result.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32 * v.w;
	result.w = m.m03 * v.x + m.m13 * v.y + m.m23 * v.z + m.m33 * v.w;
}

FORCEINLINE float& Matrix4::operator[](int index)
{
	return e[index];
}

FORCEINLINE float Matrix4::operator[](int index) const
{
	return e[index];
}

FORCEINLINE Matrix4 Matrix4::operator*(const Matrix4& mat4) const
{
	Matrix4 result;
	MultiplyMatrix(*this, mat4, result);
	return result;
}

FORCEINLINE void Matrix4::operator*=(const Matrix4& mat4)
{
	Matrix4 m(*this);
	MultiplyMatrix(m, mat4, *this);
}

FORCEINLINE Vector4 Matrix4::operator*(const Vector4& vec4) const
{
	Vector4 result;
	MultiplyVectorByMatrix(vec4, *this, result);
	return result;
}

FORCEINLINE Vector4 operator*(const Vector4& vec4, const Matrix4& mat4)
{
	return mat4 * vec4;
}

FORCEINLINE Matrix4 Matrix4::operator*(float value) const
{
	Matrix4 result;
	for (int i = 0; i < 16; i++)
		result.e[i] = e[i] * value;
	return result;
}

FORCEINLINE Matrix4 operator*(float value, const Matrix4& mat4)
{
	return mat4 * value;
}

FORCEINLINE void Matrix4::operator*=(float value)
{
	for (int i = 0; i < 16; i++)
		e[i] = e[i] * value;
}

FORCEINLINE bool Matrix4::operator==(const Matrix4& mat4) const
{
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			if (m[row][col] != mat4.m[row][col])
			{
				return false;
			}
		}
	}

	return true;
}

FORCEINLINE bool Matrix4::operator!=(const Matrix4& mat4) const
{
	return !(*this == mat4);
}