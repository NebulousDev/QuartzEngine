#pragma once
#include "../Common.h"
#include "../util/Intrinsics.h"

#include <math.h>

#ifndef USE_FAST_SQRT
#define USE_FAST_SQRT 1
#endif

namespace Quartz
{
	//////////////////////////////////////////////////////////////
	// Vector Typedefs
	//////////////////////////////////////////////////////////////

	template<typename Type, UInt8 Align = alignof(Float32)> struct Vector4;
	template<typename Type, UInt8 Align = alignof(Float32)> struct Vector3;
	template<typename Type, UInt8 Align = alignof(Float32)> struct Vector2;

	typedef Vector4<Float32> Vec4f;
	typedef Vector3<Float32> Vec3f;
	typedef Vector2<Float32> Vec2f;

	typedef Vector4<Int32> Vec4i;
	typedef Vector3<Int32> Vec3i;
	typedef Vector2<Int32> Vec2i;

	typedef Vector4<UInt32> Vec4u;
	typedef Vector3<UInt32> Vec3u;
	typedef Vector2<UInt32> Vec2u;

	typedef Vector4<Float32> Color4f;
	typedef Vector3<Float32> Color3f;

	typedef Vector4<Float32, 16> Vec4fA;
	typedef Vector3<Float32, 16> Vec3fA;
	typedef Vector2<Float32, 16> Vec2fA;

	typedef Vector4<Int32, 16> Vec4iA;
	typedef Vector3<Int32, 16> Vec3iA;
	typedef Vector2<Int32, 16> Vec2iA;

	typedef Vector4<UInt32, 16> Vec4uA;
	typedef Vector3<UInt32, 16> Vec3uA;
	typedef Vector2<UInt32, 16> Vec2uA;

	typedef Vector4<Float32, 16> Color4fA;
	typedef Vector3<Float32, 16> Color3fA;

	//////////////////////////////////////////////////////////////
	// Vector4
	//////////////////////////////////////////////////////////////

	template<typename Type, UInt8 Align>
	struct Vector4
	{
		static constexpr UInt32 SIZE = 4;

		__declspec(align(Align)) union
		{
			struct {
				Type x;
				Type y;
				Type z;
				Type w;
			};

			struct
			{
				Type r;
				Type g;
				Type b;
				Type a;
			};

			Type values[4];
		};

		Vector4() : 
			x(0), y(0), z(0), w(0) { }

		Vector4(const Vector4<Type, Align>& vec4) :
			x(vec4.x), y(vec4.y), z(vec4.z), w(vec4.w) { }

		Vector4(const Vector3<Type, Align>& vec3, const Type& w = 0) :
			x(vec3.x), y(vec3.y), z(vec3.z), w(w) { }

		Vector4(const Vector2<Type, Align>& vec2, const Type& z = 0, const Type& w = 0) :
			x(vec2.x), y(vec2.y), z(z), w(w) { }

