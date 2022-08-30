#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
	:
	parent(parent)
{
	if (transformBuffer == nullptr)
	{
		transformBuffer = std::make_unique< VertexConstantBuffer<Transforms>>(gfx);
	}
}

void TransformCbuf::Bind(Graphics& gfx) noexcept
{
	const auto model = parent.GetTransformXM();
	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(model),
		DirectX::XMMatrixTranspose(model * gfx.GetCamera() * gfx.GetProjection())
	};

	transformBuffer->Update(gfx, tf);
	transformBuffer->Bind(gfx);
}

std::unique_ptr< VertexConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::transformBuffer;
