add_library(glfw3 INTERFACE)
target_include_directories(glfw3 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include)
target_link_libraries(glfw3 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/glfw/lib-vc2022/glfw3.lib)

add_library(stbImage INTERFACE)
target_include_directories(stbImage INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stb)

add_library(tinyobjloader tinyobjloader/tiny_obj_loader.cc)
target_include_directories(tinyobjloader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tinyobjloader)

add_subdirectory(spdlog)

add_subdirectory(glm)

add_subdirectory(EasyVK)