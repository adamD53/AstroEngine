#pragma once

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>

struct SDLWindowDeleter
{
    void operator()(SDL_Window* window) const noexcept
    {
        if (window != nullptr)
        {
            SDL_DestroyWindow(window);
        }
    }
};

struct SDLGPUDeviceDeleter
{
    void operator()(SDL_GPUDevice* device) const noexcept
    {
        if (device != nullptr)
        {
            SDL_DestroyGPUDevice(device);
        }
    }
};

struct SDLGPUBufferDeleter
{
    SDL_GPUDevice* device{nullptr};

    void operator()(SDL_GPUBuffer* buffer) const noexcept
    {
        if (device != nullptr && buffer != nullptr)
        {
            SDL_ReleaseGPUBuffer(device, buffer);
        }
    }
};

struct SDLGPUTransferBufferDeleter
{
    SDL_GPUDevice* device{nullptr};

    void operator()(SDL_GPUTransferBuffer* transferBuffer) const noexcept
    {
        if (device != nullptr && transferBuffer != nullptr)
        {
            SDL_ReleaseGPUTransferBuffer(device, transferBuffer);
        }
    }
};

struct SDLGPUTextureDeleter
{
    SDL_GPUDevice* device{nullptr};

    void operator()(SDL_GPUTexture* texture) const noexcept
    {
        if (device != nullptr && texture != nullptr)
        {
            SDL_ReleaseGPUTexture(device, texture);
        }
    }
};

struct SDLGPUSamplerDeleter
{
    SDL_GPUDevice* device{nullptr};

    void operator()(SDL_GPUSampler* sampler) const noexcept
    {
        if (device != nullptr && sampler != nullptr)
        {
            SDL_ReleaseGPUSampler(device, sampler);
        }
    }
};

struct SDLGPUShaderDeleter
{
    SDL_GPUDevice* device{nullptr};

    void operator()(SDL_GPUShader* shader) const noexcept
    {
        if (device != nullptr && shader != nullptr)
        {
            SDL_ReleaseGPUShader(device, shader);
        }
    }
};

struct SDLGPUGraphicsPipelineDeleter
{
    SDL_GPUDevice* device{nullptr};

    void operator()(SDL_GPUGraphicsPipeline* pipeline) const noexcept
    {
        if (device != nullptr && pipeline != nullptr)
        {
            SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
        }
    }
};
