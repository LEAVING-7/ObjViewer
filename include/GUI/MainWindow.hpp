#pragma once
#include "common.hpp"
#include "pch.hpp"

namespace myvk::gui {
class MainWindow {
public:
  GLFWwindow* m_window;
  u32         m_width, m_height;

  bool m_framebufferResized{false};

  void create(int width, int height, ccstr title, GLFWmonitor* monitor);
  void destroy();
  void updateExtent();

  VkSurfaceKHR createSurface(VkInstance instance);
  bool         shouldClose();
  bool         isFramebufferResized();

  std::pair<int, int> getWindowSize();
  std::pair<int, int> getFrameBufferSize();

  static void waitEvents();

  static void setUserPointer(GLFWwindow* m_window, void* pointer);
  
  template <typename T> static T getUserPointer(GLFWwindow* m_window) {
    return reinterpret_cast<T>(glfwGetWindowUserPointer(m_window));
  }

  MainWindow& setErrorCallback(GLFWerrorfun callback =
                                   [](int error_code, const char* description) {
                                     LOG_INFO("[glfw error: {}]: {}",
                                              error_code, description);
                                   });

  MainWindow& setKeyCallback(GLFWkeyfun callback);
  MainWindow& setWindowSizeCallback(GLFWwindowsizefun callback);
  MainWindow& setCursorPosCallback(GLFWcursorposfun callback);
  MainWindow& setFramebufferSizeCallback(GLFWframebuffersizefun callback);
};
} // namespace myvk::gui