#include "ShaderProgram.hpp"

#include "CoreUtils.hpp"
#include "Types.hpp"

#include <SDL3_shadercross/SDL_shadercross.h>
#include <shaderc/shaderc.hpp>

#include <array>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <vector>

namespace
{

auto ReadShaderFile(const std::filesystem::path& path) -> std::string
{
    const auto resolved = Utils::ResolveAssetPath(path);
    auto file = std::ifstream(resolved, std::ios::in | std::ios::binary);
    Utils::core_assert(file.is_open(), "Failed to open shader file: " + resolved.string());

    const auto fileSize = static_cast<size_t>(std::filesystem::file_size(resolved));
    auto content = std::string(fileSize, '\0');
    if (fileSize > 0)
    {
        file.read(content.data(), static_cast<std::streamsize>(fileSize));
    }
    return content;
}

void EnsureShaderCrossInitialized()
{
    static const auto initialized = []()
    {
        const auto success = SDL_ShaderCross_Init();
        Utils::core_assert(success, "Failed to initialize SDL_shadercross.");
        return success;
    }();
    (void)initialized;
}

} // namespace

ShaderProgram::ShaderProgram(
    SDL_GPUDevice* device,
    SDL_Window* window,
    const std::string_view hlslFilePath
)
    : m_VertexShader(nullptr, SDLGPUShaderDeleter{device})
    , m_PixelShader(nullptr, SDLGPUShaderDeleter{device})
    , m_Pipeline(nullptr, SDLGPUGraphicsPipelineDeleter{device})
    , m_FilePath(hlslFilePath)
{
    Utils::core_assert(device != nullptr, "ShaderProgram received a null SDL_GPUDevice.");
    Utils::core_assert(window != nullptr, "ShaderProgram received a null SDL_Window.");

    EnsureShaderCrossInitialized();
    CreateShaders(device);
    CreatePipeline(device, window);
}

ShaderProgram::~ShaderProgram() = default;

void ShaderProgram::CreateShaders(SDL_GPUDevice* device)
{
    const auto hlslSource = ReadShaderFile(m_FilePath);
    Utils::core_assert(!hlslSource.empty(), "HLSL shader source is empty.");

    const auto compiler = shaderc::Compiler{};
    auto options = shaderc::CompileOptions{};
    options.SetSourceLanguage(shaderc_source_language_hlsl);
    options.SetOptimizationLevel(shaderc_optimization_level_performance);

    // 1. Compile Vertex Shader (VS_Main)
    const auto vsSpv = compiler.CompileGlslToSpv(
        hlslSource,
        shaderc_vertex_shader,
        m_FilePath.c_str(),
        "VS_Main",
        options
    );
    Utils::core_assert(
        vsSpv.GetCompilationStatus() == shaderc_compilation_status_success,
        "Vertex shader compilation failed: " + vsSpv.GetErrorMessage()
    );

    const auto* vsBytes = reinterpret_cast<const uint8_t*>(vsSpv.cbegin());
    const auto vsByteSize = static_cast<size_t>(std::distance(vsSpv.cbegin(), vsSpv.cend()) * sizeof(uint32_t));

    const auto vsInfo = SDL_ShaderCross_SPIRV_Info{
        .bytecode = vsBytes,
        .bytecode_size = vsByteSize,
        .entrypoint = "VS_Main",
        .shader_stage = SDL_SHADERCROSS_SHADERSTAGE_VERTEX,
        .props = 0
    };

    auto* vsMetadata = SDL_ShaderCross_ReflectGraphicsSPIRV(vsBytes, vsByteSize, 0);
    Utils::core_assert(vsMetadata != nullptr, "Failed to reflect vertex shader SPIR-V.");

    auto* rawVertexShader = SDL_ShaderCross_CompileGraphicsShaderFromSPIRV(
        device,
        &vsInfo,
        &vsMetadata->resource_info,
        0
    );
    SDL_free(vsMetadata);
    Utils::core_assert(rawVertexShader != nullptr, "Failed to create SDL_GPUShader for vertex stage.");
    m_VertexShader.reset(rawVertexShader);

    // 2. Compile Pixel / Fragment Shader (PS_Main)
    const auto psSpv = compiler.CompileGlslToSpv(
        hlslSource,
        shaderc_fragment_shader,
        m_FilePath.c_str(),
        "PS_Main",
        options
    );
    Utils::core_assert(
        psSpv.GetCompilationStatus() == shaderc_compilation_status_success,
        "Fragment shader compilation failed: " + psSpv.GetErrorMessage()
    );

    const auto* psBytes = reinterpret_cast<const uint8_t*>(psSpv.cbegin());
    const auto psByteSize = static_cast<size_t>(std::distance(psSpv.cbegin(), psSpv.cend()) * sizeof(uint32_t));

    const auto psInfo = SDL_ShaderCross_SPIRV_Info{
        .bytecode = psBytes,
        .bytecode_size = psByteSize,
        .entrypoint = "PS_Main",
        .shader_stage = SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT,
        .props = 0
    };

    auto* psMetadata = SDL_ShaderCross_ReflectGraphicsSPIRV(psBytes, psByteSize, 0);
    Utils::core_assert(psMetadata != nullptr, "Failed to reflect pixel shader SPIR-V.");

    auto* rawPixelShader = SDL_ShaderCross_CompileGraphicsShaderFromSPIRV(
        device,
        &psInfo,
        &psMetadata->resource_info,
        0
    );
    SDL_free(psMetadata);
    Utils::core_assert(rawPixelShader != nullptr, "Failed to create SDL_GPUShader for pixel stage.");
    m_PixelShader.reset(rawPixelShader);
}

