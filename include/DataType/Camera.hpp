#pragma once
#include "common.hpp"

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#define LOG_VEC3(vec) LOG_INFO(#vec ": [{:.4}, {:.4}, {:.4}]", vec.x, vec.y, vec.z);



namespace myvk::data {
struct Camera {
  enum class MoveDirection {
    eForward,
    eBackward,
    eLeft,
    eRight,
    eUp,
    eDown,
  };

  glm::vec3 m_position{0.f, 0.f, 2.f};
  glm::vec3 m_front{0.f, 0.f, 1.f};
  glm::vec3 m_up{0, 1, 0};
  glm::vec3 m_right{1, 0, 0};
  glm::vec3 m_worldUp{0, -1, 0};

  float m_yam{0.f}, m_pitch{0.f};

  float m_movementSpeed{2.5f}, m_mouseSensitivity{.1f}, m_zoom{45.f};

  Camera() {
    updateCameraVectors();
  };

  Camera(glm::vec3 position, glm::vec3 front, glm::vec3 worldUp)
      : m_position{position}, m_front{front}, m_worldUp{worldUp} {
  }

  glm::mat4 viewMat() {
    return glm::lookAtRH(m_position, m_position + m_front, m_up);
  }

  void move(MoveDirection dir, float time = 1.f);
  void processMouseMovement(float xOffset, float yOffset);
private:
  void updateCameraVectors();
};
}; // namespace myvk::data