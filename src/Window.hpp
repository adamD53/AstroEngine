#pragma once

#include "SDLDeleters.hpp"
#include "Types.hpp"

#include <SDL3/SDL_video.h>
#include <cstdint>
#include <memory>
#include <string_view>

class Window
{
public:
    Window(uint32_t width, uint32_t height, std::string_view title);
    ~Window();

    Window(const Window&) = delete;
    auto operator=(const Window&) -> Window& = delete;

    Window(Window&&) noexcept = default;
    auto operator=(Window&&) noexcept -> Window& = default;

    auto PollEvents() -> bool;
    void SetRelativeMouseMode(bool enabled) const;

    auto IsOpen() const -> bool;
    auto GetNativeHandle() const -> SDL_Window*;
    auto GetPixelSize() const -> Graphics::Extent2D;
    auto GetLogicalSize() const -> Graphics::Extent2D;
    auto GetPixelWidth() const -> uint32_t;
    auto GetPixelHeight() const -> uint32_t;
    auto GetLogicalWidth() const -> uint32_t;
    auto GetLogicalHeight() const -> uint32_t;
    auto GetTime() const -> double;

private:
    std::unique_ptr<SDL_Window, SDLWindowDeleter> m_Window;
    uint32_t m_LogicalWidth{0};
    uint32_t m_LogicalHeight{0};
    bool m_IsOpen{true};
};