void ShaderProgram::CreatePipeline(SDL_GPUDevice* device, SDL_Window* window)
{
    constexpr auto vertexBufferDesc = SDL_GPUVertexBufferDescription{
        .slot = 0,
        .pitch = sizeof(Graphics::Vertex),
        .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
        .instance_step_rate = 0
    };

    constexpr auto vertexAttributes = std::array<SDL_GPUVertexAttribute, 2>{{
        {
            .location = 0,
            .buffer_slot = 0,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
            .offset = offsetof(Graphics::Vertex, x)
        },
        {
            .location = 1,
            .buffer_slot = 0,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
            .offset = offsetof(Graphics::Vertex, r)
        }
    }};

    const auto colorTargetDesc = SDL_GPUColorTargetDescription{
        .format = SDL_GetGPUSwapchainTextureFormat(device, window),
        .blend_state = SDL_GPUColorTargetBlendState{
            .enable_blend = false,
            .enable_color_write_mask = false
        }
    };

    const auto pipelineCreateInfo = SDL_GPUGraphicsPipelineCreateInfo{
        .vertex_shader = m_VertexShader.get(),
        .fragment_shader = m_PixelShader.get(),
        .vertex_input_state = SDL_GPUVertexInputState{
            .vertex_buffer_descriptions = &vertexBufferDesc,
            .num_vertex_buffers = 1,
            .vertex_attributes = vertexAttributes.data(),
            .num_vertex_attributes = static_cast<uint32_t>(vertexAttributes.size())
        },
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .rasterizer_state = SDL_GPURasterizerState{
            .fill_mode = SDL_GPU_FILLMODE_FILL,
            .cull_mode = SDL_GPU_CULLMODE_NONE,
            .front_face = SDL_GPU_FRONTFACE_CLOCKWISE,
            .depth_bias_constant_factor = 0.0f,
            .depth_bias_clamp = 0.0f,
            .depth_bias_slope_factor = 0.0f,
            .enable_depth_bias = false,
            .enable_depth_clip = true
        },
        .multisample_state = SDL_GPUMultisampleState{
            .sample_count = SDL_GPU_SAMPLECOUNT_1,
            .sample_mask = 0,
            .enable_mask = false
        },
        .depth_stencil_state = SDL_GPUDepthStencilState{
            .compare_op = SDL_GPU_COMPAREOP_ALWAYS,
            .enable_depth_test = false,
            .enable_depth_write = false,
            .enable_stencil_test = false
        },
        .target_info = SDL_GPUGraphicsPipelineTargetInfo{
            .color_target_descriptions = &colorTargetDesc,
            .num_color_targets = 1,
            .depth_stencil_format = SDL_GPU_TEXTUREFORMAT_INVALID,
            .has_depth_stencil_target = false
        },
        .props = 0
    };

    auto* rawPipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineCreateInfo);
    Utils::core_assert(rawPipeline != nullptr, "Failed to create SDL_GPU graphics pipeline.");
    m_Pipeline.reset(rawPipeline);
}

void ShaderProgram::Bind(SDL_GPURenderPass* renderPass) const
{
    Utils::core_assert(renderPass != nullptr, "ShaderProgram::Bind received null render pass.");
    Utils::core_assert(m_Pipeline != nullptr, "ShaderProgram pipeline is uninitialized.");

    SDL_BindGPUGraphicsPipeline(renderPass, m_Pipeline.get());
}

auto ShaderProgram::GetPipeline() const -> SDL_GPUGraphicsPipeline*
{
    return m_Pipeline.get();
}
