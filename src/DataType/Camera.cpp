#include "DataType/Camera.hpp"

namespace myvk::data {

void Camera::move(MoveDirection dir, float time) {
  float velocity = 0.05f * time;
  switch (dir) {
  case MoveDirection::eForward:
    m_position += m_front * velocity;
    break;
  case MoveDirection::eBackward:
    m_position -= m_front * velocity;
    break;
  case MoveDirection::eLeft:
    m_position -= m_right * velocity;
    break;
  case MoveDirection::eRight:
    m_position += m_right * velocity;
    break;
  case MoveDirection::eUp:
    m_position += m_up * velocity;
    break;
  case MoveDirection::eDown:
    m_position -= m_up * velocity;
    break;
  }
}
void Camera::processMouseMovement(float xOffset, float yOffset) {
  xOffset *= m_mouseSensitivity;
  yOffset *= m_mouseSensitivity;
  m_yam += xOffset;
  m_pitch -= yOffset;

  if (m_pitch > 89.9f)
    m_pitch = 89.f;
  if (m_pitch < -89.9f)
    m_pitch = -89.f;

  float degreeYam = glm::radians(m_yam), degreePitch = glm::radians(m_pitch);
  glm::vec3 newFront = {
      cos(degreeYam) * cos(degreePitch),
      sin(degreePitch),
      sin(degreeYam) * cos(degreePitch),
  };
  m_front = glm::normalize(newFront);
  m_right = glm::normalize(glm::cross(m_front, m_worldUp));
  m_up    = glm::normalize(glm::cross(m_right, m_front));
}
void Camera::updateCameraVectors() {
  glm::vec3 front{
      cos(glm::radians(m_yam)) * cos(glm::radians(m_pitch)),
      sin(glm::radians(m_pitch)),
      sin(glm::radians(m_yam)) * cos(glm::radians(m_pitch)),
  };
  m_front = glm::normalize(front);
  m_right = glm::normalize(glm::cross(front, m_worldUp));
  m_up    = glm::normalize(glm::cross(m_right, front));
}

} // namespace myvk::data