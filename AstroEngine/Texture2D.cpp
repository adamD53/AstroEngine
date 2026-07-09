#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "Texture2D.h"
#include "CoreUtils.h"

#include "stb_image.h"

Texture2D::Texture2D(ID3D11Device* device, const std::string_view filePath)
{
    InitializeSampler(device);
    InitializeTexture(device, filePath);
    InitializeResourceView(device);
}

Texture2D::~Texture2D() = default;

auto Texture2D::InitializeTexture(ID3D11Device* device, const std::string_view filePath) -> void
{
    const auto pixels = stbi_load(std::string(filePath).c_str(), &m_Width, &m_Height, &m_Channels, STBI_rgb_alpha);
    Utils::core_assert(pixels != nullptr, "Failed to load texture from given path.");

    D3D11_TEXTURE2D_DESC textureDesc = {
        .Width = static_cast<UINT>(m_Width),
        .Height = static_cast<UINT>(m_Height),
        .MipLevels = 1,
        .ArraySize = 1,
        .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
        .SampleDesc = {
            .Count = 1,
            .Quality = 0
        },
        .Usage = D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_SHADER_RESOURCE,
        .CPUAccessFlags = 0
    };
    
    D3D11_SUBRESOURCE_DATA initTexData = {
        .pSysMem = pixels,
        .SysMemPitch = static_cast<UINT>(m_Width * 4)
    };
    
    HRESULT hr = device->CreateTexture2D(&textureDesc, &initTexData, &m_Texture);
    Utils::core_assert(SUCCEEDED(hr), "Failed to create Texture2D");

    stbi_image_free(pixels);
}

auto Texture2D::InitializeSampler(ID3D11Device* device) -> void
{
    D3D11_SAMPLER_DESC samplerDesc = {
        .Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
        .AddressU = D3D11_TEXTURE_ADDRESS_WRAP,
        .AddressV = D3D11_TEXTURE_ADDRESS_WRAP,
        .AddressW = D3D11_TEXTURE_ADDRESS_WRAP
    };
    
    HRESULT hr = device->CreateSamplerState(&samplerDesc, &m_SamplerState);
    Utils::core_assert(SUCCEEDED(hr), "Failed to create Sampler State");
}

auto Texture2D::InitializeResourceView(ID3D11Device* device) -> void
{
    D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc = {
        .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
        .ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D,
        .Texture2D = {
            .MostDetailedMip = 0,
            .MipLevels = 1
        }
    };
    
    HRESULT hr = device->CreateShaderResourceView(m_Texture.Get(), &resourceViewDesc, &m_ResourceView);
    Utils::core_assert(SUCCEEDED(hr), "Failed to create SRV");
}

auto Texture2D::Bind(ID3D11DeviceContext* context) -> void
{
    if (m_ResourceView != nullptr && m_SamplerState != nullptr) {
		context->PSSetShaderResources(0, 1, m_ResourceView.GetAddressOf());
		context->PSSetSamplers(0, 1, m_SamplerState.GetAddressOf());
    }
}


