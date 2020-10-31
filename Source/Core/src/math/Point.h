#pragma once

#include "../Common.h"
#include "Defines.h"

namespace Quartz
{
	//////////////////////////////////////////////////////////////
	// Point2 Definition
	//////////////////////////////////////////////////////////////

	template<typename ValueType>
	struct Point2
	{
		using Point2Type = Point2<ValueType>;

		ValueType x, y;

		constexpr Point2()
			: x(0), y(0) { }

		constexpr Point2(ValueType x, ValueType y)
			: x(x), y(y) { }

		constexpr Point2(const Point2Type& point2)
			: x(point2.x), y(point2.y) { }

		Point2Type operator+(const Point2Type& point2)
		{
			return Point2Type(x + point2.x, y + point2.y);
		}

		Point2Type operator-(const Point2Type& point2)
		{
			return Point2Type(x - point2.x, y - point2.y);
		}

		Point2Type operator*(const Point2Type& point2)
		{
			return Point2Type(x * point2.x, y * point2.y);
		}

		Point2Type operator/(const Point2Type& point2)
		{
			return Point2Type(x / point2.x, y / point2.y);
		}

		template<typename OtherValueType>
		operator Point2<OtherValueType>() const
		{
			Point2<OtherValueType> result;
			result.x = (OtherValueType)x;
			result.y = (OtherValueType)y;
			return result;
		}
	};

	typedef Point2<Int32> Point2i;
	typedef Point2<UInt32> Point2u;
	typedef Point2<Float32> Point2f;

	//////////////////////////////////////////////////////////////
	// Point3 Definition
	//////////////////////////////////////////////////////////////

	template<typename ValueType>
	struct Point3
	{
		using Point3Type = Point3<ValueType>;

		ValueType x, y, z;

		constexpr Point3()
			: x(0), y(0) { }

		constexpr Point3(ValueType x, ValueType y)
			: x(x), y(y) { }

		constexpr Point3(const Point3Type& point3)
			: x(point3.x), y(point3.y) { }

		Point3Type operator+(const Point3Type& point3)
		{
			return Point3Type(x + point3.x, y + point3.y, z + point3.z);
		}

		Point3Type operator-(const Point3Type& point3)
		{
			return Point3Type(x - point3.x, y - point3.y, z - point3.z);
		}

		Point3Type operator*(const Point3Type& point3)
		{
			return Point3Type(x * point3.x, y * point3.y, z * point3.z);
		}

		Point3Type operator/(const Point3Type& point3)
		{
			return Point3Type(x / point3.x, y / point3.y, z / point3.z);
		}

		template<typename OtherValueType>
		operator Point3<OtherValueType>() const
		{
			Point3<OtherValueType> result;
			result.x = (OtherValueType)x;
			result.y = (OtherValueType)y;
			result.z = (OtherValueType)z;
			return result;
		}
	};

	typedef Point3<Int32> Point3i;
	typedef Point3<UInt32> Point3u;
	typedef Point3<Float32> Point3f;
}