#pragma once
#include "../Types.h"
#include "../debug/Debug.h"

#include "Vector.h"

namespace Quartz
{
	__declspec(align(16)) struct Quaternion
	{
		union
		{
			struct
			{
				Float32 x;
				Float32 y;
				Float32 z;
				Float32 w;
			};
			
			Float32 values[4];
		};

		FORCE_INLINE Quaternion();
		FORCE_INLINE Quaternion(Float32 data[]);
		FORCE_INLINE Quaternion(const Vec3f& vec, const Float32 scalar);
		FORCE_INLINE Quaternion(const Float32 x, const Float32 y, const Float32 z, const Float32 w);

		FORCE_INLINE Quaternion& SetIdentity();
		FORCE_INLINE Quaternion& SetAngleAxis(const Vec3f& axis, const Float32 angle);
		FORCE_INLINE Quaternion& SetEuler(const Vec3f& rotation);

		FORCE_INLINE Quaternion Conjugate() const;
		FORCE_INLINE Quaternion Inverse() const;

		FORCE_INLINE Float32 Dot(const Quaternion& quat) const;

		FORCE_INLINE Float32 LengthSquared() const;
		FORCE_INLINE Float32 Length() const;

		FORCE_INLINE Quaternion& Normalize();
		FORCE_INLINE Quaternion Normalized() const;

		FORCE_INLINE Quaternion operator+(const Quaternion& quat) const;
		FORCE_INLINE Quaternion& operator+=(const Quaternion& quat);

		FORCE_INLINE Quaternion operator-(const Quaternion& quat) const;
		FORCE_INLINE Quaternion& operator-=(const Quaternion& quat);

		FORCE_INLINE Quaternion operator*(const Quaternion& quat) const;
		FORCE_INLINE Quaternion operator*(const Float32 scalar) const;
		FORCE_INLINE Quaternion& operator*=(const Quaternion& quat);
		FORCE_INLINE Quaternion& operator*=(const Float32 scalar);

		FORCE_INLINE Vec3f operator*(const Vec3f& quat) const;

		FORCE_INLINE Quaternion operator/(const Float32 scalar) const;
		FORCE_INLINE Quaternion& operator/=(const Float32 scalar);

		FORCE_INLINE Quaternion& operator=(const Quaternion& quat);

		FORCE_INLINE Float32& operator[](const UInt32&& idx);
	};

	FORCE_INLINE Quartz::Quaternion::Quaternion() :
		x(0), y(0), z(0), w(0) { }

	FORCE_INLINE Quaternion::Quaternion(Float32 values[]) :
		x(values[0]), y(values[1]), z(values[2]), w(values[3]) { }

	FORCE_INLINE Quaternion::Quaternion(const Vec3f& vec, const Float32 scalar) :
		x(vec.x), y(vec.y), z(vec.z), w(scalar) { }

	FORCE_INLINE Quaternion::Quaternion(const Float32 x, const Float32 y, const Float32 z, const Float32 w) :
		x(x), y(y), z(z), w(w) { }

	FORCE_INLINE Quaternion& Quaternion::SetIdentity()
	{
		return *this = { 0.0f, 0.0f, 0.0f, 1.0f };
	}

	FORCE_INLINE Quaternion& Quaternion::SetAngleAxis(const Vec3f& axis, const Float32 angle)
	{
		const Float32 sinHalfAngle = sinf(angle * 0.5f);
		const Float32 cosHalfAngle = cosf(angle * 0.5f);

		this->x = axis.x * sinHalfAngle;
		this->y = axis.y * sinHalfAngle;
		this->z = axis.z * sinHalfAngle;
		this->w = cosHalfAngle;

		return *this;
	}

	FORCE_INLINE Quaternion& Quaternion::SetEuler(const Vec3f& rotation)
	{
		const Float32 cx = cosf(x * 0.5f);
		const Float32 cy = cosf(y * 0.5f);
		const Float32 cz = cosf(z * 0.5f);
		const Float32 sx = sinf(x * 0.5f);
		const Float32 sy = sinf(y * 0.5f);
		const Float32 sz = sinf(z * 0.5f);

		this->x = cx * sy * sz + cy * cz * sx;
		this->y = cx * cz * sy - cy * sx * sz;
		this->z = cx * cy * sz - cz * sx * sy;
		this->w = sx * sy * sz + cx * cy * cz;

		return *this;
	}

