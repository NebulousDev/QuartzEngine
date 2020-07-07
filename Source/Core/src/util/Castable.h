#pragma once

namespace Quartz
{
	class QUARTZ_API Castable
	{
	public:
		template<typename AsType>
		constexpr AsType CastAs()
		{
			return static_cast<AsType>(*this);
		}

		template<typename AsType>
		constexpr AsType CastAs() const
		{
			return static_cast<AsType>(*this);
		}
	};
}