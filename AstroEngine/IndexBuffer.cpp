#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "IndexBuffer.h"
#include "CoreUtils.h"

IndexBuffer::IndexBuffer(const std::vector<unsigned int>& indices, ID3D11Device* device, 
    unsigned int usage, unsigned int bindFlag, unsigned int cpuFlag)
    : m_Buffer(nullptr)
{
    D3D11_BUFFER_DESC indexBufferDesc = {
        .ByteWidth = static_cast<UINT>(sizeof(unsigned int) * indices.size()),
        .Usage = static_cast<D3D11_USAGE>(usage),
        .BindFlags = bindFlag,
        .CPUAccessFlags = cpuFlag
    };

    D3D11_SUBRESOURCE_DATA indexBufferInitData = {};
    indexBufferInitData.pSysMem = indices.data();

    HRESULT hr = device->CreateBuffer(&indexBufferDesc, &indexBufferInitData, &m_Buffer);
    Utils::core_assert(SUCCEEDED(hr), "DirectX Index Buffer failed to create.");
}

IndexBuffer::~IndexBuffer()
{
    if (m_Buffer) m_Buffer->Release();
}
