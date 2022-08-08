#pragma once

#include "Bindable.h"

class VertexBuffer : public Bindable
{
public:
	template<class T>
	VertexBuffer(Graphics& gfx, const std::vector<T>& vertices);
	void Bind(Graphics& gfx) noexcept override;
	UINT GetStride() const noexcept;
protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};

