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

void* MainWindow::getUserPointer(GLFWwindow* m_window) {
  return glfwGetWindowUserPointer(m_window);
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

} // namespace myvk::gui