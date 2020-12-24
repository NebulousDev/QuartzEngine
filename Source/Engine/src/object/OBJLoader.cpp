#include "OBJLoader.h"

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

	struct OBJModel
	{
		Array<Vector3> positions;
		Array<Vector3> normals;
		Array<Vector2> texCoords;
		Array<OBJIndex> indices;
	};

	template<>
	UInt32 Hash<OBJIndex>(const OBJIndex& index)
	{
		return Hash<UInt64>(index.Hash());
	}

	OBJModel ParseOBJ(const String& data)
	{
		OBJModel objModel{};

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

				objModel.positions.PushBack({ x, y, -z });

				parser.ReadLine();
			}

			// Vertex normal
			else if (token == "vn")
			{
				parser.AdvanceWhitespace();

				Float32 x = parser.ReadFloat();
				Float32 y = parser.ReadFloat();
				Float32 z = parser.ReadFloat();

				objModel.normals.PushBack({ x, y, -z });

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

		return objModel;
	}

	Model LoadOBJ(const String& data)
	{
		OBJModel objModel = ParseOBJ(data);

		VertexFormat vertexFormat =
		{
			{
				{ 0, VERTEX_ATTRIBUTE_POSITION, VERTEX_TYPE_FLOAT3 },
				{ 1, VERTEX_ATTRIBUTE_NORMAL, VERTEX_TYPE_FLOAT3 },
				{ 2, VERTEX_ATTRIBUTE_TEXCOORD, VERTEX_TYPE_FLOAT2 },
			},
			3
		};

		VertexData vertexData{};
		vertexData.format = vertexFormat;

		IndexData indexData{};
		indexData.format = INDEX_FORMAT_INT32;

		UInt32 currentIndex = 0;
		Map<OBJIndex, UInt32> indexMap{};
		indexMap.Reserve(objModel.indices.Size());

		for (const OBJIndex& index : objModel.indices)
		{
			UInt32* pIdx = indexMap.Get(index);

			if (pIdx != nullptr)
			{
				indexData.buffer.Push(*pIdx);
			}
			else
			{
				Vector3& position = objModel.positions[index.positionIdx - 1];
				vertexData.buffer.Push(position);

				if (objModel.normals.Size() > 0 && index.normalIdx != 0)
				{
					Vector3& normal = objModel.normals[index.normalIdx - 1];
					vertexData.buffer.Push(normal);
				}
				else
				{
					// Generate normals
					Vector3 normal = Vector3(0, 0, 0);
					vertexData.buffer.Push(normal);
				}

				if (objModel.texCoords.Size() > 0 && index.texCoordIdx != 0)
				{
					Vector2& texCoord = objModel.texCoords[index.texCoordIdx - 1];
					vertexData.buffer.Push(texCoord);
				}
				else
				{
					// Generate texCoords
					Vector2 texCoord = Vector2(0, 0);
					vertexData.buffer.Push(texCoord);
				}

				indexMap.Put(index, currentIndex);
				indexData.buffer.Push(currentIndex++);
			}
		}

		Model model;
		model.vertexData = vertexData;
		model.indexData = indexData;

		return model;
	}
}