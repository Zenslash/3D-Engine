#include "VertexShader.h"
#include "GraphicsMacro.h"
#include <d3dcompiler.h>

VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
{
	GFX_GET_INFO_MANAGER(gfx);

	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pShaderBytecode));
	GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(pShaderBytecode->GetBufferPointer(), pShaderBytecode->GetBufferSize(), nullptr, &pVertexShader));
}

void VertexShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetBytecode() const noexcept
{
	return pShaderBytecode.Get();
}
