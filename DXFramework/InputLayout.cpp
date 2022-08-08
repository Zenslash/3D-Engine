#include "InputLayout.h"
#include "GraphicsMacro.h"

InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& desc, ID3DBlob* pVertexShaderBytecode)
{
	GFX_GET_INFO_MANAGER(gfx);

	GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(
		desc.data(), (UINT)desc.size(),
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&pInputLayout
	));
}

void InputLayout::Bind(Graphics& gfx) noexcept
{
	//Bind vertex layout
	GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}
