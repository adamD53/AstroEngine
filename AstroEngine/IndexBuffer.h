#pragma once
#include <vector>

struct ID3D11Device;
struct ID3D11Buffer;

class IndexBuffer
{
public:
	IndexBuffer(const std::vector<unsigned int>& indices, ID3D11Device* device,
		unsigned int usage, unsigned int bindFlag, unsigned int cpuFlag);
	
	~IndexBuffer();

	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	auto GetRawBuffer() -> ID3D11Buffer* const { return m_Buffer; }

private:
	ID3D11Buffer* m_Buffer;
};

