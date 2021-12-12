#pragma once

#include "util/Array.h"
#include "math/Math.h"
#include "util/String.h"

#include "../Resource.h"

namespace Quartz
{
	typedef UInt64 OBJIndexHash;

	struct OBJIndex
	{
		UInt32 positionIdx;
		UInt32 normalIdx;
		UInt32 texCoordIdx;

		OBJIndexHash Hash() const
		{
			OBJIndexHash hash;
			hash = (positionIdx & 0x001fffff);
			hash |= (normalIdx & 0x001fffff) >> 21;
			hash |= texCoordIdx >> 21;

			return hash;
		}

		Bool8 operator==(const OBJIndex& index) const
		{
			return positionIdx == index.positionIdx &&
				normalIdx == index.normalIdx &&
				texCoordIdx == index.texCoordIdx;
		}

		Bool8 operator!=(const OBJIndex& index) const
		{
			return !operator==(index);
		}
	};

	struct OBJMaterial
	{

	};

	struct OBJModel : public Resource<OBJModel>
	{
		Array<Vector3> positions;
		Array<Vector3> normals;
		Array<Vector2> texCoords;
		Array<OBJIndex> indices;
	};
}