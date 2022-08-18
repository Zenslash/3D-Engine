#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include "ZenMath.h"

class Cone
{
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int longDiv)
	{
		namespace dx = DirectX;
		assert(longDiv >= 3);

		const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const float longAngle = 2 * PI / longDiv;

		//Generate base vertices
		std::vector<V> vertices;
		for (int i = 0; i < longDiv; i++)
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform
			(
				base,
				dx::XMMatrixRotationZ(longAngle * i)
			);
			dx::XMStoreFloat3(&vertices.back().pos, v);
		}

		//center
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, -1.0f };
		const auto center = (unsigned short)(vertices.size() - 1);

		//tip
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, 1.0f };
		const auto tip = (unsigned short)(vertices.size() - 1);

		//Wrap base
		std::vector<unsigned short> indices;
		for (unsigned short i = 0; i < longDiv; i++)
		{
			indices.push_back(center);
			indices.push_back((i + 1) % longDiv);
			indices.push_back(i);
		}

		//Wrap cone
		for (unsigned short i = 0; i < longDiv; i++)
		{
			indices.push_back(i);
			indices.push_back((i + 1) % longDiv);
			indices.push_back(tip);
		}

		return { std::move(vertices), std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(24);
	}
};