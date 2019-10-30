#pragma once
#include "../Types.h"
#include "../util/Intrinsics.h"

#include "Vector.h"
#include "Quaternion.h"

#ifndef USE_FAST_SQRT
#define USE_FAST_SQRT 1
#endif

namespace Quartz
{
	//////////////////////////////////////////////////////////////
	// Matrix Typedefs
	//////////////////////////////////////////////////////////////

	template<typename Type, UInt8 Align = 4> struct Matrix4;

	typedef Matrix4<Float32, 4> Mat4f;
	typedef Matrix4<Int32, 4> Mat4i;
	typedef Matrix4<UInt32, 4> Mat4u;

	typedef Matrix4<Float32, 16> Mat4fA;
	typedef Matrix4<Int32, 16> Mat4iA;
	typedef Matrix4<UInt32, 16> Mat4uA;

	//////////////////////////////////////////////////////////////
	// Matrix4
	//////////////////////////////////////////////////////////////

	template<typename Type, UInt8 Align>
	struct Matrix4
	{
		static constexpr UInt32 SIZE = 16;

		XMMVEC_ALIGN union
		{
		public:
			Type elements[16];

			Vector4<Type, Align> vectors[4];

			struct
			{
				Type m00, m01, m02, m03;
				Type m10, m11, m12, m13;
				Type m20, m21, m22, m23;
				Type m30, m31, m32, m33;
			};
		};

		Matrix4() :
			elements{}
		{ }

		Matrix4(const Matrix4<Type, Align>& mat4)
		{
			MemCopy(mat4.elements, this->elements, 16 * sizeof(Type));
		}

		Matrix4(const Type elements[16])
		{
			MemCopy(elements, this->elements, 16 * sizeof(Type));
		}

		Matrix4(const Vector4<Type> vectors[4])
		{
			MemCopy(vectors, this->elements, 16 * sizeof(Type));
		}

		Matrix4(const Vector4<Type, Align> vec1, const Vector4<Type, Align> vec2,
			const Vector4<Type, Align> vec3, const Vector4<Type, Align> vec4)
		{
			vectors[0] = vec1;
			vectors[1] = vec2;
			vectors[2] = vec3;
			vectors[3] = vec4;
		}

		Matrix4
		(
			const Type m00, const Type m01, const Type m02, const Type m03,
			const Type m10, const Type m11, const Type m12, const Type m13,
			const Type m20, const Type m21, const Type m22, const Type m23,
			const Type m30, const Type m31, const Type m32, const Type m33
		)
		{
			this->m00 = m00; this->m01 = m01; this->m02 = m02; this->m03 = m03;
			this->m10 = m10; this->m11 = m11; this->m12 = m12; this->m13 = m13;
			this->m20 = m20; this->m21 = m21; this->m22 = m22; this->m23 = m23;
			this->m30 = m30; this->m31 = m31; this->m32 = m32; this->m33 = m33;
		}

		FORCE_INLINE Matrix4<Type, Align>& SetZero();
		FORCE_INLINE Matrix4<Type, Align>& SetIdentity();
		FORCE_INLINE Matrix4<Type, Align>& SetTranslation(const Vector3<Type, Align>& translation);
		FORCE_INLINE Matrix4<Type, Align>& SetRotation(const Vector3<Type, Align>& axis, const Float32 angle);
		FORCE_INLINE Matrix4<Type, Align>& SetRotation(const Quaternion& quat);
		FORCE_INLINE Matrix4<Type, Align>& SetScale(const Vector3<Type, Align>& scale);

		FORCE_INLINE Matrix4<Type, Align>& SetViewLH(const Vector3<Type, Align>& left, const Vector3<Type, Align>& up,
			const Vector3<Type, Align>& forward, const Vector3<Type, Align>& position);
		FORCE_INLINE Matrix4<Type, Align>& SetViewRH(const Vector3<Type, Align>& left, const Vector3<Type, Align>& up,
			const Vector3<Type, Align>& forward, const Vector3<Type, Align>& position);
		FORCE_INLINE Matrix4<Type, Align>& SetLookAtLH(const Vector3<Type, Align>& position,
			const Vector3<Type, Align>& target, const Vector3<Type, Align>& up);
		FORCE_INLINE Matrix4<Type, Align>& SetLookAtRH(const Vector3<Type, Align>& position,
			const Vector3<Type, Align>& target, const Vector3<Type, Align>& up);
		FORCE_INLINE Matrix4<Type, Align>& SetPerspectiveLH(const Float32 fov, const Float32 aspect, Float32 zNear, Float32 zFar);
		FORCE_INLINE Matrix4<Type, Align>& SetPerspectiveRH(const Float32 fov, const Float32 aspect, Float32 zNear, Float32 zFar);

