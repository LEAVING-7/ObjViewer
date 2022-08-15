#pragma once
#include "common.hpp"
#include "pch.hpp"

namespace myvk::bs {
class Device {
public:
  vkb::Device         m_device;
  vkb::PhysicalDevice m_gpu;

  void create(vkb::PhysicalDevice& gpu, std::vector<ccstr>& layers,
              std::vector<ccstr>& extensions);
  void destroy();

  MYVK_CONVERT_OP(Device, m_device);
};

} // namespace myvk::bs