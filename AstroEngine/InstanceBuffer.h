#pragma once
#include <vector>
#include <cstdint>
#include "Types.h"

struct ID3D11Device;
struct ID3D11Buffer;

class InstanceBuffer
{
public:
	InstanceBuffer(const std::vector<Graphics::InstanceData>& instances, uint32_t maxInstances, ID3D11Device* device,
		unsigned int usage, unsigned int bindFlag, unsigned int cpuFlag);
	
	~InstanceBuffer();

	InstanceBuffer(const InstanceBuffer&) = delete;
	InstanceBuffer& operator=(const InstanceBuffer&) = delete;

	auto GetRawBuffer() -> ID3D11Buffer* const { return m_Buffer; }

private:
	ID3D11Buffer* m_Buffer;
};

