#pragma once

#include "GPUBuffer.hpp"

#include <SDL3/SDL_gpu.h>
#include <cstdint>
#include <span>

class IndexBuffer
{
public:
    IndexBuffer(SDL_GPUDevice* device, std::span<const uint32_t> indices);
    ~IndexBuffer();

    IndexBuffer(const IndexBuffer&) = delete;
    auto operator=(const IndexBuffer&) -> IndexBuffer& = delete;

    IndexBuffer(IndexBuffer&&) noexcept = default;
    auto operator=(IndexBuffer&&) noexcept -> IndexBuffer& = default;

    void Bind(SDL_GPURenderPass* renderPass) const;

    auto GetRawBuffer() const -> SDL_GPUBuffer*;
    auto GetIndexCount() const -> uint32_t;
    auto GetIndexElementSize() const -> SDL_GPUIndexElementSize;

private:
    GPUBuffer m_Buffer;
    uint32_t m_IndexCount{0};
    SDL_GPUIndexElementSize m_ElementSize{SDL_GPU_INDEXELEMENTSIZE_32BIT};
};
