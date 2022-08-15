#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
	:
	parent(parent)
{
	if (transformBuffer == nullptr)
	{
		transformBuffer = std::make_unique< VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

void TransformCbuf::Bind(Graphics& gfx) noexcept
{
	transformBuffer->Update(gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * gfx.GetProjection()
		));
	transformBuffer->Bind(gfx);
}

std::unique_ptr< VertexConstantBuffer<DirectX::XMMATRIX>> TransformCbuf::transformBuffer;
