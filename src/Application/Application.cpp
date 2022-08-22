#include "Application/Application.hpp"

extern std::vector<ccstr> g_instanceExtensionNames;
extern std::vector<ccstr> g_layerNames;
extern std::vector<ccstr> g_deviceExtensionNames;
namespace myvk {

Application::Application() {}

Application::~Application() {}

Application* Application::GetInstance() {
  std::call_once(sm_onlyOnce, [] { sm_instance.reset(new Application); });
  return sm_instance.get();
}

void Application::initialize() {
  glfwInit();
  ccstr title = "Halo";
  m_instanceObj.setDebugMessenger(
      [](VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
         VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
         const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
         void*                                       pUserData) -> VkBool32 {
        // auto servertiy = vkb::to_string_message_severity(messageSeverity);

        if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
          LOG_ERR("\n[Vulkan]: {}", pCallbackData->pMessage);
        } else if (messageSeverity &
                   VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
          LOG_WARN("\n[Vulkan]: {}", pCallbackData->pMessage);
        } else if (messageSeverity &
                   VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
          LOG_INFO("\n[Vulkan]: {}", pCallbackData->pMessage);
        } else if (messageSeverity &
                   VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
          // skip verbose
        }
        fflush(stdout);
        return VK_FALSE;
      });

  assert(glfwVulkanSupported());

  u32    glfwRequiredExtensionCount;
  ccstr* glfwRequiredExtension =
      glfwGetRequiredInstanceExtensions(&glfwRequiredExtensionCount);
  for (u32 i = 0; i < glfwRequiredExtensionCount; ++i) {
    g_instanceExtensionNames.push_back(glfwRequiredExtension[i]);
  }

  m_instanceObj.create(g_layerNames, g_instanceExtensionNames, title);

  m_rendererObj = std::make_unique<Renderer>();
  m_rendererObj->createWindow(m_instanceObj);

  vkb::PhysicalDeviceSelector selector{m_instanceObj.m_instance,
                                       m_rendererObj->m_surface};

  auto selection = selector.set_minimum_version(1, 1)
                       .add_desired_extensions(g_deviceExtensionNames)
                       .select();

  assert(selection.has_value());
  vkb::PhysicalDevice physicalDevice = selection.value();

  m_deviceObj = std::make_unique< ezvk::Device>();
  m_deviceObj->create(physicalDevice, g_layerNames, g_deviceExtensionNames);

  m_allocator.create(getVkPhysicalDevice(), getVkDevice(), getVkInstance());
  m_rendererObj->create(this);
}

void Application::deInitialize() {
  m_rendererObj->destroy();
  m_rendererObj->destroyWindow(m_instanceObj);
  m_allocator.destroy();
  m_deviceObj->destroy();
  m_instanceObj.destroy();
}

void Application::prepare() {
  m_isPrepared = false;
  m_rendererObj->prepare();
  m_isPrepared = true;
}
void Application::update() {}

bool Application::render() {
  m_rendererObj->render();
  return m_rendererObj->windowShouldClose();
}

} // namespace myvk
