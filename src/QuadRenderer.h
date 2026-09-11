#pragma once
#include "Types.h"

#include <cstdint>
#include <memory>
#include <vector>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;
#include <wrl/client.h>

class VertexBuffer;
class IndexBuffer;
class InstanceBuffer;

class QuadRenderer
{
public:
    QuadRenderer(
        ID3D11Device* device, const std::vector<Graphics::Vertex>& vertices,
        const std::vector<Graphics::InstanceData>& instances, const std::vector<unsigned int>& indices
    );

    ~QuadRenderer();

    QuadRenderer(const QuadRenderer&) = delete;
    QuadRenderer& operator=(const QuadRenderer&) = delete;

    auto UpdateTime(ID3D11DeviceContext* context, float time) -> void;
    auto Draw(ID3D11DeviceContext* context, uint32_t maxInstances) -> void;

private:
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<InstanceBuffer> m_InstanceBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer;
};
