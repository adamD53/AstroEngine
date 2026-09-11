#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>

#pragma warning(push, 0)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma warning(pop)

#include "QuadRenderer.h"
#include "Renderer.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Types.h"
#include "Window.h"

#include <array>
#include <vector>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

auto main() -> int
{
    constexpr auto windowWidth = 1280u;
    constexpr auto windowHeight = 720u;
    constexpr auto maxInstances = 1000000u;

    std::vector<Graphics::Vertex> vertices = {
        {{-0.05f, 0.05f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
         {0.05f, 0.05f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
         {0.05f, -0.05f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
         {-0.05f, -0.05f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f}}
    };

    std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};

    std::vector<Graphics::InstanceData> instances{};
    instances.resize(maxInstances);

    auto index = 0u;
    for (auto y{0u}; y < (maxInstances / 1000); ++y)
    {
        for (auto x{0u}; x < (maxInstances / 1000); ++x)
        {
            auto finalX = (static_cast<float>(x) / 50.0f) - 1.0f;
            auto finalY = (static_cast<float>(y) / 50.0f) - 1.0f;

            instances[index].x = finalX;
            instances[index].y = finalY;
            instances[index].z = 0.0f;

            index++;
        }
    }

    Window window(windowWidth, windowHeight, "GLFW Window DirectX");

    Renderer renderer(window.GetNativeHandle(), window.GetWidth(), window.GetHeight());

    QuadRenderer quadMesh(renderer.GetDevice(), vertices, instances, indices);
    instances.clear();

    ShaderProgram shaders(renderer.GetDevice(), "shaders/shaders.hlsl");

    Texture2D texture(renderer.GetDevice(), "assets/image.jpg");

    texture.Bind(renderer.GetContext());
    shaders.Bind(renderer.GetContext());

    std::array<float, 4> clearColor = {0.1f, 0.1f, 0.1f, 1.0f};

    while (window.IsOpen())
    {
        window.PollEvents();

        renderer.BeginFrame(clearColor.data());

        quadMesh.UpdateTime(renderer.GetContext(), static_cast<float>(window.GetTime()));
        quadMesh.Draw(renderer.GetContext(), maxInstances);

        renderer.EndFrame();
    }

    return 0;
}