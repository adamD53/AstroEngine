#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "QuadRenderer.h"
#include "Types.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InstanceBuffer.h"
#include "CoreUtils.h"

#include <vector>
#include <array>
#include <math.h>

QuadRenderer::QuadRenderer(ID3D11Device* device, const std::vector<Graphics::Vertex>& vertices,
    const std::vector<Graphics::InstanceData>& instances, const std::vector<unsigned int>& indices)
{
    m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, device, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0);
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, device, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0);
    m_InstanceBuffer = std::make_unique<InstanceBuffer>(instances, device, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0);

    D3D11_BUFFER_DESC cbDesc = {
        .ByteWidth = 16,
        .Usage = D3D11_USAGE_DYNAMIC,
        .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
        .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE
    };

    HRESULT hr = device->CreateBuffer(&cbDesc, nullptr, &m_ConstantBuffer);
    Utils::core_assert(SUCCEEDED(hr), "DirectX Constant Buffer failed to create.");
}

QuadRenderer::~QuadRenderer() = default;

auto QuadRenderer::UpdateTime(ID3D11DeviceContext* context, float time) -> void
{
    struct TimeData {
        float time;
        std::array<float, 3> padding;
    } data;
    data.time = time;
    data.padding[0] = 0.0f;
    data.padding[1] = 0.0f;
    data.padding[2] = 0.0f;

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    context->Map(m_ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    std::memcpy(mappedResource.pData, &data, sizeof(TimeData));
    context->Unmap(m_ConstantBuffer.Get(), 0);

    context->VSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());
}

auto QuadRenderer::Draw(ID3D11DeviceContext* context, uint32_t maxInstances) -> void
{
    std::array<ID3D11Buffer*, 2> buffers = { m_VertexBuffer->GetRawBuffer(), m_InstanceBuffer->GetRawBuffer()};
    std::array<UINT, 2> strides = { sizeof(Graphics::Vertex), sizeof(Graphics::InstanceData) };
    std::array<UINT, 2> offsets = { 0, 0 };
    
    context->IASetVertexBuffers(0, 2, buffers.data(), strides.data(), offsets.data());
    context->IASetIndexBuffer(m_IndexBuffer->GetRawBuffer(), DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        
    context->DrawIndexedInstanced(6, maxInstances, 0, 0, 0);
}
