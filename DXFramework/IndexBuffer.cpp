#include "IndexBuffer.h"

void IndexBuffer::Bind(Graphics& gfx) noexcept
{
	//Bind buffer to IA
	GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const noexcept
{
    return count;
}
