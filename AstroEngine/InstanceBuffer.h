#pragma once
#include <vector>
#include <cstdint>
#include <wrl/client.h>
#include "Types.h"

struct ID3D11Device;
struct ID3D11Buffer;

class InstanceBuffer
{
public:
	InstanceBuffer(const std::vector<Graphics::InstanceData>& instances, ID3D11Device* device,
		unsigned int usage, unsigned int bindFlag, unsigned int cpuFlag);
	
	~InstanceBuffer();

	InstanceBuffer(const InstanceBuffer&) = delete;
	InstanceBuffer& operator=(const InstanceBuffer&) = delete;

	auto GetRawBuffer() const -> ID3D11Buffer* { return m_Buffer.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
};

