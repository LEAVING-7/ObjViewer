#pragma once
#include "common.hpp"
#include "pch.hpp"

namespace myvk::bs {
class Instance {
public:
  vkb::Instance m_instance;

private:
  VkDebugUtilsMessengerEXT             m_messenger;
  PFN_vkDebugUtilsMessengerCallbackEXT m_fpDebugMessenger{nullptr};
  bool                                 m_enableValidationLayer;

public:
  void setDebugMessenger(PFN_vkDebugUtilsMessengerCallbackEXT callback) {
    m_fpDebugMessenger = callback;
  };

  void create(std::vector<ccstr>& layers, std::vector<ccstr>& extensions,
              ccstr appName, bool enableValidationLayer = true);
  void destroy();

  MYVK_CONVERT_OP(Instance, m_instance);
};
} // namespace myvk::bs