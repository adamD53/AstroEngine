#include "Renderer.h"

#include "CoreUtils.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>

Renderer::Renderer(HWND windowNativeHandle, const uint32_t windowWidth, const uint32_t windowHeight)
{
    DXGI_SWAP_CHAIN_DESC scDesc = {
        .BufferDesc =
            {.Width = windowWidth,
             .Height = windowHeight,
             .RefreshRate = {.Numerator = 60, .Denominator = 1},
             .Format = DXGI_FORMAT_R8G8B8A8_UNORM},
        .SampleDesc = {.Count = 1},
        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount = 1,
        .OutputWindow = windowNativeHandle,
        .Windowed = TRUE
    };

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    auto hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, nullptr, 0, D3D11_SDK_VERSION, &scDesc,
        &m_SwapChain, &m_Device, nullptr, &m_Context
    );

    Utils::core_assert(SUCCEEDED(hr), "DirectX Device and Swap Chain initialization error.");

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));

    m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_RenderTargetView);

    m_Context->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);

    D3D11_VIEWPORT viewport = {
        .TopLeftX = 0.0f,
        .TopLeftY = 0.0f,
        .Width = static_cast<FLOAT>(windowWidth),
        .Height = static_cast<FLOAT>(windowHeight),
        .MinDepth = 0.0f,
        .MaxDepth = 1.0f
    };
    m_Context->RSSetViewports(1, &viewport);
}

auto Renderer::BeginFrame(float* rgba) -> void
{
    m_Context->ClearRenderTargetView(m_RenderTargetView.Get(), rgba);
}

auto Renderer::EndFrame() -> void
{
    m_SwapChain->Present(1, 0);
}

Renderer::~Renderer() = default;
