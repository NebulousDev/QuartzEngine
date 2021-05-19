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

	struct OBJMaterial
	{

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
				{ 2, VERTEX_ATTRIBUTE_BINORMAL, VERTEX_TYPE_FLOAT3 },
				{ 3, VERTEX_ATTRIBUTE_TANGENT, VERTEX_TYPE_FLOAT3 },
				{ 4, VERTEX_ATTRIBUTE_TEXCOORD, VERTEX_TYPE_FLOAT2 },
			},
			5
		};

		VertexData vertexData{};
		vertexData.format = vertexFormat;

		IndexData indexData{};
		indexData.format = INDEX_FORMAT_INT32;

		UInt32 currentIndex = 0;
		Map<OBJIndex, UInt32> indexMap{};
		indexMap.Reserve(objModel.indices.Size());

		for (UInt32 i = 0; i < (objModel.indices.Size() - 2); i += 3)
		{
			const OBJIndex& index0 = objModel.indices[i];
			const OBJIndex& index1 = objModel.indices[i + 1];
			const OBJIndex& index2 = objModel.indices[i + 2];

			Vector3 binormal = Vector3(0, 0, 0);
			Vector3 tangent = Vector3(0, 0, 0);

			if (objModel.texCoords.Size() > 0 && 
				index0.texCoordIdx != 0 && 
				index1.texCoordIdx != 0 && 
				index2.texCoordIdx != 0)
			{
				// Generate tangents/binormals *per-face*

				Vector3 deltaPos1 = objModel.positions[index1.positionIdx - 1] - objModel.positions[index0.positionIdx - 1];
				Vector3 deltaPos2 = objModel.positions[index2.positionIdx - 1] - objModel.positions[index1.positionIdx - 1];
				Vector2 deltaTex1 = objModel.texCoords[index1.texCoordIdx - 1] - objModel.texCoords[index0.texCoordIdx - 1];
				Vector2 deltaTex2 = objModel.texCoords[index2.texCoordIdx - 1] - objModel.texCoords[index1.texCoordIdx - 1];

				float f = 1.0f / (deltaTex1.x * deltaTex2.y - deltaTex2.x * deltaTex1.y);

				tangent.x = f * (deltaTex2.y * deltaPos1.x - deltaTex1.y * deltaPos2.x);
				tangent.y = f * (deltaTex2.y * deltaPos1.y - deltaTex1.y * deltaPos2.y);
				tangent.z = f * (deltaTex2.y * deltaPos1.z - deltaTex1.y * deltaPos2.z);

				binormal.x = f * (-deltaTex2.x * deltaPos1.x + deltaTex1.x * deltaPos2.x);
				binormal.y = f * (-deltaTex2.x * deltaPos1.y + deltaTex1.x * deltaPos2.y);
				binormal.z = f * (-deltaTex2.x * deltaPos1.z + deltaTex1.x * deltaPos2.z);
			}

			for (UInt32 j = 0; j < 3; j++)
			{
				const OBJIndex& index = objModel.indices[i + j];
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

					vertexData.buffer.Push(binormal);
					vertexData.buffer.Push(tangent);

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
		}

		Model model;
		model.vertexData = vertexData;
		model.indexData = indexData;

		return model;
	}
}