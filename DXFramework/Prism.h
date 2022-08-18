#pragma once

#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include "ZenMath.h"

class Prism
{
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int longDiv)
	{
		namespace dx = DirectX;
		assert(longDiv >= 3);

		const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const auto offset = dx::XMVectorSet(0.0f, 0.0f, 2.0f, 0.0f);
		const float longitudeAngle = 2 * PI / longDiv;

		//center
		std::vector<V> vertices;
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, -1.0f };
		const auto center = (unsigned short)(vertices.size() - 1);

		//top
		vertices.emplace_back();
		vertices.back();
		vertices.back().pos = { 0.0f, 0.0f, 1.0f };
		const auto top = (unsigned short)(vertices.size() - 1);

		//Generate vertices
		for (int i = 0; i < longDiv; i++)
		{
			//bottom
			{
				vertices.emplace_back();
				const auto v = dx::XMVector3Transform
				(
					base, dx::XMMatrixRotationZ(longitudeAngle * i)
				);
				dx::XMStoreFloat3(&vertices.back().pos, v);
			}
			//up
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform
				(
					base, dx::XMMatrixRotationZ(longitudeAngle * i)
				);
				v = dx::XMVectorAdd(v, offset);
				dx::XMStoreFloat3(&vertices.back().pos, v);
			}
		}

		std::vector<unsigned short> indices;
		//Generate bottom, up indices
		for (unsigned short i = 0; i < longDiv; i++)
		{
			const auto j = i * 2;
			const auto mod = longDiv * 2;

			indices.push_back(j + 2);
			indices.push_back(center);
			indices.push_back((j + 2) % mod + 2);
			indices.push_back(top);
			indices.push_back(j + 3);
			indices.push_back((j + 3) % mod + 2);
		}
		//Generate side indices
		for (unsigned short i = 0; i < longDiv; i++)
		{
			const auto j = i * 2;
			const auto mod = longDiv * 2;

			indices.push_back(j + 2);
			indices.push_back((j + 2) % mod + 2);
			indices.push_back(j + 3);
			indices.push_back((j + 2) % mod + 2);
			indices.push_back((j + 3) % mod + 2);
			indices.push_back(j + 3);
		}

		return { std::move(vertices), std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(24);
	}
};