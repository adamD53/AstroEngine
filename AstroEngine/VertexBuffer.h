#pragma once
#include <vector>
#include <wrl/client.h>
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
	
	auto GetRawBuffer() const -> ID3D11Buffer* { return m_Buffer.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
};

