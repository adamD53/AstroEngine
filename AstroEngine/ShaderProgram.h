#pragma once
#include <string_view>
#include <string>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

class ShaderProgram
{
public:
	ShaderProgram(ID3D11Device* device, std::string_view filePath);
	~ShaderProgram();

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;

	auto Bind(ID3D11DeviceContext* context) -> void;

private:
    ID3D11VertexShader* m_VertexShader;
    ID3D11PixelShader* m_PixelShader;
    ID3D11InputLayout* m_InputLayout;
	std::wstring m_FilePath;

	auto InitializeVertexShader(ID3D11Device* device) -> void;
	auto InitializePixelShader(ID3D11Device* device) -> void;
};
