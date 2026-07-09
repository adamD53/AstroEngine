#pragma once
#include <windows.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <cstdint>
#include <string_view>

class Window
{
public:
	Window(const uint32_t width, const uint32_t height, const std::string_view title);
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	auto GetNativeHandle() const -> HWND;
	auto IsOpen() const -> bool;
	auto PollEvents() const -> void;

	auto GetWidth() const -> uint32_t { return m_Width; }
	auto GetHeight() const -> uint32_t { return m_Height; }
	auto GetTime() const -> double { return glfwGetTime(); }

private:
	uint32_t m_Width, m_Height;
	GLFWwindow* m_Window;
};

