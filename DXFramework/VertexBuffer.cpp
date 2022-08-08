#include "VertexBuffer.h"
#include "GraphicsMacro.h"

template<class T>
VertexBuffer::VertexBuffer(Graphics& gfx, const std::vector<T>& vertices) : stride(sizeof(T))
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

void VertexBuffer::Bind(Graphics& gfx) noexcept
{
	const UINT offset = 0u;
	GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}

UINT VertexBuffer::GetStride() const noexcept
{
    return stride;
}
