#include "VertexBuffer.hpp"

#include "CoreUtils.hpp"

VertexBuffer::VertexBuffer(SDL_GPUDevice* device, const std::span<const Graphics::Vertex> vertices)
    : m_Buffer(device, SDL_GPU_BUFFERUSAGE_VERTEX, static_cast<uint32_t>(vertices.size_bytes()))
    , m_VertexCount(static_cast<uint32_t>(vertices.size()))
{
    Utils::core_assert(!vertices.empty(), "VertexBuffer initialized with empty vertex span.");
    m_Buffer.SetData(device, vertices);
}

VertexBuffer::~VertexBuffer() = default;

void VertexBuffer::Bind(SDL_GPURenderPass* renderPass, const uint32_t slot) const
{
    Utils::core_assert(renderPass != nullptr, "VertexBuffer::Bind received null render pass.");

    const auto binding = SDL_GPUBufferBinding{
        .buffer = m_Buffer.GetRawBuffer(),
        .offset = 0
    };

    SDL_BindGPUVertexBuffers(renderPass, slot, &binding, 1);
}

auto VertexBuffer::GetRawBuffer() const -> SDL_GPUBuffer*
{
    return m_Buffer.GetRawBuffer();
}

auto VertexBuffer::GetVertexCount() const -> uint32_t
{
    return m_VertexCount;
}
