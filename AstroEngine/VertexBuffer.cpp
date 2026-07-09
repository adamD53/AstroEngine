#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "VertexBuffer.h"
#include "CoreUtils.h"

VertexBuffer::VertexBuffer(const std::vector<Graphics::Vertex>& vertices, ID3D11Device* device, 
    unsigned int usage, unsigned int bindFlag, unsigned int cpuFlag)
    : m_Buffer(nullptr)
{
    D3D11_BUFFER_DESC vertexBufferDesc = {
        .ByteWidth = static_cast<UINT>(sizeof(Graphics::Vertex) * vertices.size()),
        .Usage = static_cast<D3D11_USAGE>(usage),
        .BindFlags = bindFlag,
        .CPUAccessFlags = cpuFlag
    };
    
    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices.data();

    HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &initData, &m_Buffer);
    Utils::core_assert(SUCCEEDED(hr), "DirectX Vertex Buffer failed to create.");
}

VertexBuffer::~VertexBuffer()
{
	if (m_Buffer) m_Buffer->Release();
}
