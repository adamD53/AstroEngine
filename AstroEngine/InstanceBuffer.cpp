#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "InstanceBuffer.h"
#include "CoreUtils.h"

InstanceBuffer::InstanceBuffer(const std::vector<Graphics::InstanceData>& instances, uint32_t maxInstances, 
    ID3D11Device* device, unsigned int usage, unsigned int bindFlag, unsigned int cpuFlag)
    : m_Buffer(nullptr)
{
    D3D11_BUFFER_DESC instanceBufferDesc = {
        .ByteWidth = sizeof(Graphics::InstanceData) * maxInstances,
        .Usage = static_cast<D3D11_USAGE>(usage),
        .BindFlags = bindFlag,
        .CPUAccessFlags = cpuFlag
    };

    D3D11_SUBRESOURCE_DATA instanceBufferInitData = {};
    instanceBufferInitData.pSysMem = instances.data();

    HRESULT hr = device->CreateBuffer(&instanceBufferDesc, &instanceBufferInitData, &m_Buffer);
    Utils::core_assert(SUCCEEDED(hr), "DirectX Instance Buffer failed to create.");
}

InstanceBuffer::~InstanceBuffer()
{
	if (m_Buffer) m_Buffer->Release();
}
