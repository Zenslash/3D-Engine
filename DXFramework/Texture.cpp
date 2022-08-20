#include "Texture.h"
#include "GraphicsMacro.h"
#include "Surface.h"

Texture::Texture(Graphics& gfx, const Surface& surface) noexcept(!IS_DEBUG)
{
	GFX_GET_INFO_MANAGER(gfx);

	//Create resource descriptor
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = surface.GetWidth();
	desc.Height = surface.GetHeight();
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = surface.GetBufferPtr();
	sd.SysMemPitch = surface.GetWidth() * sizeof(Surface::Color);
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&desc, &sd, &pTexture));

	//Create resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format = desc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MostDetailedMip = 0;
	viewDesc.Texture2D.MipLevels = 1;
	GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView
	(
		pTexture.Get(), &viewDesc, &pTextureView
	));
}

void Texture::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShaderResources(0u, 1u, pTextureView.GetAddressOf());
}
