#pragma once
#include "common.hpp"

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#define LOG_VEC3(vec)                                                          \
  LOG_INFO(#vec ": [{:.4}, {:.4}, {:.4}]", vec.x, vec.y, vec.z);

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

  glm::vec3 m_eye{0, 0, 5};
  glm::vec3 m_lookAt{0, 0, 0};
  glm::vec3 m_up{0, 1, 0};

  float m_yam{0.f}, m_pitch{0.f};

  float m_moveScale{1.0f}, m_mouseSensitivity{.05f}, m_zoom{45.f};

  Camera() {}

  Camera(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 up)
      : m_eye{eye}, m_lookAt{lookAt}, m_up{up} {};

  glm::mat4 viewMat() {
    return glm::lookAtRH(m_eye, m_lookAt, m_up);
  }
  glm::vec3 front() {
    return glm::normalize(m_lookAt - m_eye);
  }
  glm::vec3 right() {
    return (glm::cross(front(), m_up));
  }

  glm::mat4 projMat(float aspect) {
    glm::mat4 ret = glm::perspective(m_zoom, aspect, 0.1f, 10000.f);
    ret[1][1] *= -1;
    return ret;
  }

  void processFlyMove(MoveDirection dir, float time = 1.f);

  void processFlyRotation(float xOffset, float yOffset);

  void processArcBallRotation(float xOffset, float yOffset, float viewportW,
                              float viewportH);
  void processArcBallZoom(float offset);
  void processArcBallMove(float xOffset, float yOffset);
};
}; // namespace myvk::data