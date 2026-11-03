#pragma once

#include <cstdint>

namespace Graphics
{

struct Extent2D final
{
    uint32_t width{0};
    uint32_t height{0};
};

struct Vertex final
{
    float x{0.0f};
    float y{0.0f};
    float z{0.0f};

    float r{1.0f};
    float g{1.0f};
    float b{1.0f};
    float a{1.0f};

    float u{0.0f};
    float v{0.0f};
};

} // namespace Graphics
