#include "DataType/Camera.hpp"

#include <numbers>
namespace myvk::data {

void Camera::processFlyMove(MoveDirection dir, float time) {
  float velocity = 0.1f * time;
  switch (dir) {
  case MoveDirection::eForward:
    m_eye += front() * velocity;
    break;
  case MoveDirection::eBackward:
    m_eye -= front() * velocity;
    break;
  case MoveDirection::eLeft:
    m_eye -= right() * velocity;
    break;
  case MoveDirection::eRight:
    m_eye += right() * velocity;
    break;
  case MoveDirection::eUp:
    m_eye += m_up * velocity;
    break;
  case MoveDirection::eDown:
    m_eye -= m_up * velocity;
    break;
  }
}

void Camera::processFlyRotation(float xOffset, float yOffset) {
  xOffset *= m_mouseSensitivity;
  yOffset *= m_mouseSensitivity;
  m_yam -= xOffset;
  m_pitch -= yOffset;

  if (m_pitch > 89.9f)
    m_pitch = 89.9f;
  if (m_pitch < -89.9f)
    m_pitch = -89.9f;

  float degreeYam = glm::radians(m_yam), degreePitch = glm::radians(m_pitch);
  glm::vec3 newFront = {
      -cos(degreeYam) * cos(degreePitch),
      sin(degreePitch),
      sin(degreeYam) * cos(degreePitch),
  };

  m_lookAt = m_eye + newFront;
}

void Camera::processArcBallRotation(float xOffset, float yOffset,
                                    float viewportW, float viewportH) {
  glm::vec4 pos   = {m_eye, 1.f};
  glm::vec4 pivot = {m_lookAt, 1.f};

  float xAngle = (float) (-xOffset) * (std::numbers::pi * 2 / viewportW);
  float yAngle = (float) (-yOffset) * (std::numbers::pi / viewportH);

  float cosAngle = glm::dot(front(), {0, 1, 0});

  if (cosAngle * (yOffset > 0.f ? -1.f : 1.f) > 0.99)
    yAngle = 0;

  glm::mat4x4 rotationMatrixX(1.0f);
  rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, m_up);

  pos = (rotationMatrixX * (pos - pivot)) + pivot;
  glm::mat4x4 rotationMatrixY(1.0f);
  rotationMatrixY         = glm::rotate(rotationMatrixY, yAngle, right());
  glm::vec3 finalPosition = (rotationMatrixY * (pos - pivot)) + pivot;

  m_eye = finalPosition;
}

void Camera::processArcBallZoom(float offset) {
  offset *= m_mouseSensitivity * 50;

  if (glm::length(m_eye - m_lookAt) > offset) {
    m_eye += front() * offset;
  } 
}

void Camera::processArcBallMove(float xOffset, float yOffset) {
  xOffset *= -m_mouseSensitivity;
  yOffset *= m_mouseSensitivity;

  glm::vec3 up     = glm::cross(right(), front());
  glm::vec3 xyMove = right() * xOffset + up * yOffset;
  m_eye += xyMove;
  m_lookAt += xyMove;
}

} // namespace myvk::data