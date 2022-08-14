function(compile_shaders)
  find_program(GLSL_VALIDATOR glslangValidator HINTS $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)
  file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/shaders/*.frag"
    "${PROJECT_SOURCE_DIR}/shaders/*.vert"
    "${PROJECT_SOURCE_DIR}/shaders/*.comp"    
  )
  message(STATUS ${GLSL_SOURCE_FILES})
  foreach(GLSL ${GLSL_SOURCE_FILES})
    message(STATUS "Building shader")
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/${FILE_NAME}.spv)
    add_custom_command(
      OUTPUT ${SPIRV}
      COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    )
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
  endforeach(GLSL)
  add_custom_target(
    Shaders ALL
    DEPENDS ${SPIRV_BINARY_FILES}
  )
endfunction(compile_shaders)
