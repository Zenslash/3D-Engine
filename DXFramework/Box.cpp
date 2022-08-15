#include "Box.h"
#include "BindableBase.h"

Box::Box(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist) :
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
			struct
			{
				float x;
				float y;
				float z;
			} pos;
		};

		//Create vertex array
		const std::vector<Vertex> vertices =
		{
			{-1.0f, -1.0f, -1.0f},	//bottom left corner
			{-1.0f, -1.0f, 1.0f},		//bottom upper left corner
			{1.0f, -1.0f, 1.0f},		//bottom upper right corner
			{1.0f, -1.0f, -1.0f},		//bottom right corner
			{-1.0f, 1.0f, -1.0f},		//upper left corner
			{-1.0f, 1.0f, 1.0f},		//upper upper left corner
			{1.0f, 1.0f, 1.0f},		//upper upper right corner
			{1.0f, 1.0f, -1.0f},		//upper right corner
		};
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

		auto pVertexShader = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
		auto pVertexBytecode = pVertexShader->GetBytecode();
		AddStaticBind(std::move(pVertexShader));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

		//Create index buffer
		const std::vector<unsigned short> indices =
		{
			3, 1, 0,	2,1,3,	//+
			0, 1, 4,	1,5,4,	//+
			0, 4, 3,	3,4,7,	//+
			2, 3, 7,	7,6,2,	//+
			6, 7, 5,	5,7,4,	//+
			2, 6, 1,	1,6,5	//+
		};
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

		struct ConstantBuffer2
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			}faceColors[6];
		};
		const ConstantBuffer2 cb2 =
		{
			{
				{1.0f, 1.0f, 0.0f, 1.0f},
				{0.0f, 1.0f, 1.0f, 1.0f},
				{1.0f, 0.0f, 0.0f, 1.0f},
				{1.0f, 1.0f, 1.0f, 1.0f},
				{0.0f, 1.0f, 0.0f, 1.0f},
				{0.0f, 0.0f, 1.0f, 1.0f},
			}
		};
		AddStaticBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
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

void Box::Update(float delta) noexcept
{
	roll += droll * delta;
	yaw += dyaw * delta;
	pitch += dpitch * delta;
	theta += dtheta * delta;
	chi += dchi * delta;
	phi += dphi * delta;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}