		Vector4(const Type arr[4]) :
			x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3]) { }

		Vector4(const Type& x, const Type& y, const Type& z, const Type& w) :
			x(x), y(y), z(z), w(w) { }

		FORCE_INLINE Vector4<Type, Align> operator+(const Vector4<Type, Align>& vec4) const;
		FORCE_INLINE Vector4<Type, Align> operator+(const Vector3<Type, Align>& vec3) const;
		FORCE_INLINE Vector4<Type, Align> operator+(const Vector2<Type, Align>& vec2) const;
		FORCE_INLINE Vector4<Type, Align>& operator+=(const Vector4<Type, Align>& vec4);
		FORCE_INLINE Vector4<Type, Align>& operator+=(const Vector3<Type, Align>& vec3);
		FORCE_INLINE Vector4<Type, Align>& operator+=(const Vector2<Type, Align>& vec2);

		FORCE_INLINE Vector4<Type, Align> operator-(const Vector4<Type, Align>& vec4) const;
		FORCE_INLINE Vector4<Type, Align> operator-(const Vector2<Type, Align>& vec2) const;
		FORCE_INLINE Vector4<Type, Align> operator-(const Vector3<Type, Align>& vec3) const;
		FORCE_INLINE Vector4<Type, Align>& operator-=(const Vector4<Type, Align>& vec4);
		FORCE_INLINE Vector4<Type, Align>& operator-=(const Vector3<Type, Align>& vec3);
		FORCE_INLINE Vector4<Type, Align>& operator-=(const Vector2<Type, Align>& vec2);

		FORCE_INLINE Vector4<Type, Align> operator*(const Type& scalar) const;
		FORCE_INLINE Vector4<Type, Align>& operator*=(const Type& scalar);

		FORCE_INLINE Vector4<Type, Align> operator/(const Type& scalar) const;
		FORCE_INLINE Vector4<Type, Align>& operator/=(const Type& scalar);

		FORCE_INLINE Vector4<Type, Align> operator-() const;

		FORCE_INLINE Vector4<Type, Align>& operator=(const Vector4<Type, Align>& vec4);

		FORCE_INLINE Type& operator[](const UInt32&& idx);

		FORCE_INLINE Type Dot(const Vector4<Type, Align>& vec4) const;

		FORCE_INLINE Float32 LengthSquared() const;
		FORCE_INLINE Float32 Length() const;
		FORCE_INLINE Vector4<Type, Align>& Normalize();
		FORCE_INLINE Vector4<Type, Align> Normalized() const;

		FORCE_INLINE Vector3<Type, Align> xyz() const;
		FORCE_INLINE Vector3<Type, Align> xy() const;
		FORCE_INLINE Vector2<Type, Align> rgb() const;
		FORCE_INLINE Vector2<Type, Align> rg() const;
	};

	//////////////////////////////////////////////////////////////
	// Vector3
	//////////////////////////////////////////////////////////////

	template<typename Type, UInt8 Align>
	struct Vector3
	{
		static constexpr UInt32 SIZE = 3;

		__declspec(align(Align)) union
		{
			struct {
				Type x;
				Type y;
				Type z;
			};

			struct
			{
				Type r;
				Type g;
				Type b;
			};
		
			Type values[3];
		};

		Vector3() :
			x(0), y(0), z(0) { }

		Vector3(const Vector3<Type, Align>& vec3) :
			x(vec3.x), y(vec3.y), z(vec3.z) { }

		Vector3(const Vector2<Type, Align>& vec2, const Type& z = 0) :
			x(vec2.x), y(vec2.y), z(z) { }

		Vector3(const Type arr[3]) :
			x(arr[0]), y(arr[1]), z(arr[2]) { }

		Vector3(const Type& x, const Type& y, const Type& z) :
			x(x), y(y), z(z) { }

		FORCE_INLINE Vector3<Type, Align> operator+(const Vector3<Type, Align>& vec3) const;
		FORCE_INLINE Vector3<Type, Align> operator+(const Vector2<Type, Align>& vec2) const;
		FORCE_INLINE Vector3<Type, Align>& operator+=(const Vector3<Type, Align>& vec3);
		FORCE_INLINE Vector3<Type, Align>& operator+=(const Vector2<Type, Align>& vec2);
		FORCE_INLINE Vector3<Type, Align> operator-(const Vector3<Type, Align>& vec3) const;
		FORCE_INLINE Vector3<Type, Align> operator-(const Vector2<Type, Align>& vec2) const;
		FORCE_INLINE Vector3<Type, Align>& operator-=(const Vector3<Type, Align>& vec3);
		FORCE_INLINE Vector3<Type, Align>& operator-=(const Vector2<Type, Align>& vec2);

		FORCE_INLINE Vector3<Type, Align> operator*(const Type& scalar);
		FORCE_INLINE Vector3<Type, Align>& operator*=(const Type& scalar);

		FORCE_INLINE Vector3<Type, Align> operator/(const Type& scalar);
		FORCE_INLINE Vector3<Type, Align>& operator/=(const Type& scalar);

		FORCE_INLINE Vector3<Type, Align> operator-() const;

		FORCE_INLINE Type& operator[](const UInt32&& idx);

		FORCE_INLINE Type Dot(const Vector3<Type, Align>& vec3) const;
		FORCE_INLINE Vector3<Type, Align> Cross(const Vector3<Type, Align>& vec3) const;

		FORCE_INLINE Float32 LengthSquared() const;
		FORCE_INLINE Float32 Length() const;
		FORCE_INLINE Vector3<Type, Align>& Normalize();
		FORCE_INLINE Vector3<Type, Align> Normalized() const;

		FORCE_INLINE Vector2<Type, Align> xy() const;
		FORCE_INLINE Vector2<Type, Align> rg() const;
	};

	//////////////////////////////////////////////////////////////
	// Vector2
	//////////////////////////////////////////////////////////////

	template<typename Type, UInt8 Align>
	struct Vector2
	{
		static constexpr UInt32 SIZE = 2;

		__declspec(align(Align)) union
		{
			struct {
				Type x;
				Type y;
			};

			struct
			{
				Type r;
				Type g;
			};

			Type values[2];
		};

		Vector2() :
			x(0), y(0) { }

		Vector2(const Vector2<Type, Align>& vec2) :
			x(vec2.x), y(vec2.y) { }

		Vector2(const Type arr[2]) :
			x(arr[0]), y(arr[1]) { }

		Vector2(const Type& x, const Type& y) :
			x(x), y(y) { }

		FORCE_INLINE Vector2<Type, Align> operator+(const Vector2<Type, Align>& vec2) const;
		FORCE_INLINE Vector2<Type, Align>& operator+=(const Vector2<Type, Align>& vec2);

		FORCE_INLINE Vector2<Type, Align> operator-(const Vector2<Type, Align>& vec2) const;
		FORCE_INLINE Vector2<Type, Align>& operator-=(const Vector2<Type, Align>& vec2);

		FORCE_INLINE Vector2<Type, Align> operator*(const Type& scalar);
		FORCE_INLINE Vector2<Type, Align>& operator*=(const Type& scalar);

		FORCE_INLINE Vector2<Type, Align> operator/(const Type& scalar);
		FORCE_INLINE Vector2<Type, Align>& operator/=(const Type& scalar);

		FORCE_INLINE Type& operator[](const UInt32&& idx);

		FORCE_INLINE Type Dot(const Vector2<Type, Align>& vec2) const;

		FORCE_INLINE Float32 LengthSquared();
		FORCE_INLINE Float32 Length();
		FORCE_INLINE Vector2<Type, Align>& Normalize();
		FORCE_INLINE Vector2<Type, Align> Normalized() const;
	};

	//////////////////////////////////////////////////////////////
	// Vector4 [Float] SIMD Overrides
	//////////////////////////////////////////////////////////////

	// -----------------------------------------------------------

