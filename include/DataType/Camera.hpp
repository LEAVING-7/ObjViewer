#pragma once
#include "common.hpp"

#include <chrono>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

void inline log_vec3(glm::vec3& vec) {
  LOG_INFO("[{}, {}, {}]", vec.x, vec.y, vec.z);
}

namespace myvk::data {
struct Camera {
  enum class MoveDirection {
    eForward,
    eBackward,
    eLeft,
    eRight,
  };

  glm::vec3 position{0.f, 0.f, 0.f};
  glm::vec3 front{0.f, 0.f, 1.f};
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp{0.f, 0.f, 1.f};

  float yam{0.f}, pitch{0.f};

  float movementSpeed{2.5f}, mouseSensitivity{1.f}, zoom{45.f};

  Camera() {
    updateCameraVectors();
    log_vec3(up);
    log_vec3(right);
    log_vec3(front);
  };

  Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up)
      : position{position}, front{front}, up{up} {}

  glm::mat4 viewMat() {
    return glm::lookAt(position, position + front, up);
  }

  void move(MoveDirection dir);
  void processMouseMovement(float xOffset, float yOffset);

private:
  void updateCameraVectors();
};
}; // namespace myvk::data