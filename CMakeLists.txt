cmake_minimum_required(VERSION 3.0.0)
project(ObjViewer VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

find_package(Vulkan REQUIRED)
add_subdirectory(external)

include(CompileShaders.cmake)
compile_shaders()

add_executable(ObjViewer)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src TARGET_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/bootstrap MYVK_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/GUI GUI_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/DataType DATA_TYPE_SRC)


target_sources(ObjViewer PUBLIC ${MYVK_SRC} ${TARGET_SRC} ${GUI_SRC} ${DATA_TYPE_SRC})
target_include_directories(ObjViewer PUBLIC include ${Vulkan_INCLUDE_DIR})
target_link_libraries(ObjViewer PUBLIC ${Vulkan_LIBRARY} 
                        spdlog::spdlog glfw3 
                        VulkanMemoryAllocator glm)
                        
target_precompile_headers(ObjViewer PRIVATE include/pch.hpp)