#ifndef NO_INTRINSICS

	template<>
	FORCE_INLINE Vec4f Vec4f::operator+(const Vec4f& vec4f) const
	{
		XMMVEC xVec1 = XMMLoadFloat4(reinterpret_cast<const Float32*>(this));
		XMMVEC xVec2 = XMMLoadFloat4(reinterpret_cast<const Float32*>(&vec4f));
		xVec1 = XMMAddFloat4(xVec1, xVec2);
		return *reinterpret_cast<Vec4f*>(&xVec1);
	}

	template<>
	FORCE_INLINE Vec4f Vec4f::operator+(const Vec3f& vec3f) const
	{
		XMMVEC xVec1 = XMMLoadFloat4(reinterpret_cast<const Float32*>(this));
		XMMVEC xVec2 = XMMSetFloat4(vec3f.x, vec3f.y, vec3f.z, 0.0f);
		xVec1 = XMMAddFloat4(xVec1, xVec2);
		return *reinterpret_cast<Vec4f*>(&xVec1);
	}

	template<>
	FORCE_INLINE Vec4f Vec4f::operator+(const Vec2f& vec2f) const
	{
		XMMVEC xVec1 = XMMLoadFloat4(reinterpret_cast<const Float32*>(this));
		XMMVEC xVec2 = XMMSetFloat4(vec2f.x, vec2f.y, 0.0f, 0.0f);
		xVec1 = XMMAddFloat4(xVec1, xVec2);
		return *reinterpret_cast<Vec4f*>(&xVec1);
	}

	template<>
	FORCE_INLINE Vec4f& Vec4f::operator+=(const Vec4f& vec4f)
	{
		*this = (*this + vec4f);
		return *this;
	}

	template<>
	FORCE_INLINE Vec4f& Vec4f::operator+=(const Vec3f& vec3f)
	{
		*this = (*this + vec3f);
		return *this;
	}

	template<>
	FORCE_INLINE Vec4f& Vec4f::operator+=(const Vec2f& vec2f)
	{
		*this = (*this + vec2f);
		return *this;
	}

	// -----------------------------------------------------------

	template<>
	FORCE_INLINE Vec4f Vec4f::operator-(const Vec4f& vec4f) const
	{
		XMMVEC xVec1 = XMMLoadFloat4(reinterpret_cast<const Float32*>(this));
		XMMVEC xVec2 = XMMLoadFloat4(reinterpret_cast<const Float32*>(&vec4f));
		xVec1 = XMMSubFloat4(xVec1, xVec2);
		return *reinterpret_cast<Vec4f*>(&xVec1);
	}

	template<>
	FORCE_INLINE Vec4f Vec4f::operator-(const Vec3f& vec3f) const
	{
		XMMVEC xVec1 = XMMLoadFloat4(reinterpret_cast<const Float32*>(this));
		XMMVEC xVec2 = XMMSetFloat4(vec3f.x, vec3f.y, vec3f.z, 0.0f);
		xVec1 = XMMSubFloat4(xVec1, xVec2);
		return *reinterpret_cast<Vec4f*>(&xVec1);
	}

	template<>
	FORCE_INLINE Vec4f Vec4f::operator-(const Vec2f& vec2f) const
	{
		XMMVEC xVec1 = XMMLoadFloat4(reinterpret_cast<const Float32*>(this));
		XMMVEC xVec2 = XMMSetFloat4(vec2f.x, vec2f.y, 0.0f, 0.0f);
		xVec1 = XMMSubFloat4(xVec1, xVec2);
		return *reinterpret_cast<Vec4f*>(&xVec1);
	}

	template<>
	FORCE_INLINE Vec4f& Vec4f::operator-=(const Vec4f& vec4f)
	{
		*this = (*this - vec4f);
		return *this;
	}

	template<>
	FORCE_INLINE Vec4f& Vec4f::operator-=(const Vec3f& vec3f)
	{
		*this = (*this - vec3f);
		return *this;
	}

	template<>
	FORCE_INLINE Vec4f& Vec4f::operator-=(const Vec2f& vec2f)
	{
		*this = (*this - vec2f);
		return *this;
	}

	// -----------------------------------------------------------

	template<>
	FORCE_INLINE Vec4f Vec4f::operator*(const Float32& scalar) const
	{
		XMMVEC xVec1 = XMMLoadFloat4(reinterpret_cast<const Float32*>(this));
		XMMVEC xVec2 = XMMSetScalar(scalar);
		xVec1 = XMMMulFloat4(xVec1, xVec2);
		return *reinterpret_cast<Vec4f*>(&xVec1);
	}

	template<>
	FORCE_INLINE Vec4f& Vec4f::operator*=(const Float32& scalar)
	{
		*this = (*this * scalar);
		return *this;
	}

	// -----------------------------------------------------------

	template<>
	FORCE_INLINE Vec4f Vec4f::operator/(const Float32& scalar) const
	{
		XMMVEC xVec1 = XMMLoadFloat4(reinterpret_cast<const Float32*>(this));
		XMMVEC xVec2 = XMMSetScalar(scalar);
		xVec1 = XMMDivFloat4(xVec1, xVec2);
		return *reinterpret_cast<Vec4f*>(&xVec1);
	}

	template<>
	FORCE_INLINE Vec4f& Vec4f::operator/=(const Float32& scalar)
	{
		*this = (*this / scalar);
		return *this;
	}

	// -----------------------------------------------------------

	template<>
	FORCE_INLINE Float32 Vec4f::Dot(const Vec4f& vec4f) const
	{
		XMMVEC xVec1 = XMMLoadFloat4(reinterpret_cast<const Float32*>(this));
		XMMVEC xVec2 = XMMLoadFloat4(reinterpret_cast<const Float32*>(&vec4f));
		xVec1 = XMMDotFloat4(xVec1, xVec2);
		return XMMGetFloat(xVec1, 0);
	}

	// -----------------------------------------------------------

	template<>
	FORCE_INLINE Float32 Vec4f::LengthSquared() const
	{
		XMMVEC xVec = XMMLoadFloat4(reinterpret_cast<const Float32*>(this));
		xVec = XMMHorizAddFloat4(xVec);
		xVec = XMMMulFloatScalar(xVec, xVec);
		return XMMGetFloat(xVec, 0);
	}

	// -----------------------------------------------------------

	//////////////////////////////////////////////////////////////
	// Vector4 [Aligned Float] SIMD Overrides
	//////////////////////////////////////////////////////////////

	// -----------------------------------------------------------

	template<>
	FORCE_INLINE Vec4fA Vec4fA::operator+(const Vec4fA& vec4f) const
	{
		XMMVEC xVec1 = *(XMMVEC*)this;
		XMMVEC xVec2 = *(XMMVEC*)&vec4f;
		xVec1 = XMMAddFloat4(xVec1, xVec2);
		return *(Vec4fA*)&xVec1;
	}

	template<>
	FORCE_INLINE Vec4fA Vec4fA::operator+(const Vec3fA& vec3f) const
	{
		XMMVEC xVec1 = *(XMMVEC*)this;
		XMMVEC xVec2 = *(XMMVEC*)&vec3f;
		xVec1 = XMMAddFloat4(xVec1, xVec2);
		return *(Vec4fA*)&xVec1;
	}

	template<>
	FORCE_INLINE Vec4fA Vec4fA::operator+(const Vec2fA& vec2f) const
	{
		XMMVEC xVec1 = *(XMMVEC*)this;
		XMMVEC xVec2 = *(XMMVEC*)&vec2f;
		xVec1 = XMMAddFloat4(xVec1, xVec2);
		return *(Vec4fA*)&xVec1;
	}

	template<>
	FORCE_INLINE Vec4fA& Vec4fA::operator+=(const Vec4fA& vec4f)
	{
		*this = (*this + vec4f);
		return *this;
	}

	template<>
	FORCE_INLINE Vec4fA& Vec4fA::operator+=(const Vec3fA& vec3f)
	{
		*this = (*this + vec3f);
		return *this;
	}

	template<>
	FORCE_INLINE Vec4fA& Vec4fA::operator+=(const Vec2fA& vec2f)
	{
		*this = (*this + vec2f);
		return *this;
	}

	// -----------------------------------------------------------

	template<>
	FORCE_INLINE Vec4fA Vec4fA::operator-(const Vec4fA& vec4f) const
	{
		XMMVEC xVec1 = *(XMMVEC*)this;
		XMMVEC xVec2 = *(XMMVEC*)&vec4f;
		xVec1 = XMMSubFloat4(xVec1, xVec2);
		return *(Vec4fA*)&xVec1;
	}

	template<>
	FORCE_INLINE Vec4fA Vec4fA::operator-(const Vec3fA& vec3f) const
	{
		XMMVEC xVec1 = *(XMMVEC*)this;
		XMMVEC xVec2 = *(XMMVEC*)&vec3f;
		xVec1 = XMMSubFloat4(xVec1, xVec2);
		return *(Vec4fA*)&xVec1;
	}

	template<>
	FORCE_INLINE Vec4fA Vec4fA::operator-(const Vec2fA& vec2f) const
	{
		XMMVEC xVec1 = *(XMMVEC*)this;
		XMMVEC xVec2 = *(XMMVEC*)&vec2f;
		xVec1 = XMMSubFloat4(xVec1, xVec2);
		return *(Vec4fA*)&xVec1;
	}

	template<>
	FORCE_INLINE Vec4fA& Vec4fA::operator-=(const Vec4fA& vec4f)
	{
		*this = (*this - vec4f);
		return *this;
	}

	template<>
	FORCE_INLINE Vec4fA& Vec4fA::operator-=(const Vec3fA& vec3f)
	{
		*this = (*this - vec3f);
		return *this;
	}

	template<>
	FORCE_INLINE Vec4fA& Vec4fA::operator-=(const Vec2fA& vec2f)
	{
		*this = (*this - vec2f);
		return *this;
	}

	// -----------------------------------------------------------

	template<>
	FORCE_INLINE Vec4fA Vec4fA::operator*(const Float32& scalar) const
	{
		XMMVEC xVec1 = *(XMMVEC*)this;
		XMMVEC xVec2 = XMMSetScalar(scalar);
		xVec1 = XMMMulFloat4(xVec1, xVec2);
		return *(Vec4fA*)&xVec1;
	}

	template<>
	FORCE_INLINE Vec4fA& Vec4fA::operator*=(const Float32& scalar)
	{
		*this = (*this * scalar);
		return *this;
	}

	// -----------------------------------------------------------

	template<>
	FORCE_INLINE Vec4fA Vec4fA::operator/(const Float32& scalar) const
	{
		XMMVEC xVec1 = *(XMMVEC*)this;
		XMMVEC xVec2 = XMMSetScalar(scalar);
		xVec1 = XMMDivFloat4(xVec1, xVec2);
		return *(Vec4fA*)&xVec1;
	}

	template<>
	FORCE_INLINE Vec4fA& Vec4fA::operator/=(const Float32& scalar)
	{
		*this = (*this / scalar);
		return *this;
	}

	// -----------------------------------------------------------

	template<>
	FORCE_INLINE Vec4fA& Vec4fA::operator=(const Vec4fA& vec4)
	{
		*(XMMVEC*)this = *(XMMVEC*)&vec4;
		return *this;
	}

	// -----------------------------------------------------------

	template<>
	FORCE_INLINE Float32 Vec4fA::Dot(const Vec4fA& vec4f) const
	{
		XMMVEC xVec1 = *(XMMVEC*)this;
		XMMVEC xVec2 = *(XMMVEC*)&vec4f;
		xVec1 = XMMDotFloat4(xVec1, xVec2);
		return XMMGetFloat(xVec1, 0);
	}

	// -----------------------------------------------------------

	template<>
	FORCE_INLINE Float32 Vec4fA::LengthSquared() const
	{
		XMMVEC xVec = *(XMMVEC*)this;
		xVec = XMMHorizAddFloat4(xVec);
		xVec = XMMMulFloatScalar(xVec, xVec);
		return XMMGetFloat(xVec, 0);
	}

