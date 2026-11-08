#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

class Camera final
{
public:
    explicit Camera(
        const glm::vec3& position = glm::vec3(0.0f, 0.0f, 3.0f),
        float aspectRatio = 16.0f / 9.0f,
        float yawDegrees = -90.0f,
        float pitchDegrees = 0.0f
    );
    ~Camera() = default;

    Camera(const Camera&) = default;
    auto operator=(const Camera&) -> Camera& = default;

    Camera(Camera&&) noexcept = default;
    auto operator=(Camera&&) noexcept -> Camera& = default;

    void Update(float deltaTime);
    void ProcessKeyboard(float deltaTime);
    void ProcessMouseMovement(float deltaX, float deltaY, bool constrainPitch = true);

    void SetAspectRatio(float aspectRatio) noexcept;
    void SetPosition(const glm::vec3& position) noexcept;
    void SetMoveSpeed(float speed) noexcept;
    void SetMouseSensitivity(float sensitivity) noexcept;
    void SetFov(float fovDegrees) noexcept;
    void SetClipPlanes(float nearPlane, float farPlane) noexcept;

    [[nodiscard]] auto GetViewMatrix() const noexcept -> glm::mat4;
    [[nodiscard]] auto GetProjectionMatrix() const noexcept -> glm::mat4;
    [[nodiscard]] auto GetViewProjectionMatrix() const noexcept -> glm::mat4;

    [[nodiscard]] auto GetPosition() const noexcept -> const glm::vec3&;
    [[nodiscard]] auto GetFront() const noexcept -> const glm::vec3&;
    [[nodiscard]] auto GetUp() const noexcept -> const glm::vec3&;
    [[nodiscard]] auto GetRight() const noexcept -> const glm::vec3&;
    [[nodiscard]] auto GetYaw() const noexcept -> float;
    [[nodiscard]] auto GetPitch() const noexcept -> float;
    [[nodiscard]] auto GetFov() const noexcept -> float;

private:
    void UpdateCameraVectors() noexcept;

    glm::vec3 m_Position{0.0f, 0.0f, 3.0f};
    glm::vec3 m_Front{0.0f, 0.0f, -1.0f};
    glm::vec3 m_Up{0.0f, 1.0f, 0.0f};
    glm::vec3 m_Right{1.0f, 0.0f, 0.0f};
    glm::vec3 m_WorldUp{0.0f, 1.0f, 0.0f};

    float m_Yaw{-90.0f};
    float m_Pitch{0.0f};

    float m_MoveSpeed{4.0f};
    float m_SprintMultiplier{2.5f};
    float m_MouseSensitivity{0.1f};

    float m_FovDegrees{45.0f};
    float m_AspectRatio{16.0f / 9.0f};
    float m_NearPlane{0.1f};
    float m_FarPlane{100.0f};
};
