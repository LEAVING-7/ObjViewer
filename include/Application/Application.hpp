#pragma once

#include "bootstrap/BufferAllocator.hpp"
#include "bootstrap/Device.hpp"
#include "bootstrap/Instance.hpp"

#include "Application/Renderer.hpp"

namespace myvk::bs {

class Application {
private:
  static inline std::unique_ptr<Application> sm_instance;
  static inline std::once_flag               sm_onlyOnce;

public:
  Instance                  m_instanceObj;
  std::unique_ptr<Device>   m_deviceObj;
  std::unique_ptr<Renderer> m_rendererObj;
  BufferAllocator           m_allocator;

private:
  bool m_isPrepared;
  bool m_isResizing;

public:
  static Application* GetInstance();

private:
  Application();

public:
  ~Application();

public:
  void initialize();
  void prepare();
  void update();
  bool render();
  void deInitialize();

  VkInstance getVkInstance() {
    return m_instanceObj.m_instance.instance;
  }
  VkDevice getVkDevice() {
    return m_deviceObj->m_device.device;
  }
  VkPhysicalDevice getVkPhysicalDevice() {
    return m_deviceObj->m_gpu.physical_device;
  }

  operator VkInstance() {
    return getVkInstance();
  }
  operator VkDevice() {
    return getVkDevice();
  }
  operator VkPhysicalDevice() {
    return getVkPhysicalDevice();
  }
};

} // namespace myvk_bs