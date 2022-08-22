#pragma warning(disable : 4819)
#pragma once
#ifdef _WIN32
#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>
#include <vk_mem_alloc.h>

#include <glm/common.hpp>
#include <glm/ext.hpp>
#include <glm/glm.hpp>


#include <stb_include.h>

#include <tiny_obj_loader.h>

#include <memory>
#include <vector>
