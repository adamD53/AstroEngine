#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "Window.h"
#include "CoreUtils.h"

#include <source_location>
#include <iostream>

static int s_GLFWWindowCount = 0;

Window::Window(const uint32_t width, const uint32_t height, const std::string_view title)
	: m_Width(width), m_Height(height), m_Window(nullptr)
{
	if (s_GLFWWindowCount == 0) {
		auto init = glfwInit();
		Utils::core_assert(init != 0, "Failed to initialize GLFW window.");
	}
	s_GLFWWindowCount++;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_Window = glfwCreateWindow(m_Width, m_Height, std::string(title).data(), nullptr, nullptr);
	Utils::core_assert(m_Window != nullptr, "Failed to initialize GLFW window.");
}

Window::~Window()
{
	if (m_Window) {
		glfwDestroyWindow(m_Window);
	}
	
	s_GLFWWindowCount--;
	if (s_GLFWWindowCount == 0) {
		glfwTerminate();
	}
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
