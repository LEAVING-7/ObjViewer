function(compile_shaders)
  find_program(GLSL_VALIDATOR glslangValidator HINTS $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)
  file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/shaders/*.frag"
    "${PROJECT_SOURCE_DIR}/shaders/*.vert"
    "${PROJECT_SOURCE_DIR}/shaders/*.comp"    
  )
  foreach(GLSL ${GLSL_SOURCE_FILES})
    message(STATUS "Building shader\n")
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/${FILE_NAME}.spv)
    add_custom_command(
      OUTPUT ${SPIRV}
      COMMAND ${GLSL_VALIDATOR} --target-env vulkan1.2 ${GLSL} -o ${SPIRV}
    )
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
  endforeach(GLSL)
  add_custom_target(
    Shaders ALL
    DEPENDS ${SPIRV_BINARY_FILES}
  )
endfunction(compile_shaders)
