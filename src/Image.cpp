#include "Image.hpp"

#include "CoreUtils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Graphics
{

void StbiImageDeleter::operator()(unsigned char* pixels) const noexcept
{
    if (pixels != nullptr)
    {
        stbi_image_free(pixels);
    }
}

Image::Image(const std::filesystem::path& filePath, const bool flipVertically)
{
    const auto resolvedPath = Utils::ResolveAssetPath(filePath);
    const auto pathStr = resolvedPath.string();

    stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

    auto width = 0;
    auto height = 0;
    auto fileChannels = 0;

    auto* rawPixels = stbi_load(pathStr.c_str(), &width, &height, &fileChannels, STBI_rgb_alpha);
    Utils::core_assert(
        rawPixels != nullptr,
        "Failed to load image (" + pathStr + "): " + (stbi_failure_reason() != nullptr ? stbi_failure_reason() : "unknown error")
    );

    m_Extent = Extent2D{
        .width = static_cast<uint32_t>(width),
        .height = static_cast<uint32_t>(height)
    };
    m_Channels = 4u;
    m_Pixels.reset(rawPixels);
}

auto Image::GetExtent() const noexcept -> Extent2D
{
    return m_Extent;
}

auto Image::GetChannels() const noexcept -> uint32_t
{
    return m_Channels;
}

auto Image::GetByteSize() const noexcept -> size_t
{
    return static_cast<size_t>(m_Extent.width) * static_cast<size_t>(m_Extent.height) * static_cast<size_t>(m_Channels);
}

auto Image::GetData() const noexcept -> std::span<const std::byte>
{
    if (!IsValid())
    {
        return {};
    }
    const auto* bytePtr = reinterpret_cast<const std::byte*>(m_Pixels.get());
    return std::span<const std::byte>{bytePtr, GetByteSize()};
}

auto Image::IsValid() const noexcept -> bool
{
    return m_Pixels != nullptr && m_Extent.width > 0 && m_Extent.height > 0;
}

} // namespace Graphics
