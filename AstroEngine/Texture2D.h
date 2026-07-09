#pragma once
#include <cstdint>
#include <string_view>
#include <wrl/client.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;

class Texture2D
{
public:
	Texture2D(ID3D11Device* device, const std::string_view filePath);
	~Texture2D();

	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;

	auto Bind(ID3D11DeviceContext* context) -> void;

private:
	int32_t m_Width, m_Height, m_Channels;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_Texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ResourceView;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerState;

	auto InitializeTexture(ID3D11Device* device, const std::string_view filePath) -> void;
	auto InitializeSampler(ID3D11Device* device) -> void;
	auto InitializeResourceView(ID3D11Device* device) -> void;
};

