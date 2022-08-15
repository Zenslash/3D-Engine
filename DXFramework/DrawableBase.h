#pragma once
#include "Drawable.h"
#include "IndexBuffer.h"

template<class T>
class DrawableBase : public Drawable
{
protected:
	static bool IsStaticBindsInitialized() noexcept
	{
		return !staticBinds.empty();
	}
	static void AddStaticBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG)
	{
		staticBinds.push_back(std::move(bind));
	}
	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer) noexcept(!IS_DEBUG)
	{
		assert(pIndexBuffer == nullptr);
		pIndexBuffer = indexBuffer.get();
		staticBinds.push_back(std::move(indexBuffer));
	}
	void SetIndexFromStatic() noexcept(!IS_DEBUG)
	{
		assert("Trying to SetIndexFromStatic " && pIndexBuffer == nullptr);
		for (const auto& b : staticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
			{
				pIndexBuffer = p;
				return;
			}
		}
		assert("SetIndexFromStatic failed" && pIndexBuffer == nullptr);
	}
private:
	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override
	{
		return staticBinds;
	}
private:
	static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;

