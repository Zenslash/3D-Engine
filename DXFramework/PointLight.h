#pragma once

#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"

class PointLight
{
public:
	PointLight(Graphics& gfx, float radius = 0.5f);
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Draw(Graphics& gfx) const noexcept(!IS_DEBUG);
	void Bind(Graphics& gfx) const noexcept;
private:
	struct PointLightCBuf
	{
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 materialColor;
		alignas(16) DirectX::XMFLOAT3 ambientColor;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntentisy;
		float attConst;
		float attLin;
		float attQuad;
	};
private:
	PointLightCBuf constantBufferData;
	mutable SolidSphere mesh;
	mutable PixelConstantBuffer<PointLightCBuf> constantBuffer;
};

