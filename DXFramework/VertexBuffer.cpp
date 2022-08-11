#include "VertexBuffer.h"
#include "GraphicsMacro.h"

void VertexBuffer::Bind(Graphics& gfx) noexcept
{
	const UINT offset = 0u;
	GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}

UINT VertexBuffer::GetStride() const noexcept
{
    return stride;
}
