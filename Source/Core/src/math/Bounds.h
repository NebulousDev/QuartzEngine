#pragma once

#include "../Common.h"
#include "Defines.h"
#include "Point.h"

namespace Quartz
{
	//////////////////////////////////////////////////////////////
	// Bounds2 Definition
	//////////////////////////////////////////////////////////////

	template<typename ValueType>
	struct Bounds2
	{
		Point2<ValueType> start;
		Point2<ValueType> end;

		constexpr Bounds2() {};

		constexpr Bounds2(ValueType sx, ValueType sy, ValueType ex, ValueType ey)
			: start(sx, sy), end(ex, ey) { }

		constexpr Bounds2(Point2<ValueType> start, Point2<ValueType> end)
			: start(start), end(end) { }

		constexpr Bounds2(const Bounds2& bounds2)
			: start(bounds2.start), end(bounds2.end) { }

		ValueType Width() const
		{
			return end.x - start.x;
		}

		ValueType Height() const
		{
			return end.y - start.y;
		}

		template<typename OtherValueType>
		operator Bounds2<OtherValueType>() const
		{
			Bounds2<OtherValueType> result;
			result.start = (Point2<OtherValueType>)start;
			result.end = (Point2<OtherValueType>)end;
			return result;
		}
	};

	typedef Bounds2<UInt32> Bounds2u;
	typedef Bounds2<Int32> Bounds2i;
	typedef Bounds2<Float32> Bounds2f;

	//////////////////////////////////////////////////////////////
	// Bounds3 Definition
	//////////////////////////////////////////////////////////////

	template<typename ValueType>
	struct Bounds3
	{
		Point3<ValueType> start;
		Point3<ValueType> end;

		constexpr Bounds3() {};

		constexpr Bounds3(ValueType sx, ValueType sy, ValueType sz, ValueType ex, ValueType ey, ValueType ez)
			: start(sx, sy, sz), end(ex, ey.ez) { }

		constexpr Bounds3(Point3<ValueType> start, Point3<ValueType> end)
			: start(start), end(end) { }

		constexpr Bounds3(const Bounds3& bounds3)
			: start(bounds3.start), end(bounds3.end) { }

		ValueType Width() const
		{
			return end.x - start.x;
		}

		ValueType Height() const
		{
			return end.y - start.y;
		}

		ValueType Depth() const
		{
			return end.z - start.z;
		}

		template<typename OtherValueType>
		operator Bounds3<OtherValueType>() const
		{
			Bounds3<OtherValueType> result;
			result.start = (Point3<OtherValueType>)start;
			result.end = (Point3<OtherValueType>)end;
			return result;
		}
	};

	typedef Bounds3<UInt32> Bounds3u;
	typedef Bounds3<Int32> Bounds3i;
	typedef Bounds3<Float32> Bounds3f;

	//////////////////////////////////////////////////////////////
	// Util Functions
	//////////////////////////////////////////////////////////////

	template<typename ValueType>
	Bool8 PointInBounds(Point2<ValueType> point, Bounds2<ValueType> bounds)
	{
		return point.x > bounds.start.x && point.x < bounds.end.x && 
			point.y > bounds.start.y && point.y < bounds.end.y;
	}

	template<typename ValueType>
	Bool8 PointInBounds(Point3<ValueType> point, Bounds3<ValueType> bounds)
	{
		return point.x > bounds.start.x && point.x < bounds.end.x &&
			point.y > bounds.start.y && point.y < bounds.end.y &&
			point.z > bounds.start.z && point.z < bounds.end.z;
	}
}