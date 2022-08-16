#include "bootstrap/Device.hpp"

namespace myvk::bs {
void Device::create(vkb::PhysicalDevice& gpu, std::vector<ccstr>& layers,
                    std::vector<ccstr>& extensions) {
  vkb::DeviceBuilder builder{gpu};

  auto result = builder.build();
  assert(result.has_value());
  m_device = std::move(result.value());
  m_gpu    = std::move(gpu);
}
void Device::destroy() {
  vkb::destroy_device(m_device);
}
} // namespace myvk::bs