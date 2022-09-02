#pragma once

#include "EasyVK/Allocator.hpp"
#include "EasyVK/Device.hpp"
#include "EasyVK/Instance.hpp"

#include "Application/Renderer.hpp"

namespace myvk {

class Application {
private:
  static inline std::unique_ptr<Application> sm_instance;
  static inline std::once_flag               sm_onlyOnce;

public:
  ezvk::Instance  m_instanceObj;
  ezvk::Device    m_deviceObj;
  ezvk::Allocator m_allocator;
  Renderer        m_rendererObj;

private:
  bool m_isPrepared;
  bool m_isResizing;

public:
  static Application* GetInstance();

private:
  Application(){};

public:
  ~Application(){};

public:
  void initialize();
  void prepare();
  void update();
  bool render();
  void deInitialize();

  VkInstance getVkInstance() {
    return m_instanceObj;
  }
  VkDevice getVkDevice() {
    return m_deviceObj.m_device.device;
  }
  VkPhysicalDevice getVkPhysicalDevice() {
    return m_deviceObj.m_gpu.physical_device;
  }

  EZVK_CONVERT_OP(VkInstance, getVkInstance());
  EZVK_CONVERT_OP(VkDevice, getVkDevice());
  EZVK_CONVERT_OP(VkPhysicalDevice, getVkPhysicalDevice());
  EZVK_CONVERT_OP(ezvk::Allocator&, m_allocator);
};

} // namespace myvk