#include "myvk-bootstrap/Device.hpp"
#include "myvk-bootstrap/Application.hpp"

namespace myvk_bs {
void Device::create(std::vector<ccstr>& layers,
                    std::vector<ccstr>& extensions) {
  vkb::DeviceBuilder builder{m_gpu};

  auto result = builder.build();
  assert(result.has_value());
  m_device = result.value();
}
void Device::destroy() {
  vkb::destroy_device(m_device);
}
} // namespace myvk_bs