#pragma once
#include <vector>
#include "Types.h"

struct ID3D11Device;
struct ID3D11Buffer;

class VertexBuffer
{
public:
	VertexBuffer(const std::vector<Graphics::Vertex>& vertices, ID3D11Device* device,
		unsigned int usage, unsigned int bindFlag, unsigned int cpuFlag);
	
	~VertexBuffer();

	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;
	
	auto GetRawBuffer() -> ID3D11Buffer* const { return m_Buffer; }

private:
	ID3D11Buffer* m_Buffer;
};

