#pragma once
#include <cstdint>

#include <wrl/client.h>

struct HWND__;
typedef HWND__* HWND;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

class Renderer
{
public:
	Renderer(HWND windowNativeHandle, const uint32_t windowWidth, const uint32_t windowHeight);
    ~Renderer();

	Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    auto GetDevice() const -> ID3D11Device* { return m_Device.Get(); }
    auto GetContext() const -> ID3D11DeviceContext* { return m_Context.Get(); }
    auto GetSwapChain() const -> IDXGISwapChain* { return m_SwapChain.Get(); }

    auto BeginFrame(float* rgba) -> void;
    auto EndFrame() -> void;

private:
    Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
};

