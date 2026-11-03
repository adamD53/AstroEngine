#pragma once

#include "Types.hpp"

#include <cstddef>
#include <filesystem>
#include <memory>
#include <span>

namespace Graphics
{

struct StbiImageDeleter final
{
    void operator()(unsigned char* pixels) const noexcept;
};

class Image final
{
public:
    explicit Image(const std::filesystem::path& filePath, bool flipVertically = false);
    ~Image() = default;

    Image(const Image&) = delete;
    auto operator=(const Image&) -> Image& = delete;

    Image(Image&&) noexcept = default;
    auto operator=(Image&&) noexcept -> Image& = default;

    [[nodiscard]] auto GetExtent() const noexcept -> Extent2D;
    [[nodiscard]] auto GetChannels() const noexcept -> uint32_t;
    [[nodiscard]] auto GetByteSize() const noexcept -> size_t;
    [[nodiscard]] auto GetData() const noexcept -> std::span<const std::byte>;
    [[nodiscard]] auto IsValid() const noexcept -> bool;

private:
    Extent2D m_Extent{};
    uint32_t m_Channels{4};
    std::unique_ptr<unsigned char, StbiImageDeleter> m_Pixels{nullptr};
};

} // namespace Graphics
