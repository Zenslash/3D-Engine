#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot)
	:
	parent(parent)
{
	if (transformBuffer == nullptr)
	{
		transformBuffer = std::make_unique< VertexConstantBuffer<Transforms>>(gfx, slot);
	}
}

void TransformCbuf::Bind(Graphics& gfx) noexcept
{
	const auto modelView = parent.GetTransformXM() * gfx.GetCamera();
	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(modelView * gfx.GetProjection())
	};

	transformBuffer->Update(gfx, tf);
	transformBuffer->Bind(gfx);
}

std::unique_ptr< VertexConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::transformBuffer;
