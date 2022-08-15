#include "GUI/MainWindow.hpp"

namespace myvk::gui {
void MainWindow::create(int width, int height, ccstr title,
                        GLFWmonitor* monitor) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  m_window = glfwCreateWindow(width, height, title, monitor, nullptr);
}

void MainWindow::destroy() {
  glfwDestroyWindow(m_window);
}
bool MainWindow::shouldClose() {
  return glfwWindowShouldClose(m_window);
}

VkSurfaceKHR MainWindow::createSurface(VkInstance instance) {
  VkSurfaceKHR ret;
  glfwCreateWindowSurface(instance, m_window, nullptr, &ret);
  return ret;
}

void MainWindow::setErrorCallback(GLFWerrorfun callback) {
  glfwSetErrorCallback(callback);
}

void MainWindow::setKeyCallback(GLFWkeyfun callback) {
  glfwSetKeyCallback(m_window, callback);
}

void MainWindow::setWindowSizeCallback(GLFWwindowsizefun callback) {
  glfwSetWindowSizeCallback(m_window, callback);
}
void MainWindow::setCursorPosCallback(GLFWcursorposfun callback) {
  glfwSetCursorPosCallback(m_window, callback);
}
} // namespace myvk::gui