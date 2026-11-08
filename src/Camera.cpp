#include "Camera.hpp"

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_scancode.h>
#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(
    const glm::vec3& position, const float aspectRatio, const float yawDegrees,
    const float pitchDegrees
)
    : m_Position(position)
    , m_Yaw(yawDegrees)
    , m_Pitch(pitchDegrees)
    , m_AspectRatio(aspectRatio)
{
    UpdateCameraVectors();
}

void Camera::Update(const float deltaTime)
{
    auto deltaX = 0.0f;
    auto deltaY = 0.0f;
    SDL_GetRelativeMouseState(&deltaX, &deltaY);

    if (deltaX != 0.0f || deltaY != 0.0f)
    {
        ProcessMouseMovement(deltaX, deltaY);
    }

    ProcessKeyboard(deltaTime);
}

void Camera::ProcessKeyboard(const float deltaTime)
{
    const auto* keyState = SDL_GetKeyboardState(nullptr);
    if (keyState == nullptr)
    {
        return;
    }

    auto speed = m_MoveSpeed;
    if (keyState[SDL_SCANCODE_LSHIFT])
    {
        speed *= m_SprintMultiplier;
    }

    const auto velocity = speed * deltaTime;
    auto moveDirection = glm::vec3(0.0f);

    if (keyState[SDL_SCANCODE_W])
    {
        moveDirection += m_Front;
    }
    if (keyState[SDL_SCANCODE_S])
    {
        moveDirection -= m_Front;
    }
    if (keyState[SDL_SCANCODE_D])
    {
        moveDirection += m_Right;
    }
    if (keyState[SDL_SCANCODE_A])
    {
        moveDirection -= m_Right;
    }
    if (keyState[SDL_SCANCODE_E] || keyState[SDL_SCANCODE_SPACE])
    {
        moveDirection += m_Up;
    }
    if (keyState[SDL_SCANCODE_Q] || keyState[SDL_SCANCODE_LCTRL])
    {
        moveDirection -= m_Up;
    }

    if (glm::dot(moveDirection, moveDirection) > 0.0f)
    {
        m_Position += glm::normalize(moveDirection) * velocity;
    }
}

void Camera::ProcessMouseMovement(const float deltaX, const float deltaY, const bool constrainPitch)
{
    m_Yaw += deltaX * m_MouseSensitivity;
    m_Pitch -= deltaY * m_MouseSensitivity;

    if (constrainPitch)
    {
        m_Pitch = std::clamp(m_Pitch, -89.0f, 89.0f);
    }

    UpdateCameraVectors();
}

void Camera::SetAspectRatio(const float aspectRatio) noexcept
{
    if (aspectRatio > 0.0f)
    {
        m_AspectRatio = aspectRatio;
    }
}

void Camera::SetPosition(const glm::vec3& position) noexcept
{
    m_Position = position;
}

void Camera::SetMoveSpeed(const float speed) noexcept
{
    m_MoveSpeed = speed;
}

void Camera::SetMouseSensitivity(const float sensitivity) noexcept
{
    m_MouseSensitivity = sensitivity;
}

void Camera::SetFov(const float fovDegrees) noexcept
{
    m_FovDegrees = std::clamp(fovDegrees, 1.0f, 120.0f);
}

void Camera::SetClipPlanes(const float nearPlane, const float farPlane) noexcept
{
    m_NearPlane = nearPlane;
    m_FarPlane = farPlane;
}

auto Camera::GetViewMatrix() const noexcept -> glm::mat4
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

auto Camera::GetProjectionMatrix() const noexcept -> glm::mat4
{
    return glm::perspective(glm::radians(m_FovDegrees), m_AspectRatio, m_NearPlane, m_FarPlane);
}

auto Camera::GetViewProjectionMatrix() const noexcept -> glm::mat4
{
    return GetProjectionMatrix() * GetViewMatrix();
}

auto Camera::GetPosition() const noexcept -> const glm::vec3&
{
    return m_Position;
}

auto Camera::GetFront() const noexcept -> const glm::vec3&
{
    return m_Front;
}

auto Camera::GetUp() const noexcept -> const glm::vec3&
{
    return m_Up;
}

auto Camera::GetRight() const noexcept -> const glm::vec3&
{
    return m_Right;
}

auto Camera::GetYaw() const noexcept -> float
{
    return m_Yaw;
}

auto Camera::GetPitch() const noexcept -> float
{
    return m_Pitch;
}

auto Camera::GetFov() const noexcept -> float
{
    return m_FovDegrees;
}

void Camera::UpdateCameraVectors() noexcept
{
    const auto yawRad = glm::radians(m_Yaw);
    const auto pitchRad = glm::radians(m_Pitch);

    const auto front = glm::vec3{
        std::cos(yawRad) * std::cos(pitchRad), std::sin(pitchRad),
        std::sin(yawRad) * std::cos(pitchRad)
    };

    m_Front = glm::normalize(front);
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
