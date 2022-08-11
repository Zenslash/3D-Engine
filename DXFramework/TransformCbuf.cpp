#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
	:
	transformBuffer(gfx),
	parent(parent)
{}

void TransformCbuf::Bind(Graphics& gfx) noexcept
{
	transformBuffer.Update(gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * gfx.GetProjection()
		));
	transformBuffer.Bind(gfx);
}
