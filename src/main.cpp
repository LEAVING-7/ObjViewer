#include "myvk-bootstrap/Application.hpp"
#include "pch.hpp"

std::vector g_instanceExtensionNames{
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
};

std::vector g_layerNames{"VK_LAYER_KHRONOS_validation"};

std::vector g_deviceExtensionNames{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

int main() {
  myvk_bs::Application *appObj = myvk_bs::Application::GetInstance();
  appObj->initialize();
  LOG_INFO("initialize successfully");
  
  appObj->prepare();
  LOG_INFO("prepare successfully");

  bool shouldWindowClose = false;
  while (!shouldWindowClose) {
    appObj->update();
    shouldWindowClose = appObj->render();
  }
  appObj->deInitialize();
}