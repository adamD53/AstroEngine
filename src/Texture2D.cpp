#include "Texture2D.hpp"

#include "CoreUtils.hpp"
#include "SDL3/SDL_gpu.h"

#include <cstring>

namespace Graphics
{

Texture2D::Texture2D(SDL_GPUDevice* device, const Image& image)
    : m_Extent(image.GetExtent())
    , m_Texture(nullptr, SDLGPUTextureDeleter{device})
    , m_Sampler(nullptr, SDLGPUSamplerDeleter{device})
{
    Utils::core_assert(device != nullptr, "Texture2D received a null SDL_GPUDevice.");
    Utils::core_assert(image.IsValid(), "Texture2D received an invalid Image.");

    UploadFromImage(device, image);
}

Texture2D::Texture2D(SDL_GPUDevice* device, const std::filesystem::path& filePath, const bool flipVertically)
    : Texture2D(device, Image{filePath, flipVertically})
{
}

void Texture2D::UploadFromImage(SDL_GPUDevice* device, const Image& image)
{
    const auto textureCreateInfo = SDL_GPUTextureCreateInfo{
        .type = SDL_GPU_TEXTURETYPE_2D,
        .format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
        .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
        .width = m_Extent.width,
        .height = m_Extent.height,
        .layer_count_or_depth = 1,
        .num_levels = 1,
        .sample_count = SDL_GPU_SAMPLECOUNT_1,
        .props = 0
    };

    auto* rawTexture = SDL_CreateGPUTexture(device, &textureCreateInfo);
    Utils::core_assert(rawTexture != nullptr, "Failed to create SDL_GPUTexture.");
    m_Texture.reset(rawTexture);

    const auto samplerCreateInfo = SDL_GPUSamplerCreateInfo{
        .min_filter = SDL_GPU_FILTER_LINEAR,
        .mag_filter = SDL_GPU_FILTER_LINEAR,
        .mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR,
        .address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
        .address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
        .address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
        .mip_lod_bias = 0.0f,
        .max_anisotropy = 1.0f,
        .compare_op = SDL_GPU_COMPAREOP_ALWAYS,
        .min_lod = 0.0f,
        .max_lod = 1000.0f,
        .enable_anisotropy = false,
        .enable_compare = false,
        .props = 0
    };

    auto* rawSampler = SDL_CreateGPUSampler(device, &samplerCreateInfo);
    Utils::core_assert(rawSampler != nullptr, "Failed to create SDL_GPUSampler.");
    m_Sampler.reset(rawSampler);

    const auto byteSize = static_cast<uint32_t>(image.GetByteSize());
    const auto transferCreateInfo = SDL_GPUTransferBufferCreateInfo{
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = byteSize,
        .props = 0
    };

    auto transferBuffer = std::unique_ptr<SDL_GPUTransferBuffer, SDLGPUTransferBufferDeleter>{
        SDL_CreateGPUTransferBuffer(device, &transferCreateInfo),
        SDLGPUTransferBufferDeleter{device}
    };
    Utils::core_assert(transferBuffer != nullptr, "Failed to create SDL_GPUTransferBuffer for texture upload.");

    auto* mappedPtr = SDL_MapGPUTransferBuffer(device, transferBuffer.get(), false);
    Utils::core_assert(mappedPtr != nullptr, "Failed to map SDL_GPUTransferBuffer for texture upload.");

    const auto pixels = image.GetData();
    std::memcpy(mappedPtr, pixels.data(), pixels.size_bytes());
    SDL_UnmapGPUTransferBuffer(device, transferBuffer.get());

    auto* cmdBuffer = SDL_AcquireGPUCommandBuffer(device);
    Utils::core_assert(cmdBuffer != nullptr, "Failed to acquire command buffer for texture upload.");

    auto* copyPass = SDL_BeginGPUCopyPass(cmdBuffer);
    Utils::core_assert(copyPass != nullptr, "Failed to begin GPU copy pass for texture upload.");

    const auto transferInfo = SDL_GPUTextureTransferInfo{
        .transfer_buffer = transferBuffer.get(),
        .offset = 0,
        .pixels_per_row = m_Extent.width,
        .rows_per_layer = m_Extent.height
    };

    const auto destinationRegion = SDL_GPUTextureRegion{
        .texture = m_Texture.get(),
        .mip_level = 0,
        .layer = 0,
        .x = 0,
        .y = 0,
        .z = 0,
        .w = m_Extent.width,
        .h = m_Extent.height,
        .d = 1
    };

    SDL_UploadToGPUTexture(copyPass, &transferInfo, &destinationRegion, false);
    SDL_EndGPUCopyPass(copyPass);

    const auto submitted = SDL_SubmitGPUCommandBuffer(cmdBuffer);
    Utils::core_assert(submitted, "Failed to submit texture upload command buffer.");
}

void Texture2D::Bind(SDL_GPURenderPass* renderPass, const uint32_t slot) const
{
    Utils::core_assert(renderPass != nullptr, "Texture2D::Bind received a null render pass.");
    Utils::core_assert(IsValid(), "Texture2D::Bind called on an uninitialized texture.");

    const auto samplerBinding = SDL_GPUTextureSamplerBinding{
        .texture = m_Texture.get(),
        .sampler = m_Sampler.get()
    };

    SDL_BindGPUFragmentSamplers(renderPass, slot, &samplerBinding, 1);
}

auto Texture2D::GetExtent() const noexcept -> Extent2D
{
    return m_Extent;
}

auto Texture2D::GetTexture() const noexcept -> SDL_GPUTexture*
{
    return m_Texture.get();
}

auto Texture2D::GetSampler() const noexcept -> SDL_GPUSampler*
{
    return m_Sampler.get();
}

auto Texture2D::IsValid() const noexcept -> bool
{
    return m_Texture != nullptr && m_Sampler != nullptr;
}

} // namespace Graphics
