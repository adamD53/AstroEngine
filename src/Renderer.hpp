#pragma once

#include "SDLDeleters.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>
#include <cstdint>
#include <memory>
#include <span>

class Renderer
{
public:
    explicit Renderer(SDL_Window* window);
    ~Renderer();

    Renderer(const Renderer&) = delete;
    auto operator=(const Renderer&) -> Renderer& = delete;

    Renderer(Renderer&&) noexcept = default;
    auto operator=(Renderer&&) noexcept -> Renderer& = default;

    auto BeginFrame(std::span<const float, 4> clearColor) -> SDL_GPURenderPass*;
    void EndFrame();

    auto GetDevice() const -> SDL_GPUDevice*;
    auto GetWindow() const -> SDL_Window*;
    auto GetCurrentRenderPass() const -> SDL_GPURenderPass*;
    auto GetCurrentCommandBuffer() const -> SDL_GPUCommandBuffer*;

private:
    SDL_Window* m_Window{nullptr};
    std::unique_ptr<SDL_GPUDevice, SDLGPUDeviceDeleter> m_Device;

    SDL_GPUCommandBuffer* m_CurrentCommandBuffer{nullptr};
    SDL_GPURenderPass* m_CurrentRenderPass{nullptr};
};
