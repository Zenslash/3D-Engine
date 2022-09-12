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
	static IndexedTriangleList<V> MakeTesselatedIndependentFaces(int longDiv)
	{
		assert(longDiv >= 3);

		const auto base = DirectX::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const float longitudeAngle = 2.0f * PI / longDiv;

		std::vector<V> vertices;

		//upper vertices
		const unsigned short cone = 0;
		for (int i = 0; i < longDiv; i++)
		{
			const float thetas[] =
			{
				longitudeAngle * i,
				longitudeAngle * (((i + 1) == longDiv) ? 0 : (i + 1))
			};
			//TODO Do I need so much vertices in one place?
			vertices.emplace_back();
			vertices.back().pos = { 0.0f, 0.0f, 1.0f };
			for (auto theta : thetas)
			{
				vertices.emplace_back();
				const auto v = DirectX::XMVector3Transform
				(
					base,
					DirectX::XMMatrixRotationZ(theta)
				);
				DirectX::XMStoreFloat3(&vertices.back().pos, v);
			}
		}

		//bottom vertices
		const auto baseCenter = (unsigned short)vertices.size();
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, -1.0f };
		const auto baseEdge = (unsigned short)vertices.size();
		for (int i = 0; i < longDiv; i++)
		{
			vertices.emplace_back();
			auto v = DirectX::XMVector3Transform
			(
				base,
				DirectX::XMMatrixRotationZ(longitudeAngle * i)
			);
			DirectX::XMStoreFloat3(&vertices.back().pos, v);
		}

		std::vector<unsigned short> indices;

		//upper indices
		for (unsigned short i = 0; i < longDiv * 3; i++)
		{
			indices.push_back(i + cone);
		}
		//bottom indices
		for (unsigned short i = 0; i < longDiv; i++)
		{
			indices.push_back(baseCenter);
			indices.push_back((i + 1) % longDiv + baseEdge);
			indices.push_back(i + baseEdge);
		}

		return { std::move(vertices), std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(24);
	}
};