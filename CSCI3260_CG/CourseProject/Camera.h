#pragma once

#include "Dependencies/glew/glew.h"
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

class Camera
{
public:
    Camera();

    void SetPosition(const glm::vec3&);
    void SetDirection(const glm::vec3&);
    void SetRotation(const glm::vec3&);
    void SetFOV(float);

    const glm::vec3& GetPosition() const;
    const glm::vec3& GetRotation() const;
    const glm::vec3& GetDirection() const;
    const glm::mat4& GetViewMatrix() const;
    const glm::mat4& GetProjectionMatrix() const;
    float GetFOV() const;

private:
    void RecalculateViewMatrix();

    void RecalculateProjectionMatrix();

private:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;

    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Direction;
    glm::vec3 m_LookAt;

    float m_FOV;
};

