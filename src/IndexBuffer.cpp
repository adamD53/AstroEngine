#include "IndexBuffer.hpp"

#include "CoreUtils.hpp"

IndexBuffer::IndexBuffer(SDL_GPUDevice* device, const std::span<const uint32_t> indices)
    : m_Buffer(device, SDL_GPU_BUFFERUSAGE_INDEX, static_cast<uint32_t>(indices.size_bytes()))
    , m_IndexCount(static_cast<uint32_t>(indices.size()))
    , m_ElementSize(SDL_GPU_INDEXELEMENTSIZE_32BIT)
{
    Utils::core_assert(!indices.empty(), "IndexBuffer initialized with empty index span.");
    m_Buffer.SetData(device, indices);
}

IndexBuffer::~IndexBuffer() = default;

void IndexBuffer::Bind(SDL_GPURenderPass* renderPass) const
{
    Utils::core_assert(renderPass != nullptr, "IndexBuffer::Bind received null render pass.");

    const auto binding = SDL_GPUBufferBinding{
        .buffer = m_Buffer.GetRawBuffer(),
        .offset = 0
    };

    SDL_BindGPUIndexBuffer(renderPass, &binding, m_ElementSize);
}

auto IndexBuffer::GetRawBuffer() const -> SDL_GPUBuffer*
{
    return m_Buffer.GetRawBuffer();
}

auto IndexBuffer::GetIndexCount() const -> uint32_t
{
    return m_IndexCount;
}

auto IndexBuffer::GetIndexElementSize() const -> SDL_GPUIndexElementSize
{
    return m_ElementSize;
}
