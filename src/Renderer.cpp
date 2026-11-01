#include "Renderer.hpp"

#include "CoreUtils.hpp"

Renderer::Renderer(SDL_Window* window)
    : m_Window(window)
{
    Utils::core_assert(m_Window != nullptr, "Renderer initialized with a null window handle.");

    constexpr auto shaderFormats =
        SDL_GPU_SHADERFORMAT_SPIRV |
        SDL_GPU_SHADERFORMAT_MSL |
        SDL_GPU_SHADERFORMAT_METALLIB |
        SDL_GPU_SHADERFORMAT_DXIL |
        SDL_GPU_SHADERFORMAT_DXBC;

#if !defined(NDEBUG)
    constexpr auto debugMode = true;
#else
    constexpr auto debugMode = false;
#endif

    auto* rawDevice = SDL_CreateGPUDevice(shaderFormats, debugMode, nullptr);
    Utils::core_assert(rawDevice != nullptr, "Failed to create SDL_GPU device.");
    m_Device.reset(rawDevice);

    const auto claimed = SDL_ClaimWindowForGPUDevice(m_Device.get(), m_Window);
    Utils::core_assert(claimed, "Failed to claim SDL3 window for SDL_GPU device.");

    SDL_SetGPUSwapchainParameters(
        m_Device.get(),
        m_Window,
        SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
        SDL_GPU_PRESENTMODE_VSYNC
    );
}

Renderer::~Renderer()
{
    if (m_Device != nullptr && m_Window != nullptr)
    {
        SDL_WaitForGPUIdle(m_Device.get());
        SDL_ReleaseWindowFromGPUDevice(m_Device.get(), m_Window);
    }
}

auto Renderer::BeginFrame(const std::span<const float, 4> clearColor) -> SDL_GPURenderPass*
{
    Utils::core_assert(m_CurrentCommandBuffer == nullptr, "BeginFrame called while previous frame is still active.");

    m_CurrentCommandBuffer = SDL_AcquireGPUCommandBuffer(m_Device.get());
    Utils::core_assert(m_CurrentCommandBuffer != nullptr, "Failed to acquire SDL_GPU command buffer.");

    auto* swapchainTexture = static_cast<SDL_GPUTexture*>(nullptr);
    auto swapchainWidth = 0u;
    auto swapchainHeight = 0u;

    const auto acquired = SDL_WaitAndAcquireGPUSwapchainTexture(
        m_CurrentCommandBuffer,
        m_Window,
        &swapchainTexture,
        &swapchainWidth,
        &swapchainHeight
    );

    // If window is minimized or occluded, swapchain acquisition may return false or null texture
    if (!acquired || swapchainTexture == nullptr)
    {
        SDL_SubmitGPUCommandBuffer(m_CurrentCommandBuffer);
        m_CurrentCommandBuffer = nullptr;
        return nullptr;
    }

    const auto colorTarget = SDL_GPUColorTargetInfo{
        .texture = swapchainTexture,
        .mip_level = 0,
        .layer_or_depth_plane = 0,
        .clear_color = SDL_FColor{clearColor[0], clearColor[1], clearColor[2], clearColor[3]},
        .load_op = SDL_GPU_LOADOP_CLEAR,
        .store_op = SDL_GPU_STOREOP_STORE,
        .resolve_texture = nullptr,
        .resolve_mip_level = 0,
        .resolve_layer = 0,
        .cycle = false,
        .cycle_resolve_texture = false
    };

    m_CurrentRenderPass = SDL_BeginGPURenderPass(m_CurrentCommandBuffer, &colorTarget, 1, nullptr);
    Utils::core_assert(m_CurrentRenderPass != nullptr, "Failed to begin SDL_GPU render pass.");

    const auto viewport = SDL_GPUViewport{
        .x = 0.0f,
        .y = 0.0f,
        .w = static_cast<float>(swapchainWidth),
        .h = static_cast<float>(swapchainHeight),
        .min_depth = 0.0f,
        .max_depth = 1.0f
    };
    SDL_SetGPUViewport(m_CurrentRenderPass, &viewport);

    const auto scissor = SDL_Rect{
        .x = 0,
        .y = 0,
        .w = static_cast<int>(swapchainWidth),
        .h = static_cast<int>(swapchainHeight)
    };
    SDL_SetGPUScissor(m_CurrentRenderPass, &scissor);

    return m_CurrentRenderPass;
}

void Renderer::EndFrame()
{
    if (m_CurrentRenderPass != nullptr)
    {
        SDL_EndGPURenderPass(m_CurrentRenderPass);
        m_CurrentRenderPass = nullptr;
    }

    if (m_CurrentCommandBuffer != nullptr)
    {
        const auto submitted = SDL_SubmitGPUCommandBuffer(m_CurrentCommandBuffer);
        Utils::core_assert(submitted, "Failed to submit SDL_GPU command buffer.");
        m_CurrentCommandBuffer = nullptr;
    }
}

auto Renderer::GetDevice() const -> SDL_GPUDevice*
{
    return m_Device.get();
}

auto Renderer::GetWindow() const -> SDL_Window*
{
    return m_Window;
}

auto Renderer::GetCurrentRenderPass() const -> SDL_GPURenderPass*
{
    return m_CurrentRenderPass;
}

auto Renderer::GetCurrentCommandBuffer() const -> SDL_GPUCommandBuffer*
{
    return m_CurrentCommandBuffer;
}
