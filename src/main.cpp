#include "Image.hpp"
#include "IndexBuffer.hpp"
#include "Renderer.hpp"
#include "ShaderProgram.hpp"
#include "Texture2D.hpp"
#include "Types.hpp"
#include "VertexBuffer.hpp"
#include "Window.hpp"

#include <array>
#include <cstdint>
#include <vector>

auto main() -> int
{
    constexpr auto windowWidth = 1280u;
    constexpr auto windowHeight = 720u;

    auto window = Window(windowWidth, windowHeight, "AstroEngine");
    auto renderer = Renderer(window.GetNativeHandle());

    const auto vertices = std::array<Graphics::Vertex, 4>{
        {{-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
         { 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},
         { 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
         {-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f}}
    };

    const auto indices = std::vector<uint32_t>{0, 1, 2, 0, 2, 3};

    const auto vertexBuffer = VertexBuffer(renderer.GetDevice(), vertices);
    const auto indexBuffer = IndexBuffer(renderer.GetDevice(), indices);
    const auto shaderProgram = ShaderProgram(renderer.GetDevice(), window.GetNativeHandle(), "shaders/triangle.hlsl");

    const auto image = Graphics::Image("assets/image.jpg");
    const auto texture = Graphics::Texture2D(renderer.GetDevice(), image);

    constexpr auto clearColor = std::array<float, 4>{0.08f, 0.09f, 0.12f, 1.0f};

    while (window.PollEvents())
    {
        auto* renderPass = renderer.BeginFrame(clearColor);
        if (renderPass != nullptr)
        {
            shaderProgram.Bind(renderPass);
            texture.Bind(renderPass, 0);
            vertexBuffer.Bind(renderPass);
            indexBuffer.Bind(renderPass);

            SDL_DrawGPUIndexedPrimitives(renderPass, indexBuffer.GetIndexCount(), 1, 0, 0, 0);

            renderer.EndFrame();
        }
    }

    return 0;
}
