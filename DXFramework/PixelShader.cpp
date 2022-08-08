#include "PixelShader.h"
#include "GraphicsMacro.h"
#include <d3dcompiler.h>

PixelShader::PixelShader(Graphics& gfx, const std::wstring& path)
{
	GFX_GET_INFO_MANAGER(gfx);

	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pShaderBytecode));
	GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(pShaderBytecode->GetBufferPointer(), pShaderBytecode->GetBufferSize(), nullptr, &pPixelShader));
}

void PixelShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}

ID3DBlob* PixelShader::GetBytecode() const noexcept
{
	return pShaderBytecode.Get();
}
