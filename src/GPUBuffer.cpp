#include "GPUBuffer.hpp"

#include "CoreUtils.hpp"

#include <cstring>

GPUBuffer::GPUBuffer(SDL_GPUDevice* device, const SDL_GPUBufferUsageFlags usage, const uint32_t sizeInBytes)
    : m_Buffer(nullptr, SDLGPUBufferDeleter{device})
    , m_SizeInBytes(sizeInBytes)
    , m_Usage(usage)
{
    Utils::core_assert(device != nullptr, "GPUBuffer received a null SDL_GPUDevice.");
    Utils::core_assert(m_SizeInBytes > 0, "GPUBuffer creation requested with 0 bytes.");

    const auto bufferCreateInfo = SDL_GPUBufferCreateInfo{
        .usage = m_Usage,
        .size = m_SizeInBytes,
        .props = 0
    };

    auto* rawBuffer = SDL_CreateGPUBuffer(device, &bufferCreateInfo);
    Utils::core_assert(rawBuffer != nullptr, "Failed to create SDL_GPUBuffer.");
    m_Buffer.reset(rawBuffer);
}

GPUBuffer::~GPUBuffer() = default;

void GPUBuffer::SetData(SDL_GPUDevice* device, const void* data, const uint32_t sizeInBytes)
{
    Utils::core_assert(device != nullptr, "GPUBuffer::SetData received a null SDL_GPUDevice.");
    Utils::core_assert(data != nullptr, "GPUBuffer::SetData received null data pointer.");
    Utils::core_assert(sizeInBytes <= m_SizeInBytes, "GPUBuffer::SetData byte size exceeds buffer capacity.");

    const auto transferCreateInfo = SDL_GPUTransferBufferCreateInfo{
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = sizeInBytes,
        .props = 0
    };

    auto transferBuffer = std::unique_ptr<SDL_GPUTransferBuffer, SDLGPUTransferBufferDeleter>(
        SDL_CreateGPUTransferBuffer(device, &transferCreateInfo),
        SDLGPUTransferBufferDeleter{device}
    );
    Utils::core_assert(transferBuffer != nullptr, "Failed to create SDL_GPUTransferBuffer for upload.");

    auto* mappedMemory = SDL_MapGPUTransferBuffer(device, transferBuffer.get(), false);
    Utils::core_assert(mappedMemory != nullptr, "Failed to map SDL_GPUTransferBuffer.");

    std::memcpy(mappedMemory, data, sizeInBytes);
    SDL_UnmapGPUTransferBuffer(device, transferBuffer.get());

    auto* cmdBuffer = SDL_AcquireGPUCommandBuffer(device);
    Utils::core_assert(cmdBuffer != nullptr, "Failed to acquire command buffer for buffer upload.");

    auto* copyPass = SDL_BeginGPUCopyPass(cmdBuffer);
    Utils::core_assert(copyPass != nullptr, "Failed to begin copy pass for buffer upload.");

    const auto srcLocation = SDL_GPUTransferBufferLocation{
        .transfer_buffer = transferBuffer.get(),
        .offset = 0
    };

    const auto dstRegion = SDL_GPUBufferRegion{
        .buffer = m_Buffer.get(),
        .offset = 0,
        .size = sizeInBytes
    };

    SDL_UploadToGPUBuffer(copyPass, &srcLocation, &dstRegion, false);
    SDL_EndGPUCopyPass(copyPass);

    const auto submitted = SDL_SubmitGPUCommandBuffer(cmdBuffer);
    Utils::core_assert(submitted, "Failed to submit command buffer for buffer upload.");
}

auto GPUBuffer::GetRawBuffer() const -> SDL_GPUBuffer*
{
    return m_Buffer.get();
}

auto GPUBuffer::GetSize() const -> uint32_t
{
    return m_SizeInBytes;
}

auto GPUBuffer::GetUsage() const -> SDL_GPUBufferUsageFlags
{
    return m_Usage;
}
