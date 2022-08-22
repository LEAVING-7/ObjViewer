#include "GUI/MainWindow.hpp"

using namespace std::chrono_literals;

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

void MainWindow::updateFpsCamera(data::Camera& camera) {
  if (!getAttrib(GLFW_HOVERED)) {
    return;
  }

  if (getKeyPressed(GLFW_KEY_W)) {
    camera.processFlyMove(data::Camera::MoveDirection::eForward);
  }
  if (getKeyPressed(GLFW_KEY_A)) {
    camera.processFlyMove(data::Camera::MoveDirection::eLeft);
  }
  if (getKeyPressed(GLFW_KEY_S)) {
    camera.processFlyMove(data::Camera::MoveDirection::eBackward);
  }
  if (getKeyPressed(GLFW_KEY_D)) {
    camera.processFlyMove(data::Camera::MoveDirection::eRight);
  }
  if (getKeyPressed(GLFW_KEY_Q)) {
    camera.processFlyMove(data::Camera::MoveDirection::eUp);
  }
  if (getKeyPressed(GLFW_KEY_E)) {
    camera.processFlyMove(data::Camera::MoveDirection::eDown);
  }

  if (getKeyPressed(GLFW_KEY_LEFT_ALT)) {
    setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    return;
  } else {
    setInputMode(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  }

  double xpos, ypos;
  glfwGetCursorPos(m_window, &xpos, &ypos);

  int windowWidth  = m_width / 2;
  int windowHeight = m_height / 2;

  camera.processFlyRotation(xpos - (float)windowWidth,
                            ypos - (float)windowHeight);
  glfwSetCursorPos(m_window, windowWidth, windowHeight);
}

void MainWindow::updateNormalCamera(data::Camera& cam) {

  double xCurrent0, yCurrent0;
  getMousePos(xCurrent0, yCurrent0);
  float xCurrent = (float)xCurrent0, yCurrent = (float)yCurrent0;

  static float xLastPos = xCurrent, yLastPos = yCurrent;

  float xOffset = xCurrent - xLastPos;
  float yOffset = yCurrent - yLastPos;

  if (getMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
    cam.processArcBallMove(xOffset, yOffset);
  }
  if (getMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
    cam.processArcBallRotation(xOffset, yOffset, (float)m_width,
                               (float)m_height);
  }

  xLastPos = xCurrent;
  yLastPos = yCurrent;
}

} // namespace myvk::gui