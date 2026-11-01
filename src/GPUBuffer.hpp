#pragma once

#include "SDLDeleters.hpp"

#include <SDL3/SDL_gpu.h>
#include <cstdint>
#include <memory>
#include <span>

class GPUBuffer
{
public:
    GPUBuffer(SDL_GPUDevice* device, SDL_GPUBufferUsageFlags usage, uint32_t sizeInBytes);
    ~GPUBuffer();

    GPUBuffer(const GPUBuffer&) = delete;
    auto operator=(const GPUBuffer&) -> GPUBuffer& = delete;

    GPUBuffer(GPUBuffer&&) noexcept = default;
    auto operator=(GPUBuffer&&) noexcept -> GPUBuffer& = default;

    void SetData(SDL_GPUDevice* device, const void* data, uint32_t sizeInBytes);

    template <typename T>
    void SetData(SDL_GPUDevice* device, const std::span<const T> elements)
    {
        SetData(device, elements.data(), static_cast<uint32_t>(elements.size_bytes()));
    }

    auto GetRawBuffer() const -> SDL_GPUBuffer*;
    auto GetSize() const -> uint32_t;
    auto GetUsage() const -> SDL_GPUBufferUsageFlags;

private:
    std::unique_ptr<SDL_GPUBuffer, SDLGPUBufferDeleter> m_Buffer;
    uint32_t m_SizeInBytes{0};
    SDL_GPUBufferUsageFlags m_Usage{0};
};
