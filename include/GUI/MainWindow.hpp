#pragma once
#include "common.hpp"
#include "pch.hpp"

namespace myvk::gui {
class MainWindow {
public:
  GLFWwindow* m_window;
  u32         m_width, m_height;

  void create(int width, int height, ccstr title, GLFWmonitor* monitor);
  void destroy();

  VkSurfaceKHR createSurface(VkInstance instance);
  bool shouldClose();


  void setErrorCallback(GLFWerrorfun callback = [](int         error_code,
                                                   const char* description) {
    LOG_INFO("[glfw error: {}]: {}", error_code, description);
  });
  void setKeyCallback(GLFWkeyfun callback);
  void setWindowSizeCallback(GLFWwindowsizefun callback);
  void setCursorPosCallback(GLFWcursorposfun callback);
};
} // namespace myvk::gui