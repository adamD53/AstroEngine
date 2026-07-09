#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "ShaderProgram.h"
#include "CoreUtils.h"

#include <array>
#include <iostream>
#include <string>

ShaderProgram::ShaderProgram(ID3D11Device* device, std::string_view filePath)
    : m_VertexShader(nullptr), m_PixelShader(nullptr), m_InputLayout(nullptr)
{
    m_FilePath = std::wstring(filePath.begin(), filePath.end());

    InitializeVertexShader(device);
    InitializePixelShader(device);
}

ShaderProgram::~ShaderProgram()
{
    if (m_VertexShader) m_VertexShader->Release();
    if (m_PixelShader) m_PixelShader->Release();
    if (m_InputLayout) m_InputLayout->Release();
}

auto ShaderProgram::InitializeVertexShader(ID3D11Device* device) -> void
{
    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    HRESULT hr = D3DCompileFromFile(m_FilePath.c_str(), nullptr, nullptr, "VS_Main", "vs_5_0", 0, 0, &vsBlob, &errorBlob);

    if (FAILED(hr)) {
        std::string errorMessage = "Vertex shader compilation failed.";
        if (errorBlob) {
            errorMessage += " Details: ";
            errorMessage += static_cast<char*>(errorBlob->GetBufferPointer());
            OutputDebugStringA(errorMessage.c_str());
            errorBlob->Release();
        }
        Utils::core_assert(false, errorMessage);
    }
    if (errorBlob) errorBlob->Release();

    device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_VertexShader);

    std::array<D3D11_INPUT_ELEMENT_DESC, 4> ied = { {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, (3 * sizeof(float)), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, (7 * sizeof(float)), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "INSTANCE_POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
    } };

    device->CreateInputLayout(ied.data(), static_cast<UINT>(ied.size()), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_InputLayout);
    vsBlob->Release();
}

auto ShaderProgram::InitializePixelShader(ID3D11Device* device) -> void
{
    ID3DBlob* psBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    HRESULT hr = D3DCompileFromFile(m_FilePath.c_str(), nullptr, nullptr, "PS_Main", "ps_5_0", 0, 0, &psBlob, &errorBlob);

    if (FAILED(hr)) {
        std::string errorMessage = "Pixel shader compilation failed.";
        if (errorBlob) {
            errorMessage += " Details: ";
            errorMessage += static_cast<char*>(errorBlob->GetBufferPointer());
            OutputDebugStringA(errorMessage.c_str());
            errorBlob->Release();
        }
        Utils::core_assert(false, errorMessage);
    }
    if (errorBlob) errorBlob->Release();

    device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_PixelShader);
    psBlob->Release();
}

auto ShaderProgram::Bind(ID3D11DeviceContext* context) -> void
{
    if (m_VertexShader != nullptr && m_PixelShader != nullptr) {
        context->IASetInputLayout(m_InputLayout);
		context->VSSetShader(m_VertexShader, nullptr, 0);
		context->PSSetShader(m_PixelShader, nullptr, 0);
    }
}


