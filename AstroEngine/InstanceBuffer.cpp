#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "InstanceBuffer.h"
#include "CoreUtils.h"

InstanceBuffer::InstanceBuffer(const std::vector<Graphics::InstanceData>& instances, ID3D11Device* device,
    unsigned int usage, unsigned int bindFlag, unsigned int cpuFlag)
{
    Utils::core_assert(instances.empty() == false, "DirectX Instance Buffer got an empty instances data.");
    
    D3D11_BUFFER_DESC instanceBufferDesc = {
        .ByteWidth = static_cast<UINT>(sizeof(Graphics::InstanceData) * instances.size()),
        .Usage = static_cast<D3D11_USAGE>(usage),
        .BindFlags = bindFlag,
        .CPUAccessFlags = cpuFlag
    };

    D3D11_SUBRESOURCE_DATA instanceBufferInitData = {};
    instanceBufferInitData.pSysMem = instances.data();

    HRESULT hr = device->CreateBuffer(&instanceBufferDesc, &instanceBufferInitData, &m_Buffer);
    Utils::core_assert(SUCCEEDED(hr), "DirectX Instance Buffer failed to create.");
}

InstanceBuffer::~InstanceBuffer() = default;
