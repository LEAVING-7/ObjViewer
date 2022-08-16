#include "DataType/Camera.hpp"

namespace myvk::data {

void Camera::move(MoveDirection dir) {
  float velocity = 0.1f;
  switch (dir) {
  case MoveDirection::eForward:
    position += front * velocity;
    break;
  case MoveDirection::eBackward:
    position -= front * velocity;
    break;
  case MoveDirection::eLeft:
    position -= right * velocity;
    break;
  case MoveDirection::eRight:
    position += right * velocity;
    break;
  }
}
void Camera::processMouseMovement(float xOffset, float yOffset) {
  xOffset *= mouseSensitivity;
  yOffset *= mouseSensitivity;
  yam += xOffset;
  pitch -= yOffset;

  if (pitch > 89.9f)
    pitch = 89.f;
  if (pitch < -89.9f)
    pitch = -89.f;

  updateCameraVectors();
}
void Camera::updateCameraVectors() {
  glm::vec3 front{
      cos(glm::radians(yam)) * cos(glm::radians(pitch)),
      sin(glm::radians(pitch)),
      sin(glm::radians(yam)) * cos(glm::radians(pitch)),
  };
  front = glm::normalize(front);
  right = glm::normalize(glm::cross(front, worldUp));
  up    = glm::normalize(glm::cross(right, front));
  
}

} // namespace myvk::data