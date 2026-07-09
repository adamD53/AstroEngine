#pragma once
#include <vector>

#include <wrl/client.h>

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

	auto GetRawBuffer() const -> ID3D11Buffer* { return m_Buffer.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
};

