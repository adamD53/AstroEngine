#pragma once

namespace Graphics
{

struct Vertex final
{
    float x, y, z;
    float r, g, b, a;
    float u, v;
};

struct InstanceData final
{
    float x, y, z;
};
} // namespace Graphics
