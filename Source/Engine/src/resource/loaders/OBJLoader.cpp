#include "OBJLoader.h"

#include "util\StringParser.h"
#include "util\Array.h"
#include "util\Map.h"
#include "math\Math.h"

namespace Quartz
{
	Bool8 LoadOBJ(const String& data, OBJModel* pObjModel)
	{
		OBJModel& objModel = *pObjModel;

		StringParser parser(data);

		while (!parser.IsEnd())
		{
			parser.AdvanceWhitespace();

			if (parser.PeakChar() == '\n' || parser.PeakChar() == '#')
			{
				parser.ReadLine();
				continue;
			}

			SubString token = parser.ReadToChar(' ');

			// Comment
			if (token == "#")
			{
				parser.ReadLine();
				continue;
			}

			// Object
			else if (token == "o")
			{
				parser.ReadLine();
				continue;
			}

			// Vertex position
			else if (token == "v")
			{
				parser.AdvanceWhitespace();

				Float32 x = parser.ReadFloat();
				Float32 y = parser.ReadFloat();
				Float32 z = parser.ReadFloat();

				objModel.positions.PushBack({ x, y, z });

				parser.ReadLine();
			}

			// Vertex normal
			else if (token == "vn")
			{
				parser.AdvanceWhitespace();

				Float32 x = parser.ReadFloat();
				Float32 y = parser.ReadFloat();
				Float32 z = parser.ReadFloat();

				objModel.normals.PushBack({ x, y, z });

				parser.ReadLine();
			}

			// Vertex texture coordinates
			else if (token == "vt")
			{
				parser.AdvanceWhitespace();

				Float32 x = parser.ReadFloat();
				Float32 y = parser.ReadFloat();

				objModel.texCoords.PushBack({ x, y });

				parser.ReadLine();
			}

			// Face
			else if (token == "f")
			{
				parser.AdvanceWhitespace();

				StringParser faceLine(parser.ReadLine());

				OBJIndex index{};
				UInt32 indexCount = 0;

				Array<OBJIndex>& indices = objModel.indices;

				while (!faceLine.IsEnd())
				{
					if (indexCount > 2)
					{
						OBJIndex index0 = indices[indices.Size() - 3];
						OBJIndex index1 = indices[indices.Size() - 1];
						indices.PushBack(Move(index0));
						indices.PushBack(Move(index1));
						indexCount += 2;
					}

					faceLine.AdvanceWhitespace();

					index.positionIdx = faceLine.ReadInt();

					if (faceLine.PeakChar() == '/')
					{
						faceLine.AdvanceChar();

						// No texCoord
						if (faceLine.PeakChar() == '/')
						{
							faceLine.AdvanceChar();
							index.normalIdx = faceLine.ReadInt();
						}
						else
						{
							index.texCoordIdx = faceLine.ReadInt();
						}

						if (faceLine.PeakChar() == '/')
						{
							faceLine.AdvanceChar();
							index.normalIdx = faceLine.ReadInt();
						}

						indices.PushBack(index);
						++indexCount;

						faceLine.AdvanceWhitespace();
					}
				}
			}

			// Everything else
			else
			{
				parser.ReadLine();
				continue;
			}
		}

		return true;
	}
}