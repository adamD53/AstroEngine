#pragma once

#include "SDLDeleters.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>
#include <memory>
#include <string>
#include <string_view>

class ShaderProgram
{
public:
    ShaderProgram(SDL_GPUDevice* device, SDL_Window* window, std::string_view hlslFilePath);
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    auto operator=(const ShaderProgram&) -> ShaderProgram& = delete;

    ShaderProgram(ShaderProgram&&) noexcept = default;
    auto operator=(ShaderProgram&&) noexcept -> ShaderProgram& = default;

    void Bind(SDL_GPURenderPass* renderPass) const;

    auto GetPipeline() const -> SDL_GPUGraphicsPipeline*;

private:
    std::unique_ptr<SDL_GPUShader, SDLGPUShaderDeleter> m_VertexShader;
    std::unique_ptr<SDL_GPUShader, SDLGPUShaderDeleter> m_PixelShader;
    std::unique_ptr<SDL_GPUGraphicsPipeline, SDLGPUGraphicsPipelineDeleter> m_Pipeline;
    std::string m_FilePath;

    void CreateShaders(SDL_GPUDevice* device);
    void CreatePipeline(SDL_GPUDevice* device, SDL_Window* window);
};
