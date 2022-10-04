#pragma once

#include <vector>
#include <DirectXMath.h>

struct BGRAColor
{
	unsigned char a;
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class VertexLayout
{
public:
	enum ElementType
	{
		Position2D,
		Position3D,
		Texture2D,
		Normal,
		Float3Color,
		Float4Color,
		BGRAColor,
	};
	class Element
	{
	public:
		Element(ElementType type, size_t offset) 
			: type(type), offset(offset)
		{}
		size_t GetOffsetAfter() const noexcept(!IS_DEBUG)
		{
			return offset + Size();
		}
		size_t GetOffset() const
		{
			return offset;
		}
		size_t Size() const noexcept(!IS_DEBUG)
		{
			return SizeOf(type);
		}
		static constexpr size_t SizeOf(ElementType type) noexcept(!IS_DEBUG)
		{
			switch (type)
			{
			case VertexLayout::Position2D:
				return sizeof(DirectX::XMFLOAT2);
			case VertexLayout::Position3D:
				return sizeof(DirectX::XMFLOAT3);
			case VertexLayout::Texture2D:
				return sizeof(DirectX::XMFLOAT2);
			case VertexLayout::Normal:
				return sizeof(DirectX::XMFLOAT3);
			case VertexLayout::Float3Color:
				return sizeof(DirectX::XMFLOAT3);
			case VertexLayout::Float4Color:
				return sizeof(DirectX::XMFLOAT4);
			case VertexLayout::BGRAColor:
				return sizeof(unsigned int);
			default:
				break;
			}

			assert("Invalid argument type" && false);
			return 0u;
		}
		ElementType GetType() const noexcept
		{
			return type;
		}
	private:
		ElementType type;
		size_t offset;
	};

public:
	template<ElementType Type>
	const Element& Resolve() const noexcept(!IS_DEBUG)
	{
		for (auto& e : elements)
		{
			if (e.GetType() == Type)
			{
				return e;
			}
		}
		assert("Couldn't resolve element type" && false);
		return elements.front();
	}

	const Element& ResolveByIndex(size_t i) const noexcept(!IS_DEBUG)
	{
		return elements[i];
	}

	template<ElementType Type>
	VertexLayout& Append() noexcept(!IS_DEBUG)
	{
		elements.emplace_back(Type, Size());
		return *this;
	}

	size_t Size() const noexcept(!IS_DEBUG)
	{
		return elements.empty() ? 0u : elements.back().GetOffsetAfter();
	}
private:
	std::vector<Element> elements;
};

class Vertex
{
	friend class VertexBuffer;
public:
	template<VertexLayout::ElementType Type>
	auto& Attr() noexcept(!IS_DEBUG)
	{
		auto& element = layout.Resolve<Type>();
		auto pAttr = pData + element.GetOffset();

		if constexpr (Type == VertexLayout::Position2D)
		{
			return *reinterpret_cast<DirectX::XMFLOAT2*>(pAttr);
		}
		else if constexpr (Type == VertexLayout::Position3D)
		{
			return *reinterpret_cast<DirectX::XMFLOAT3*>(pAttr);
		}
		else if constexpr (Type == VertexLayout::Texture2D)
		{
			return *reinterpret_cast<DirectX::XMFLOAT2*>(pAttr);
		}
		else if constexpr (Type == VertexLayout::Normal)
		{
			return *reinterpret_cast<DirectX::XMFLOAT3*>(pAttr);
		}
		else if constexpr (Type == VertexLayout::Float3Color)
		{
			return *reinterpret_cast<DirectX::XMFLOAT3*>(pAttr);
		}
		else if constexpr (Type == VertexLayout::Float4Color)
		{
			return *reinterpret_cast<DirectX::XMFLOAT4*>(pAttr);
		}
		else if constexpr (Type == VertexLayout::BGRAColor)
		{
			return *reinterpret_cast<BGRAColor*>(pAttr);
		}
		else
		{
			assert("Invalid argument type" && false);
			return *reinterpret_cast<char*>(pAttr);
		}
	}
	template<typename T>
	void SetAttributeByIndex(size_t i, T&& val) noexcept(!IS_DEBUG)
	{
		const auto& elem = layout.ResolveByIndex(i);
		auto pAttr = pData + elem.GetOffset();
		switch (elem.GetType())
		{
		case VertexLayout::Position2D:
			SetAttribute<DirectX::XMFLOAT2>(pAttr, std::forward<T>(val));
			break;
		case VertexLayout::Position3D:
			SetAttribute<DirectX::XMFLOAT3>(pAttr, std::forward<T>(val));
			break;
		case VertexLayout::Texture2D:
			SetAttribute<DirectX::XMFLOAT2>(pAttr, std::forward<T>(val));
			break;
		case VertexLayout::Normal:
			SetAttribute<DirectX::XMFLOAT3>(pAttr, std::forward<T>(val));
			break;
		case VertexLayout::Float3Color:
			SetAttribute<DirectX::XMFLOAT3>(pAttr, std::forward<T>(val));
			break;
		case VertexLayout::Float4Color:
			SetAttribute<DirectX::XMFLOAT4>(pAttr, std::forward<T>(val));
			break;
		case VertexLayout::BGRAColor:
			SetAttribute<BGRAColor>(pAttr, std::forward<T>(val));
			break;
		default:
			assert("Bad element type" && false);
		}
	}

private:
	Vertex(char* pData, const VertexLayout& layout) noexcept(!IS_DEBUG)
		:
		pData(pData),
		layout(layout)
	{
		assert(pData != nullptr);
	}

	template<typename First, typename ...Rest>
	void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest) noexcept(!IS_DEBUG)
	{
		SetAttributeByIndex(i, std::forward<First>(first));
		SetAttributeByIndex(i, std::forward<Rest>(rest)... );
	}

	template<typename Dest, typename Src>
	void SetAttribute(char* pAttr, Src&& val)
	{
		if constexpr (std::is_assignable<Dest, Src>::value)
		{
			*reinterpret_cast<Dest*>(pAttr) = val;
		}
		else
		{
			assert("Param attribute type mismatch" && false);
		}
	}
private:
	char* pData = nullptr;
	const VertexLayout& layout;
};

class VertexBuffer
{
public:
	VertexBuffer(VertexLayout layout) noexcept(!IS_DEBUG)
		: layout(std::move(layout))
	{}
	const VertexLayout& GetLayout() const noexcept
	{
		return layout;
	}
	size_t Size() const noexcept(!IS_DEBUG)
	{
		return buffer.size() / layout.Size();
	}
	template<typename ...Params>
	void EmplaceBack(Params&&... params) noexcept(!IS_DEBUG)
	{
		buffer.resize(buffer.size() + layout.Size());
		Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
	}
	Vertex Back() noexcept(!IS_DEBUG)
	{
		assert(buffer.size() != 0u);
		return Vertex{ buffer.data() + buffer.size() - layout.Size(), layout };
	}
	Vertex Front() noexcept(!IS_DEBUG)
	{
		assert(buffer.size() != 0u);
		return Vertex{ buffer.data(), layout };
	}
	Vertex operator[](size_t i) noexcept(!IS_DEBUG)
	{
		assert(i < Size());
		return Vertex{ buffer.data() + layout.Size() * i, layout };
	}
private:
	std::vector<char> buffer;
	VertexLayout layout;
};