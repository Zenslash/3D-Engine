#include "Sheet.h"
#include "IndexedTriangleList.h"
#include "BindableBase.h"
#include "Plane.h"
#include "Surface.h"
#include "Texture.h"
#include "Sampler.h"

Sheet::Sheet(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist)
	:
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
{
	if (!IsStaticBindsInitialized())
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			struct
			{
				float u;
				float v;
			} tex;
		};

		IndexedTriangleList<Vertex> model = Plane::Make<Vertex>();
		model.vertices[0].tex = { 0.0f, 0.0f };
		model.vertices[1].tex = { 1.0f, 0.0f };
		model.vertices[2].tex = { 0.0f, 1.0f };
		model.vertices[3].tex = { 1.0f, 1.0f };
		AddStaticBind(std::make_unique<Texture>(gfx, Surface::FromFile("Test_Texture.jpg")));
		AddStaticBind(std::make_unique<Sampler>(gfx));
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		auto pVertexShader = std::make_unique<VertexShader>(gfx, L"TextureVS.cso");
		auto pVertexBytecode = pVertexShader->GetBytecode();
		AddStaticBind(std::move(pVertexShader));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"TexturePS.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pVertexBytecode));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}
	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void Sheet::Update(float delta) noexcept
{
	roll += droll * delta;
	yaw += dyaw * delta;
	pitch += dpitch * delta;
	theta += dtheta * delta;
	chi += dchi * delta;
	phi += dphi * delta;
}

DirectX::XMMATRIX Sheet::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
		dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}
