#include "GUI/MainWindow.hpp"

namespace myvk::gui {
void MainWindow::create(int width, int height, ccstr title,
                        GLFWmonitor* monitor) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  m_window = glfwCreateWindow(width, height, title, monitor, nullptr);
  m_width  = width;
  m_height = height;
}

void MainWindow::updateExtent() {
  auto [width, height] = getFrameBufferSize();
  m_width              = width;
  m_height             = height;
}

void MainWindow::destroy() {
  glfwDestroyWindow(m_window);
}
bool MainWindow::shouldClose() {
  return glfwWindowShouldClose(m_window);
}
bool MainWindow::isFramebufferResized() {
  return m_framebufferResized;
}

std::pair<int, int> MainWindow::getWindowSize() {
  std::pair<int, int> extent;
  glfwGetWindowSize(m_window, &extent.first, &extent.second);
  return extent;
}
std::pair<int, int> MainWindow::getFrameBufferSize() {
  std::pair<int, int> ret;
  glfwGetFramebufferSize(m_window, &ret.first, &ret.second);
  return ret;
}

void MainWindow::waitEvents() {
  glfwWaitEvents();
}

void MainWindow::setUserPointer(GLFWwindow* m_window, void* pointer) {
  glfwSetWindowUserPointer(m_window, pointer);
}

VkSurfaceKHR MainWindow::createSurface(VkInstance instance) {
  VkSurfaceKHR ret;
  glfwCreateWindowSurface(instance, m_window, nullptr, &ret);
  return ret;
}

MainWindow& MainWindow::setErrorCallback(GLFWerrorfun callback) {
  glfwSetErrorCallback(callback);
  return *this;
}

MainWindow& MainWindow::setKeyCallback(GLFWkeyfun callback) {
  glfwSetKeyCallback(m_window, callback);
  return *this;
}

MainWindow& MainWindow::setWindowSizeCallback(GLFWwindowsizefun callback) {
  glfwSetWindowSizeCallback(m_window, callback);
  return *this;
}

MainWindow& MainWindow::setCursorPosCallback(GLFWcursorposfun callback) {
  glfwSetCursorPosCallback(m_window, callback);
  return *this;
}

MainWindow&
MainWindow::setFramebufferSizeCallback(GLFWframebuffersizefun callback) {
  glfwSetFramebufferSizeCallback(m_window, callback);
  return *this;
}

MainWindow& MainWindow::setInputMode(int mode, int value) {
  glfwSetInputMode(m_window, mode, value);
  return *this;
}

void MainWindow::updateCamera(data::Camera& camera) {
  if (getKey(GLFW_KEY_W) == GLFW_PRESS) {
    camera.move(data::Camera::MoveDirection::eForward);
  }
  if (getKey(GLFW_KEY_A) == GLFW_PRESS) {
    camera.move(data::Camera::MoveDirection::eLeft);
  }
  if (getKey(GLFW_KEY_S) == GLFW_PRESS) {
    camera.move(data::Camera::MoveDirection::eBackward);
  }
  if (getKey(GLFW_KEY_D) == GLFW_PRESS) {
    camera.move(data::Camera::MoveDirection::eRight);
  }
  if (getKey(GLFW_KEY_Q) == GLFW_PRESS) {
    camera.move(data::Camera::MoveDirection::eUp);
  }
  if (getKey(GLFW_KEY_E) == GLFW_PRESS) {
    camera.move(data::Camera::MoveDirection::eDown);
  }
}
} // namespace myvk::gui