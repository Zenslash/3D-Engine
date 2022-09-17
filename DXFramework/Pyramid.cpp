#include "Pyramid.h"
#include "BindableBase.h"
#include <array>
#include <vector>
#include "Cone.h"

Pyramid::Pyramid(Graphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_int_distribution<int>& tdist)
	: TestObject(gfx, rng, adist, ddist, odist, rdist)
{
	if (!IsStaticBindsInitialized())
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT3 n;
			std::array<char, 4> color;
			char padding;
		};
		const auto tesselation = tdist(rng);
		auto model = Cone::MakeTesselatedIndependentFaces<Vertex>(tesselation);
		for (auto& v : model.vertices)
		{
			v.color = { (char)10, (char)10, (char)255 };
		}
		for (int i = 0; i < tesselation; i++)
		{
			model.vertices[i * 3].color = { (char)255, (char)20, (char)20 };
		}
		model.Transform(DirectX::XMMatrixScaling(1.0f, 1.0f, 0.7f));
		model.SetNormalsIndependentFlat();

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		auto pVertexShader = std::make_unique<VertexShader>(gfx, L"BlendedPhongVS.cso");
		auto pVertexShaderBytecode = pVertexShader->GetBytecode();
		AddStaticBind(std::move(pVertexShader));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"BlendedPhongPS.cso"));
		
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pVertexShaderBytecode));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}
