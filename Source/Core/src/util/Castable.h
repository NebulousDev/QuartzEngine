#pragma once

namespace Quartz
{
	class QUARTZ_API Castable
	{
	public:
		template<typename AsType>
		AsType CastAs()
		{
			return static_cast<AsType>(*this);
		}
	};
}