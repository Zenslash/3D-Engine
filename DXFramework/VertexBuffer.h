#pragma once

#include "Bindable.h"
#include "GraphicsMacro.h"

class VertexBuffer : public Bindable
{
public:
	template<class T>
	VertexBuffer(Graphics& gfx, const std::vector<T>& vertices) : stride(sizeof(T))
	{
		GFX_GET_INFO_MANAGER(gfx);

		//Create vertex buffer
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
		D3D11_BUFFER_DESC vertexDesc = {};
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexDesc.ByteWidth = UINT(sizeof(T) * vertices.size());
		vertexDesc.CPUAccessFlags = 0u;
		vertexDesc.MiscFlags = 0u;
		vertexDesc.StructureByteStride = stride;
		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = vertices.data();
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&vertexDesc, &vertexData, &pVertexBuffer));
	}
	void Bind(Graphics& gfx) noexcept override;
	UINT GetStride() const noexcept;
protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};

