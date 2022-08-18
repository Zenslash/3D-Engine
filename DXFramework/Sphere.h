#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include "ZenMath.h"

class Sphere
{
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int longDiv, int latDiv)
	{
		namespace dx = DirectX;
		
		assert(latDiv >= 3);
		assert(longDiv >= 3);

		const float radius = 1.0f;
		const auto base = dx::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
		const float latAngle = PI / latDiv;
		const float longAngle = 2.0f * PI / longDiv;

		//Generate vertices without North and South pole
		std::vector<V> vertices;
		for (int i = 1; i < latDiv; i++)
		{
			const auto lat = dx::XMVector3Transform
			(
				base,
				dx::XMMatrixRotationX(latAngle * i)
			);
			for (int j = 0; j < longDiv; j++)
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform
				(
					lat,
					dx::XMMatrixRotationZ(longAngle * j)
				);
				dx::XMStoreFloat3(&vertices.back().pos, v);
			}
		}

		//Add North,South poles
		const auto northPole = (unsigned short)vertices.size();
		vertices.emplace_back();
		dx::XMStoreFloat3(&vertices.back().pos, base);

		const auto southPole = (unsigned short)vertices.size();
		vertices.emplace_back();
		dx::XMStoreFloat3(&vertices.back().pos, dx::XMVectorNegate(base));

		const auto calcIdx = [latDiv, longDiv](unsigned short iLat, unsigned short iLong)
		{ return iLat * longDiv + iLong; };
		std::vector<unsigned short> indices;
		for (unsigned short i = 0; i < latDiv - 2; i++)
		{
			for (unsigned short j = 0; j < longDiv - 1; j++)
			{
				indices.push_back(calcIdx(i, j));
				indices.push_back(calcIdx(i + 1, j));
				indices.push_back(calcIdx(i, j + 1));
				indices.push_back(calcIdx(i, j + 1));
				indices.push_back(calcIdx(i + 1, j));
				indices.push_back(calcIdx(i + 1, j + 1));
			}

			//Fill empty space
			//We have bands between last longitude and first, so we need wrap it
			indices.push_back(calcIdx(i, longDiv - 1));
			indices.push_back(calcIdx(i + 1, longDiv - 1));
			indices.push_back(calcIdx(i, 0));
			indices.push_back(calcIdx(i, 0));
			indices.push_back(calcIdx(i + 1, longDiv - 1));
			indices.push_back(calcIdx(i + 1, 0));
		}

		//Wrap poles
		for (int j = 0; j < longDiv - 1; j++)
		{
			//north
			indices.push_back(northPole);
			indices.push_back(calcIdx(0, j));
			indices.push_back(calcIdx(0, j + 1));

			//south
			indices.push_back(calcIdx(latDiv - 2, j + 1));
			indices.push_back(calcIdx(latDiv - 2, j));
			indices.push_back(southPole);
		}
		//Wrap bands on pole sides
		//North
		indices.push_back(northPole);
		indices.push_back(calcIdx(0, longDiv - 1));
		indices.push_back(calcIdx(0, 0));
		//South
		indices.push_back(calcIdx(latDiv - 2, 0));
		indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
		indices.push_back(southPole);

		return { std::move(vertices), std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(12, 24);
	}
};