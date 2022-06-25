#include "Camera.h"

Camera::Camera() : m_Position(glm::vec3(0.0f, 0.0f, 15.0f)), m_Rotation(glm::vec3(0.0f, -90.0f, 0.0f)), m_Direction(glm::vec3(0.0f, 0.0f, -1.0f)), m_FOV(52.0f)
{
    m_LookAt = m_Direction + m_Position;
    m_ViewMatrix = glm::lookAt(m_Position, m_LookAt, glm::vec3(0.0f, 1.0f, 0.0f));
    m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), 1.0f, 0.1f, 8000.0f);
}

void Camera::SetPosition(const glm::vec3& position) { m_Position = position; m_LookAt = m_Direction + m_Position; RecalculateViewMatrix(); }
void Camera::SetDirection(const glm::vec3& direction) { m_Direction = direction; m_LookAt = m_Direction + m_Position; RecalculateViewMatrix(); }
void Camera::SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
void Camera::SetFOV(float fov) { m_FOV = fov; RecalculateProjectionMatrix(); }

const glm::vec3& Camera::GetPosition() const { return m_Position; }
const glm::vec3& Camera::GetRotation() const { return m_Rotation; }
const glm::vec3& Camera::GetDirection() const { return m_Direction; }
const glm::mat4& Camera::GetViewMatrix() const { return m_ViewMatrix; }
const glm::mat4& Camera::GetProjectionMatrix() const { return m_ProjectionMatrix; }
float Camera::GetFOV() const { return m_FOV; }

void Camera::RecalculateViewMatrix()
{
    m_ViewMatrix = glm::lookAt(m_Position, m_LookAt, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::RecalculateProjectionMatrix()
{
    // Perspective projection
    m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), 1.0f, 0.1f, 8000.0f);
}