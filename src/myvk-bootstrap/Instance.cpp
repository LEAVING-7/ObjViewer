#include "myvk-bootstrap/Instance.hpp"

namespace myvk_bs {
void Instance::create(std::vector<ccstr> &layers,
                      std::vector<ccstr> &extensions, ccstr appName,
                      bool enableValidationLayer) {
  m_enableValidationLayer = enableValidationLayer;
  vkb::InstanceBuilder builder{};
  builder.set_app_name(appName)
      .set_engine_name(appName)
      .set_app_version(1)
      .set_engine_version(1)
      .require_api_version(1, 1);
  std::for_each(layers.cbegin(), layers.cend(),
                [&](ccstr name) { builder.enable_layer(name); });
  std::for_each(extensions.cbegin(), extensions.cend(),
                [&](ccstr name) { builder.enable_extension(name); });

  if (enableValidationLayer) {
    builder.enable_validation_layers();
    if (!m_fpDebugMessenger)
      builder.use_default_debug_messenger();
    else
      builder.set_debug_callback(m_fpDebugMessenger);
  }

  auto result = builder.build();
  assert(result.has_value());
  m_instance = result.value();
}
void Instance::destroy() { vkb::destroy_instance(m_instance); }
} // namespace myvk_bs