#pragma once

#include "GPUBuffer.hpp"
#include "Types.hpp"

#include <SDL3/SDL_gpu.h>
#include <cstdint>
#include <span>

class VertexBuffer
{
public:
    VertexBuffer(SDL_GPUDevice* device, std::span<const Graphics::Vertex> vertices);
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&) = delete;
    auto operator=(const VertexBuffer&) -> VertexBuffer& = delete;

    VertexBuffer(VertexBuffer&&) noexcept = default;
    auto operator=(VertexBuffer&&) noexcept -> VertexBuffer& = default;

    void Bind(SDL_GPURenderPass* renderPass, uint32_t slot = 0) const;

    auto GetRawBuffer() const -> SDL_GPUBuffer*;
    auto GetVertexCount() const -> uint32_t;

private:
    GPUBuffer m_Buffer;
    uint32_t m_VertexCount{0};
};