#endif // !NO_INTRINSICS

	// -----------------------------------------------------------

	//////////////////////////////////////////////////////////////
	// Vector4 [Any] Non-SIMD
	//////////////////////////////////////////////////////////////

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align> Vector4<Type, Align>::operator+(const Vector4<Type, Align>& vec4) const
	{
		return Vector4<Type, Align>(x + vec4.x, y + vec4.y, z + vec4.z, w + vec4.w);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align> Vector4<Type, Align>::operator+(const Vector3<Type, Align>& vec3) const
	{
		return Vector4<Type, Align>(x + vec3.x, y + vec3.y, z + vec3.z, w);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align> Vector4<Type, Align>::operator+(const Vector2<Type, Align>& vec2) const
	{
		return Vector4<Type, Align>(x + vec2.x, y + vec2.y, z, w);
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align>& Vector4<Type, Align>::operator+=(const Vector4<Type, Align>& vec4)
	{
		x += vec4.x;
		y += vec4.y;
		z += vec4.z;
		w += vec4.w;

		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align>& Vector4<Type, Align>::operator+=(const Vector3<Type, Align>& vec3)
	{
		x += vec3.x;
		y += vec3.y;
		z += vec3.z;

		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align>& Vector4<Type, Align>::operator+=(const Vector2<Type, Align>& vec2)
	{
		x += vec2.x;
		y += vec2.y;

		return *this;
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align> Vector4<Type, Align>::operator-(const Vector4<Type, Align>& vec4) const
	{
		return Vector4<Type, Align>(x - vec4.x, y - vec4.y, z - vec4.z, w - vec4.w);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align> Vector4<Type, Align>::operator-(const Vector3<Type, Align>& vec3) const
	{
		return Vector4<Type, Align>(x - vec3.x, y - vec3.y, z - vec3.z, w);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align> Vector4<Type, Align>::operator-(const Vector2<Type, Align>& vec2) const
	{
		return Vector4<Type, Align>(x - vec2.x, y - vec2.y, z, w);
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align>& Vector4<Type, Align>::operator-=(const Vector4<Type, Align>& vec4)
	{
		x -= vec4.x;
		y -= vec4.y;
		z -= vec4.z;
		w -= vec4.w;

		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align>& Vector4<Type, Align>::operator-=(const Vector3<Type, Align>& vec3)
	{
		x -= vec3.x;
		y -= vec3.y;
		z -= vec3.z;

		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align>& Vector4<Type, Align>::operator-=(const Vector2<Type, Align>& vec2)
	{
		x -= vec2.x;
		y -= vec2.y;

		return *this;
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align> Vector4<Type, Align>::operator*(const Type& scalar) const
	{
		return Vector4<Type, Align>(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align>& Vector4<Type, Align>::operator*=(const Type& scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;

		return *this;
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align> Vector4<Type, Align>::operator/(const Type& scalar) const
	{
		return Vector4<Type, Align>(x / scalar, y / scalar, z / scalar, w / scalar);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align>& Vector4<Type, Align>::operator/=(const Type& scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;

		return *this;
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align> Vector4<Type, Align>::operator-() const
	{
		return Vector4<Type, Align>(-x, -y, -z, -w);
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align>& Vector4<Type, Align>::operator=(const Vector4<Type, Align>& vec4)
	{
		x = vec4.x;
		y = vec4.y;
		z = vec4.z;
		w = vec4.w;
		return *this;
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Type& Vector4<Type, Align>::operator[](const UInt32&& idx)
	{
#ifdef QUARTZ_DEBUG
		assert(idx < SIZE && "Index out of bounds!");
#endif
		return values[idx];
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Type Vector4<Type, Align>::Dot(const Vector4<Type, Align>& vec4) const
	{
		return (x * vec4.x) + (y * vec4.y) + (z * vec4.z) + (w * vec4.w);
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Float32 Vector4<Type, Align>::LengthSquared() const
	{
		return (x * x) + (y * y) + (z * z) + (w * w);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Float32 Vector4<Type, Align>::Length() const
	{
#if USE_FAST_SQRT
		Float32 lenSquared = LengthSquared();
		return lenSquared * FastInvsereSquare(lenSquared);
#else
		return sqrt(LengthSquared());
#endif
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align>& Vector4<Type, Align>::Normalize()
	{
#if USE_FAST_SQRT
		Float32 invSquare = FastInvsereSquare(LengthSquared());
#else
		Float32 invSquare = 1.0f / sqrt(LengthSquared());
#endif
		*this *= invSquare;
		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector4<Type, Align> Vector4<Type, Align>::Normalized() const
	{
		Vector4<Type, Align> result = *this;
#if USE_FAST_SQRT
		Float32 invSquare = FastInvsereSquare(LengthSquared());
#else
		Float32 invSquare = 1.0f / sqrt(LengthSquared());
#endif
		result *= invSquare;
		return result;
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align> Vector4<Type, Align>::xyz() const
	{
		return Vector3<Type, Align>(x, y, z);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align> Vector4<Type, Align>::xy() const
	{
		return Vec2<Type, Align>(x, y);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector2<Type, Align> Vector4<Type, Align>::rgb() const
	{
		return xyz();
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector2<Type, Align> Vector4<Type, Align>::rg() const
	{
		return xy();
	}

	// -----------------------------------------------------------

	//////////////////////////////////////////////////////////////
	// Vector3 [Any] Non-SIMD
	//////////////////////////////////////////////////////////////

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align> Vector3<Type, Align>::operator+(const Vector2<Type, Align>& vec2) const
	{
		return Vector3<Type, Align>(x + vec2.x, y + vec2.y, z);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align> Vector3<Type, Align>::operator+(const Vector3<Type, Align>& vec3) const
	{
		return Vector3<Type, Align>(x + vec3.x, y + vec3.y, z + vec3.z);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align>& Vector3<Type, Align>::operator+=(const Vector2<Type, Align>& vec2)
	{
		x += vec2.x;
		y += vec2.y;

		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align>& Vector3<Type, Align>::operator+=(const Vector3<Type, Align>& vec3)
	{
		x += vec3.x;
		y += vec3.y;
		z += vec3.z;

		return *this;
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align> Vector3<Type, Align>::operator-(const Vector2<Type, Align>& vec2) const
	{
		return Vector3<Type, Align>(x - vec2.x, y - vec2.y, z);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align> Vector3<Type, Align>::operator-(const Vector3<Type, Align>& vec3) const
	{
		return Vector3<Type, Align>(x - vec3.x, y - vec3.y, z - vec3.z);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align>& Vector3<Type, Align>::operator-=(const Vector2<Type, Align>& vec2)
	{
		x -= vec2.x;
		y -= vec2.y;

		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align>& Vector3<Type, Align>::operator-=(const Vector3<Type, Align>& vec3)
	{
		x -= vec3.x;
		y -= vec3.y;
		z -= vec3.z;

		return *this;
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align> Vector3<Type, Align>::operator*(const Type& scalar)
	{
		return Vector3<Type, Align>(x * scalar, y * scalar, z * scalar);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align>& Vector3<Type, Align>::operator*=(const Type& scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align> Vector3<Type, Align>::operator/(const Type& scalar)
	{
		return Vector3<Type, Align>(x / scalar, y / scalar, z / scalar);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align>& Vector3<Type, Align>::operator/=(const Type& scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;

		return *this;
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align> Vector3<Type, Align>::operator-() const
	{
		return Vector3<Type, Align>(-x, -y, -z);
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Type& Vector3<Type, Align>::operator[](const UInt32&& idx)
	{
#ifdef QUARTZ_DEBUG
		assert(idx < SIZE && "Index out of bounds!");
#endif
		return values[idx];
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Type Vector3<Type, Align>::Dot(const Vector3<Type, Align>& vec3) const
	{
		return (x * vec3.x) + (y * vec3.y) + (z * vec3.z);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align> Vector3<Type, Align>::Cross(const Vector3<Type, Align>& vec3) const
	{
		return Vector3<Type, Align>
		(
			(y * vec3.z) - (z * vec3.y),
			(z * vec3.x) - (x * vec3.z),
			(x * vec3.y) - (y * vec3.x)
		);
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Float32 Vector3<Type, Align>::LengthSquared() const
	{
		return (x * x) + (y * y) + (z * z);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Float32 Vector3<Type, Align>::Length() const
	{
#if USE_FAST_SQRT
		Float32 lenSquared = LengthSquared();
		return lenSquared * FastInvsereSquare(lenSquared);
#else
		return sqrt(LengthSquared());
#endif
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align>& Vector3<Type, Align>::Normalize()
	{
#if USE_FAST_SQRT
		Float32 invSquare = FastInvsereSquare(LengthSquared());
#else
		Float32 invSquare = 1.0f / sqrt(LengthSquared());
#endif
		*this *= invSquare;
		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector3<Type, Align> Vector3<Type, Align>::Normalized() const
	{
		Vector3<Type, Align> result = *this;
#if USE_FAST_SQRT
		Float32 invSquare = FastInvsereSquare(LengthSquared());
#else
		Float32 invSquare = 1.0f / sqrt(LengthSquared());
#endif
		result *= invSquare;
		return result;
	}

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector2<Type, Align> Vector3<Type, Align>::xy() const
	{
		return Vector2<Type, Align>(x, y);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector2<Type, Align> Vector3<Type, Align>::rg() const
	{
		return xy();
	}

	// -----------------------------------------------------------

	//////////////////////////////////////////////////////////////
	// Vector2 [Any] Non-SIMD
	//////////////////////////////////////////////////////////////

	// -----------------------------------------------------------

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector2<Type, Align> Vector2<Type, Align>::operator+(const Vector2<Type, Align>& vec2) const
	{
		return Vector2<Type, Align>(x + vec2.x, y + vec2.y, z);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector2<Type, Align>& Vector2<Type, Align>::operator+=(const Vector2<Type, Align>& vec2)
	{
		x += vec2.x;
		y += vec2.y;

		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector2<Type, Align> Vector2<Type, Align>::operator-(const Vector2<Type, Align>& vec2) const
	{
		return Vector2<Type, Align>(x - vec2.x, y - vec2.y, z);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector2<Type, Align>& Vector2<Type, Align>::operator-=(const Vector2<Type, Align>& vec2)
	{
		x -= vec2.x;
		y -= vec2.y;

		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector2<Type, Align> Vector2<Type, Align>::operator*(const Type& scalar)
	{
		return Vector2<Type, Align>(x * scalar, y * scalar);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector2<Type, Align>& Vector2<Type, Align>::operator*=(const Type& scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector2<Type, Align> Vector2<Type, Align>::operator/(const Type& scalar)
	{
		return Vector2<Type, Align>(x / scalar, y / scalar);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector2<Type, Align>& Vector2<Type, Align>::operator/=(const Type& scalar)
	{
		x /= scalar;
		y /= scalar;

		return *this;
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Type& Vector2<Type, Align>::operator[](const UInt32&& idx)
	{
#ifdef QUARTZ_DEBUG
		assert(idx < SIZE && "Index out of bounds!");
#endif
		return values[idx];
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Type Vector2<Type, Align>::Dot(const Vector2<Type, Align>& vec2) const
	{
		return (x * vec2.x) + (y * vec2.y);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Float32 Vector2<Type, Align>::LengthSquared()
	{
		return (x * x) + (y * y);
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Float32 Vector2<Type, Align>::Length()
	{
#if USE_FAST_SQRT
		Float32 lenSquared = LengthSquared();
		return lenSquared * FastInvsereSquare(lenSquared);
#else
		return sqrt(LengthSquared());
#endif
	}

	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector2<Type, Align>& Vector2<Type, Align>::Normalize()
	{
#if USE_FAST_SQRT
		Float32 invSquare = FastInvsereSquare(LengthSquared());
#else
		Float32 invSquare = 1.0f / sqrt(LengthSquared());
#endif
		*this *= invSquare;
		return *this;
	}
	
	template<typename Type, UInt8 Align>
	FORCE_INLINE Vector2<Type, Align> Vector2<Type, Align>::Normalized() const
	{
		Vector2<Type, Align> result = *this;
#if USE_FAST_SQRT
		Float32 invSquare = FastInvsereSquare(LengthSquared());
#else
		Float32 invSquare = 1.0f / sqrt(LengthSquared());
#endif
		result *= invSquare;
		return result;
	}
}