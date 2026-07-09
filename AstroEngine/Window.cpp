#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "Window.h"
#include "CoreUtils.h"

#include <source_location>
#include <iostream>

Window::Window(const uint32_t width, const uint32_t height, const std::string_view title)
	: m_Width(width), m_Height(height), m_Window(nullptr)
{
	auto init = glfwInit();
	Utils::core_assert(init != 0, "Failed to initialize GLFW window.");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_Window = glfwCreateWindow(m_Width, m_Height, title.data(), nullptr, nullptr);
	Utils::core_assert(m_Window != nullptr, "Failed to initialize GLFW window.");
}

Window::~Window()
{
	if (m_Window) {
		glfwDestroyWindow(m_Window);
	}
    glfwTerminate();
}

auto Window::GetNativeHandle() const -> HWND
{
	return glfwGetWin32Window(m_Window);
}

auto Window::IsOpen() const -> bool
{
	return !glfwWindowShouldClose(m_Window);
}

auto Window::PollEvents() const -> void
{
	glfwPollEvents();
}
