#pragma once

#include <vector>
#include <utility>

template<class T>
class IndexedTriangleList
{
public:
	IndexedTriangleList() = default;
	IndexedTriangleList(std::vector<T> vert, std::vector<unsigned short> ind) :
		vertices(std::move(vert)),
		indices(std::move(ind))
	{}
	~IndexedTriangleList() = default;

public:
	std::vector<T> vertices;
	std::vector<unsigned short> indices;
};