	FORCE_INLINE Quaternion Quaternion::Conjugate() const
	{
		return Quaternion(-x, -y, -z, w);
	}

	FORCE_INLINE Quaternion Quaternion::Inverse() const
	{
		return this->Conjugate() / LengthSquared();
	}

	FORCE_INLINE Float32 Quaternion::Dot(const Quaternion& quat) const
	{
		return (x * quat.x) + (y * quat.y) + (z * quat.z) + (w * quat.w);
	}

	FORCE_INLINE Float32 Quaternion::LengthSquared() const
	{
		return (x * x) + (y * y) + (z * z) + (w * w);
	}

	FORCE_INLINE Float32 Quaternion::Length() const
	{
#if USE_FAST_SQRT
		Float32 lenSquared = LengthSquared();
		return lenSquared * FastInvsereSquare(lenSquared);
#else
		return sqrt(LengthSquared());
#endif
	}

	FORCE_INLINE Quaternion & Quaternion::Normalize()
	{
#if USE_FAST_SQRT
		Float32 invSquare = FastInvsereSquare(LengthSquared());
#else
		Float32 invSquare = 1.0f / sqrt(LengthSquared());
#endif
		*this *= invSquare;
		return *this;
	}

	FORCE_INLINE Quaternion Quaternion::Normalized() const
	{
		Quaternion result = *this;
#if USE_FAST_SQRT
		Float32 invSquare = FastInvsereSquare(LengthSquared());
#else
		Float32 invSquare = 1.0f / sqrt(LengthSquared());
#endif
		result *= invSquare;
		return result;
	}

	FORCE_INLINE Quaternion Quaternion::operator+(const Quaternion& quat) const
	{
		return Quaternion(x + quat.x, y + quat.y, z + quat.z, w + quat.w);
	}

	FORCE_INLINE Quaternion& Quaternion::operator+=(const Quaternion& quat)
	{
		return *this = *this + quat;
	}

	FORCE_INLINE Quaternion Quaternion::operator-(const Quaternion& quat) const
	{
		return Quaternion(x - quat.x, y - quat.y, z - quat.z, w - quat.w);
	}

	FORCE_INLINE Quaternion& Quaternion::operator-=(const Quaternion& quat)
	{
		return *this = *this - quat;
	}

	FORCE_INLINE Quaternion Quaternion::operator*(const Quaternion& quat) const
	{
		Quaternion result;
		result.x = w * quat.x + x * quat.w + y * quat.z - z * quat.y;
		result.y = w * quat.y + y * quat.w + z * quat.x - x * quat.z;
		result.z = w * quat.z + z * quat.w + x * quat.y - y * quat.x;
		result.w = w * quat.w - x * quat.x - y * quat.y - z * quat.z;
		return result;
	}

	FORCE_INLINE Quaternion Quaternion::operator*(const Float32 scalar) const
	{
		Quaternion result;
		result.x = x * scalar;
		result.y = y * scalar;
		result.z = z * scalar;
		result.w = w * scalar;
		return result;
	}

	FORCE_INLINE Quaternion& Quaternion::operator*=(const Quaternion& quat)
	{
		return *this = *this * quat;
	}

	FORCE_INLINE Quaternion& Quaternion::operator*=(const Float32 scalar)
	{
		return *this = *this * scalar;
	}

	FORCE_INLINE Vec3f Quaternion::operator*(const Vec3f& vec) const
	{
		Vec3f result = Vec3f(x, y, z);
		return result * result.Dot(vec) * 2.0f
			+ const_cast<Vec3f&>(vec) * (w * w - result.Dot(result))
			+ result.Cross(vec) * (2.0f * w);
	}

	FORCE_INLINE Quaternion Quaternion::operator/(const Float32 scalar) const
	{
		Quaternion result;
		result.x = x / scalar;
		result.y = y / scalar;
		result.z = z / scalar;
		result.w = w / scalar;
		return result;
	}

	FORCE_INLINE Quaternion & Quaternion::operator/=(const Float32 scalar)
	{
		return *this = *this / scalar;
	}

	FORCE_INLINE Quaternion& Quaternion::operator=(const Quaternion& quat)
	{
		x = quat.x;
		y = quat.y;
		z = quat.z;
		w = quat.w;
		return *this;
	}

	FORCE_INLINE Float32& Quaternion::operator[](const UInt32&& idx)
	{
		DEBUG_ASSERT(idx < 4 && "Index out of bounds!");
		return values[idx];
	}

}