		FORCE_INLINE Matrix4<Type, Align> Transpose();

		FORCE_INLINE Matrix4<Type, Align> operator+(const Matrix4<Type, Align>& mat4) const;
		FORCE_INLINE Matrix4<Type, Align>& operator+=(const Matrix4<Type, Align>& mat4);

		FORCE_INLINE Matrix4<Type, Align> operator-(const Matrix4<Type, Align>& mat4) const;
		FORCE_INLINE Matrix4<Type, Align>& operator-=(const Matrix4<Type, Align>& mat4);

		FORCE_INLINE Matrix4<Type, Align> operator*(const Matrix4<Type, Align>& mat4) const;
		FORCE_INLINE Matrix4<Type, Align>& operator*=(const Matrix4<Type, Align>& mat4);

		FORCE_INLINE Matrix4<Type, Align>& operator=(const Matrix4<Type, Align>& mat4);

		FORCE_INLINE Vector3<Type, Align> GetForward() const;
		FORCE_INLINE Vector3<Type, Align> GetUp() const;
		FORCE_INLINE Vector3<Type, Align> GetLeft() const;
	};

	//////////////////////////////////////////////////////////////
	// Vector4 [Any] Non-SIMD
	//////////////////////////////////////////////////////////////

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align>& Matrix4<Type, Align>::SetZero()
	{
		memset(elements, 0, 16 * sizeof(Type));
		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align>&  Matrix4<Type, Align>::SetIdentity()
	{
		this->SetZero();
		this->m00 = 1.0f;
		this->m11 = 1.0f;
		this->m22 = 1.0f;
		this->m33 = 1.0f;
		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align>& Matrix4<Type, Align>::SetTranslation(const Vector3<Type, Align>& translation)
	{
		this->SetIdentity();
		this->m30 = translation.x;
		this->m31 = translation.y;
		this->m32 = translation.z;
		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align>& Matrix4<Type, Align>::SetRotation(const Vector3<Type, Align>& axis, const Float32 angleRad)
	{
		const Float32 cosAngle = cosf(angleRad);
		const Float32 sinAngle = sinf(angleRad);
		const Float32 invCosAngle = 1 - cosAngle;

		const Float32 x = axis.x;
		const Float32 y = axis.y;
		const Float32 z = axis.z;
		const Float32 x2 = x * x;
		const Float32 yz = y * y;
		const Float32 z2 = z * z;

		this->m00 = cosAngle + (x2 * invCosAngle);
		this->m01 = (x * y * invCosAngle) - (z * sinAngle);
		this->m02 = (x * z * invCosAngle) - (y * sinAngle);
		this->m03 = 0.0f;
		this->m10 = (y * x * invCosAngle) + (z * sinAngle);
		this->m11 = cosAngle + (y2 * invCosAngle);
		this->m12 = (y * z * invCosAngle) - (x * sinAngle);
		this->m13 = 0.0f;
		this->m20 = (z * x * invCosAngle) - (y * sinAngle);
		this->m21 = (z * y * invCosAngle) + (x * sinAngle);
		this->m22 = cosAngle + (z2 * invCosAngle);
		this->m23 = 0.0f;
		this->m30 = 0.0f;
		this->m31 = 0.0f;
		this->m32 = 0.0f;
		this->m33 = 1.0f;

		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align>& Matrix4<Type, Align>::SetRotation(const Quaternion& quat)
	{
		const Float32 invLength = 1.0f / quat.Length();

		const Float32 qx = quat.x * invLength;
		const Float32 qy = quat.y * invLength;
		const Float32 qz = quat.z * invLength;
		const Float32 qw = quat.w * invLength;

		this->m00 = 1.0f - 2.0f * qy * qy - 2.0f * qz * qz;
		this->m01 = 2.0f * qx * qy - 2.0f * qz * qw;
		this->m02 = 2.0f * qx * qz + 2.0f * qy * qw;
		this->m03 = 0.0f;

		this->m10 = 2.0f * qx * qy + 2.0f * qz * qw;
		this->m11 = 1.0f - 2.0f * qx * qx - 2.0f * qz * qz;
		this->m12 = 2.0f * qy * qz - 2.0f * qx * qw;
		this->m13 = 0.0f;

		this->m20 = 2.0f * qx * qz - 2.0f * qy * qw;
		this->m21 = 2.0f * qy * qz + 2.0f * qx * qw;
		this->m22 = 1.0f - 2.0f * qx * qx - 2.0f * qy * qy;
		this->m23 = 0.0f;

		this->m30 = 0.0f;
		this->m31 = 0.0f;
		this->m32 = 0.0f;
		this->m33 = 1.0f;

		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align>& Matrix4<Type, Align>::SetScale(const Vector3<Type, Align>& scale)
	{
		this->SetZero();
		this->m00 = scale.x;
		this->m11 = scale.y;
		this->m22 = scale.z;
		this->m33 = 1.0f;
		return *this;
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align>& Matrix4<Type, Align>::SetViewLH(const Vector3<Type, Align>& left, 
		const Vector3<Type, Align>& up, const Vector3<Type, Align>& forward, const Vector3<Type, Align>& position)
	{
		m00 = left.x;    m10 = left.y;    m20 = left.z;    m30 = -position.x;
		m01 = up.x;      m11 = up.y;      m21 = up.z;      m31 = -position.y;
		m02 = forward.x; m12 = forward.y; m22 = forward.z; m32 = -position.z;
		m03 = 0.0f;      m13 = 0.0f;      m23 = 0.0f;      m33 = 1.0f;
		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align>& Matrix4<Type, Align>::SetViewRH(const Vector3<Type, Align>& left,
		const Vector3<Type, Align>& up, const Vector3<Type, Align>& forward, const Vector3<Type, Align>& position)
	{
		m00 = left.x;    m10 = left.y;    m20 = left.z;    m30 = position.x;
		m01 = up.x;      m11 = up.y;      m21 = up.z;      m31 = position.y;
		m02 = forward.x; m12 = forward.y; m22 = forward.z; m32 = position.z;
		m03 = 0.0f;      m13 = 0.0f;      m23 = 0.0f;      m33 = 1.0f;
		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align>& Matrix4<Type, Align>::SetLookAtLH(const Vector3<Type, Align>& eye,
		const Vector3<Type, Align>& target, const Vector3<Type, Align>& up)
	{
		Vector3<Type, Align> zAxis = (target - eye).Normalized();
		Vector3<Type, Align> xAxis = (up.Cross(zAxis)).Normalized();
		Vector3<Type, Align> yAxis = zAxis.Cross(xAxis);
		Vector3<Type, Align> position(xAxis.Dot(eye), yAxis.Dot(eye), zAxis.Dot(eye));
		return this->SetViewLH(xAxis, yAxis, zAxis, position);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align>& Matrix4<Type, Align>::SetLookAtRH(const Vector3<Type, Align>& eye,
		const Vector3<Type, Align>& target, const Vector3<Type, Align>& up)
	{
		Vector3<Type, Align> zAxis = (target - eye).Normalized();
		Vector3<Type, Align> xAxis = (up.Cross(zAxis)).Normalized();
		Vector3<Type, Align> yAxis = zAxis.Cross(xAxis);
		Vector3<Type, Align> position(xAxis.Dot(eye), yAxis.Dot(eye), zAxis.Dot(eye));
		return this->SetViewRH(xAxis, yAxis, zAxis, position);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align>& Matrix4<Type, Align>::SetPerspectiveLH(const Float32 fov, const Float32 aspect, Float32 zNear, Float32 zFar)
	{
		const Float32 fovRad = fov * (3.14159f / 180.0f);
		const Float32 fovY = 1.0f / tanf(fovRad / 2.0f);

		this->SetZero();
		this->m00 = fovY / aspect;
		this->m11 = fovY;
		this->m22 = zFar / (zFar - zNear);
		this->m23 = 1.0f;
		this->m32 = -zNear * (zFar / (zFar - zNear));
		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align>& Matrix4<Type, Align>::SetPerspectiveRH(const Float32 fov, const Float32 aspect, Float32 zNear, Float32 zFar)
	{
		const Float32 fovRad = fov * (3.14159f / 180.0f);
		const Float32 fovY = 1.0f / tanf(fovRad / 2.0f);

		this->SetZero();
		this->m00 = fovY / aspect;
		this->m11 = fovY;
		this->m22 = zFar / (zFar - zNear);
		this->m23 = -1.0f;
		this->m32 = zNear * (zFar / (zFar - zNear));
		return *this;
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align> Matrix4<Type, Align>::Transpose()
	{
		Matrix4<Type, Align> mat4;
		mat4.m00 = this->m00; mat4.m01 = this->m10; mat4.m02 = this->m20; mat4.m03 = this->m30;
		mat4.m10 = this->m01; mat4.m11 = this->m11; mat4.m12 = this->m21; mat4.m13 = this->m31;
		mat4.m20 = this->m02; mat4.m21 = this->m12; mat4.m22 = this->m22; mat4.m23 = this->m32;
		mat4.m30 = this->m03; mat4.m31 = this->m13; mat4.m32 = this->m23; mat4.m33 = this->m33;
		return mat4;
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align> Matrix4<Type, Align>::operator+(const Matrix4<Type, Align>& mat4) const
	{
		Vector4<Type, Align> vec1 = vectors[0] + mat4.vectors[0];
		Vector4<Type, Align> vec2 = vectors[1] + mat4.vectors[1];
		Vector4<Type, Align> vec3 = vectors[2] + mat4.vectors[2];
		Vector4<Type, Align> vec4 = vectors[3] + mat4.vectors[3];
		return Matrix4<Type, Align>(vec1, vec2, vec3, vec4);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align>& Matrix4<Type, Align>::operator+=(const Matrix4<Type, Align>& mat4)
	{
		vectors[0] = vectors[0] + mat4.vectors[0];
		vectors[1] = vectors[1] + mat4.vectors[1];
		vectors[2] = vectors[2] + mat4.vectors[2];
		vectors[3] = vectors[3] + mat4.vectors[3];
		return *this;
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align> Matrix4<Type, Align>::operator-(const Matrix4<Type, Align>& mat4) const
	{
		Vector4<Type, Align> vec1 = vectors[0] - mat4.vectors[0];
		Vector4<Type, Align> vec2 = vectors[1] - mat4.vectors[1];
		Vector4<Type, Align> vec3 = vectors[2] - mat4.vectors[2];
		Vector4<Type, Align> vec4 = vectors[3] - mat4.vectors[3];
		return Matrix4<Type, Align>(vec1, vec2, vec3, vec4);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align>& Matrix4<Type, Align>::operator-=(const Matrix4<Type, Align>& mat4)
	{
		vectors[0] = vectors[0] - mat4.vectors[0];
		vectors[1] = vectors[1] - mat4.vectors[1];
		vectors[2] = vectors[2] - mat4.vectors[2];
		vectors[3] = vectors[3] - mat4.vectors[3];
		return *this;
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align> Matrix4<Type, Align>::operator*(const Matrix4<Type, Align>& mat4) const
	{
		// Not yet implemented
		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align>& Matrix4<Type, Align>::operator*=(const Matrix4<Type, Align>& mat4)
	{
		// Not yet implemented
		return *this;
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Matrix4<Type, Align>& Matrix4<Type, Align>::operator=(const Matrix4<Type, Align>& mat4)
	{
		MemCopy(mat4.elements, this->elements, 16 * sizeof(Type));
		return *this;
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align> Matrix4<Type, Align>::GetForward() const
	{
		return Vector3<Type, Align>(m02, m12, m22);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align> Matrix4<Type, Align>::GetUp() const
	{
		return Vector3<Type, Align>(m01, m11, m21);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align> Matrix4<Type, Align>::GetLeft() const
	{
		return Vector3<Type, Align>(m00, m10, m20);
	}

	//////////////////////////////////////////////////////////////
	// Mat4 [Float] SIMD
	//////////////////////////////////////////////////////////////

	// -----------------------------------------------------------

	template<>
	FORCE_INLINE Mat4f Mat4f::operator*(const Mat4f& mat4) const
	{
		Mat4f res;

		const XMMVEC vec1 = XMMLoadFloat4((Float32*)&mat4.vectors[0]);
		const XMMVEC vec2 = XMMLoadFloat4((Float32*)&mat4.vectors[1]);
		const XMMVEC vec3 = XMMLoadFloat4((Float32*)&mat4.vectors[2]);
		const XMMVEC vec4 = XMMLoadFloat4((Float32*)&mat4.vectors[3]);

		Float32* increment = (Float32*)&this->vectors[0];
		Float32* result = (Float32*)&res.vectors[0];

		for (UInt32 i = 0; i < 4; ++i, increment += 4, result += 4) {
			XMMVEC vecX = XMMFillFloat4(increment[0]);
			XMMVEC vecY = XMMFillFloat4(increment[1]);
			XMMVEC vecZ = XMMFillFloat4(increment[2]);
			XMMVEC vecW = XMMFillFloat4(increment[3]);

			XMMVEC X = XMMMulFloat4(vecX, vec1);
			XMMVEC Y = XMMMulFloat4(vecY, vec2);
			XMMVEC Z = XMMMulFloat4(vecZ, vec3);
			XMMVEC W = XMMMulFloat4(vecW, vec4);

			XMMVEC R = XMMAddFloat4(X, XMMAddFloat4(Y , XMMAddFloat4(Z, W)));
			XMMStoreFloat4(result, R);
		}

		return res;
	}

	template<>
	FORCE_INLINE Mat4f& Mat4f::operator*=(const Mat4f& mat4)
	{
		*this = *this * mat4;
		return *this;
	}

	// -----------------------------------------------------------
}