#include "Camera.hpp"
#include "Image.hpp"
#include "IndexBuffer.hpp"
#include "Renderer.hpp"
#include "ShaderProgram.hpp"
#include "Texture2D.hpp"
#include "Types.hpp"
#include "VertexBuffer.hpp"
#include "Window.hpp"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <array>
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

auto main() -> int
{
    constexpr auto windowWidth = 1280u;
    constexpr auto windowHeight = 720u;

    auto window = Window(windowWidth, windowHeight, "AstroEngine");
    window.SetRelativeMouseMode(true);
    auto renderer = Renderer(window.GetNativeHandle());

    const auto vertices = std::array<Graphics::Vertex, 24>{
        {// Front face (+Z)
         {-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
         {0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},
         {0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
         {-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},

         // Back face (-Z)
         {0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
         {-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},
         {-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
         {0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},

         // Left face (-X)
         {-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
         {-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},
         {-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
         {-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},

         // Right face (+X)
         {0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
         {0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},
         {0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
         {0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},

         // Top face (+Y)
         {-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
         {0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},
         {0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
         {-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},

         // Bottom face (-Y)
         {-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
         {0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},
         {0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
         {-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f}
        }
    };

    const auto indices = std::vector<uint32_t>{
        0,  1,  2,  0,  2,  3,  // Front
        4,  5,  6,  4,  6,  7,  // Back
        8,  9,  10, 8,  10, 11, // Left
        12, 13, 14, 12, 14, 15, // Right
        16, 17, 18, 16, 18, 19, // Top
        20, 21, 22, 20, 22, 23  // Bottom
    };

    const auto vertexBuffer = VertexBuffer(renderer.GetDevice(), vertices);
    const auto indexBuffer = IndexBuffer(renderer.GetDevice(), indices);
    const auto shaderProgram =
        ShaderProgram(renderer.GetDevice(), window.GetNativeHandle(), "shaders/triangle.hlsl");

    const auto image = Graphics::Image("assets/image.jpg");
    const auto texture = Graphics::Texture2D(renderer.GetDevice(), image);

    const auto aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    auto camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f), aspectRatio);

    auto model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-35.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    constexpr auto clearColor = std::array<float, 4>{0.08f, 0.09f, 0.12f, 1.0f};

    auto lastTime = window.GetTime();

    while (window.PollEvents())
    {
        const auto currentTime = window.GetTime();
        const auto deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        const auto pixelSize = window.GetPixelSize();
        if (pixelSize.width > 0 && pixelSize.height > 0)
        {
            camera.SetAspectRatio(
                static_cast<float>(pixelSize.width) / static_cast<float>(pixelSize.height)
            );
        }

        camera.Update(deltaTime);
        const auto mvp = camera.GetViewProjectionMatrix() * model;

        auto* renderPass = renderer.BeginFrame(clearColor);
        if (renderPass != nullptr)
        {
            shaderProgram.Bind(renderPass);
            SDL_PushGPUVertexUniformData(renderer.GetCurrentCommandBuffer(), 0, &mvp, sizeof(mvp));
            texture.Bind(renderPass, 0);
            vertexBuffer.Bind(renderPass);
            indexBuffer.Bind(renderPass);

            SDL_DrawGPUIndexedPrimitives(renderPass, indexBuffer.GetIndexCount(), 1, 0, 0, 0);

            renderer.EndFrame();
        }
    }

    return 0;
}
