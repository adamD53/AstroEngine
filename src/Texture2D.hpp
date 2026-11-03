#pragma once

#include "Image.hpp"
#include "SDLDeleters.hpp"
#include "Types.hpp"

#include <SDL3/SDL_gpu.h>

#include <cstdint>
#include <filesystem>
#include <memory>

namespace Graphics
{

class Texture2D final
{
public:
    Texture2D(SDL_GPUDevice* device, const Image& image);
    Texture2D(SDL_GPUDevice* device, const std::filesystem::path& filePath, bool flipVertically = false);
    ~Texture2D() = default;

    Texture2D(const Texture2D&) = delete;
    auto operator=(const Texture2D&) -> Texture2D& = delete;

    Texture2D(Texture2D&&) noexcept = default;
    auto operator=(Texture2D&&) noexcept -> Texture2D& = default;

    void Bind(SDL_GPURenderPass* renderPass, uint32_t slot = 0) const;

    [[nodiscard]] auto GetExtent() const noexcept -> Extent2D;
    [[nodiscard]] auto GetTexture() const noexcept -> SDL_GPUTexture*;
    [[nodiscard]] auto GetSampler() const noexcept -> SDL_GPUSampler*;
    [[nodiscard]] auto IsValid() const noexcept -> bool;

private:
    void UploadFromImage(SDL_GPUDevice* device, const Image& image);

    Extent2D m_Extent{};
    std::unique_ptr<SDL_GPUTexture, SDLGPUTextureDeleter> m_Texture{nullptr};
    std::unique_ptr<SDL_GPUSampler, SDLGPUSamplerDeleter> m_Sampler{nullptr};
};

} // namespace Graphics
