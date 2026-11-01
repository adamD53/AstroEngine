#include "Window.hpp"

#include "CoreUtils.hpp"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_timer.h>
#include <string>

Window::Window(const uint32_t width, const uint32_t height, const std::string_view title)
    : m_LogicalWidth(width)
    , m_LogicalHeight(height)
{
    const auto initialized = SDL_InitSubSystem(SDL_INIT_VIDEO);
    Utils::core_assert(initialized, "Failed to initialize SDL3 video subsystem.");

    const auto titleString = std::string(title);
    constexpr auto windowFlags = SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE;

    auto* rawWindow = SDL_CreateWindow(
        titleString.c_str(), static_cast<int>(m_LogicalWidth), static_cast<int>(m_LogicalHeight), windowFlags
    );
    Utils::core_assert(rawWindow != nullptr, "Failed to create SDL3 window.");
    m_Window.reset(rawWindow);
}

Window::~Window()
{
    m_Window.reset();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

auto Window::PollEvents() -> bool
{
    const auto myWindowID = SDL_GetWindowID(m_Window.get());

    auto event = SDL_Event{};
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            m_IsOpen = false;
        }
        else if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == myWindowID)
        {
            m_IsOpen = false;
        }
        else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)
        {
            m_IsOpen = false;
        }
    }

    return m_IsOpen;
}

auto Window::IsOpen() const -> bool
{
    return m_IsOpen;
}

auto Window::GetNativeHandle() const -> SDL_Window*
{
    return m_Window.get();
}

auto Window::GetPixelSize() const -> Graphics::Extent2D
{
    auto width = 0;
    auto height = 0;
    SDL_GetWindowSizeInPixels(m_Window.get(), &width, &height);
    return Graphics::Extent2D{
        .width = static_cast<uint32_t>(width > 0 ? width : m_LogicalWidth),
        .height = static_cast<uint32_t>(height > 0 ? height : m_LogicalHeight)
    };
}

auto Window::GetLogicalSize() const -> Graphics::Extent2D
{
    return Graphics::Extent2D{.width = m_LogicalWidth, .height = m_LogicalHeight};
}

auto Window::GetPixelWidth() const -> uint32_t
{
    return GetPixelSize().width;
}

auto Window::GetPixelHeight() const -> uint32_t
{
    return GetPixelSize().height;
}

auto Window::GetLogicalWidth() const -> uint32_t
{
    return GetLogicalSize().width;
}

auto Window::GetLogicalHeight() const -> uint32_t
{
    return GetLogicalSize().height;
}

auto Window::GetTime() const -> double
{
    return static_cast<double>(SDL_GetTicksNS()) / 1e9;
}
