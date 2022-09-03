#pragma once
#include "Bindable.h"
#include "GraphicsMacro.h"

template<typename T>
class ConstantBuffer : public Bindable
{
public:
	void Update(Graphics& gfx, const T& consts)
	{
		GFX_GET_INFO_MANAGER(gfx);

		D3D11_MAPPED_SUBRESOURCE subresource;
		GFX_THROW_INFO(GetContext(gfx)->Map(
			pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u,
			&subresource
		));
		memcpy(subresource.pData, &consts, sizeof(consts));
		GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);
	}
	ConstantBuffer(Graphics& gfx, const T& consts, UINT slot = 0) : slot(slot)
	{
		GFX_GET_INFO_MANAGER(gfx);

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;
		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer));
	}
	ConstantBuffer(Graphics& gfx, UINT slot = 0) : slot(slot)
	{
		GFX_GET_INFO_MANAGER(gfx);

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(T);
		cbd.StructureByteStride = 0u;
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer));
	}
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	UINT slot;
};

template<typename T>
class VertexConstantBuffer : public ConstantBuffer<T>
{
	using ConstantBuffer<T>::pConstantBuffer;
	using ConstantBuffer<T>::slot;
	using Bindable::GetContext;
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void Bind(Graphics& gfx) noexcept override
	{
		GetContext(gfx)->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
	}
};

template<typename T>
class PixelConstantBuffer : public ConstantBuffer<T>
{
	using ConstantBuffer<T>::pConstantBuffer;
	using ConstantBuffer<T>::slot;
	using Bindable::GetContext;
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void Bind(Graphics& gfx) noexcept override
	{
		GetContext(gfx)->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
	}
};

