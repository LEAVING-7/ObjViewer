#include "Application/Application.hpp"
#include "pch.hpp"

#include <memory>

int   allocCnt   = 0;
int   deallocCnt = 0;
void* operator new(size_t size) {
  ++allocCnt;
  return malloc(size);
}

void operator delete(void* ptr) noexcept {
  ++deallocCnt;
  free(ptr);
}

std::vector g_instanceExtensionNames{
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
};

std::vector g_layerNames{"VK_LAYER_KHRONOS_validation"};

std::vector g_deviceExtensionNames{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

int main() {
  {
    myvk::bs::Application* appObj = myvk::bs::Application::GetInstance();
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
  LOG_INFO("{} {}", allocCnt, deallocCnt);
}