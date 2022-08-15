#pragma once

#include "Bindable.h"
#include "GraphicsMacro.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices) : count((UINT)indices.size())
	{
		GFX_GET_INFO_MANAGER(gfx);

		D3D11_BUFFER_DESC indexDesc = {};
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexDesc.Usage = D3D11_USAGE_DEFAULT;
		indexDesc.ByteWidth = UINT(count * sizeof(unsigned short));
		indexDesc.CPUAccessFlags = 0u;
		indexDesc.MiscFlags = 0u;
		indexDesc.StructureByteStride = sizeof(unsigned short);
		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = indices.data();
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&indexDesc, &indexData, &pIndexBuffer));
	}

	void Bind(Graphics& gfx) noexcept override;
	UINT GetCount() const noexcept;
protected:
	UINT count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};

