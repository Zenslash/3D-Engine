#pragma once

#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include <array>
#include "ZenMath.h"

class Plane
{
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int div_x, int div_y)
	{
		namespace dx = DirectX;
		assert(div_x >= 1);
		assert(div_y >= 1);

		const float width = 2.0f;
		const float height = 2.0f;
		const int nVertices_x = div_x + 1;
		const int nVertices_y = div_y + 1;
		std::vector<V> vertices(nVertices_x * nVertices_y);
		//Generate vertices
		{
			const float center_x = width / 2.0f;
			const float center_y = height / 2.0f;
			const float nBlocks_x = width / float(div_x);
			const float nBlocks_y = width / float(div_y);
			const auto bottomLeft = dx::XMVectorSet(-center_x, -center_y, 0.0f, 0.0f);

			for (int i = 0, index = 0; i < nVertices_y; i++)
			{
				const float y = float(i) * nBlocks_y;
				for (int j = 0; j < nVertices_x; j++, index++)
				{
					const auto v = dx::XMVectorAdd
					(
						bottomLeft, dx::XMVectorSet(float(j) * nBlocks_x, y, 0.0f, 0.0f)
					);
					dx::XMStoreFloat3(&vertices[index].pos, v);
				}
			}
		}

		std::vector<unsigned short> indices;
		//TODO Optimize memory alloc
		indices.reserve(Square(div_x * div_y) * 6);
		{
			const auto vertexPos2index = [nVertices_x](size_t x, size_t y)
			{
				return (unsigned short)(y * nVertices_x + x);
			};
			for (size_t y = 0; y < div_y; y++)
			{
				for (size_t x = 0; x < div_x; x++)
				{
					const std::array<unsigned short, 4> indexArray =
					{ vertexPos2index(x, y), vertexPos2index(x + 1, y),
						vertexPos2index(x, y + 1), vertexPos2index(x + 1, y + 1)
					};
					indices.push_back(indexArray[0]);
					indices.push_back(indexArray[2]);
					indices.push_back(indexArray[1]);
					indices.push_back(indexArray[1]);
					indices.push_back(indexArray[2]);
					indices.push_back(indexArray[3]);
				}
			}
		}

		return { std::move(vertices), std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(1, 1);
	}
};