include(FetchContent)

FetchContent_Declare(
  shaderc
  GIT_REPOSITORY https://github.com/google/shaderc.git
  GIT_TAG        6d2848321250a19641800f8afb07738f3b0b7f18
)

#FetchContent_MakeAvailable(shaderc)

function(add_shaders TARGET_NAME)
  set(SHADER_SOURCE_FILES ${ARGN}) # the rest of arguments to this function will be assigned as shader source files
  
  # Validate that source files have been passed
  list(LENGTH SHADER_SOURCE_FILES FILE_COUNT)
  if(FILE_COUNT EQUAL 0)
    message(FATAL_ERROR "Can not create a shaders target without any source files")
  endif()

  set(SHADER_COMMAND)
  set(SHADER_PRODUCTS)

  foreach(SHADER_SOURCE IN LISTS SHADER_SOURCE_FILES)
    set(SHADER_RELATIVE_SOURCE ${SHADER_SOURCE})
    set(SHADER_RELATIVE_SOURCE ${SHADER_SOURCE})
    get_filename_component(SHADER_RELATIVE_PATH ${SHADER_RELATIVE_SOURCE} DIRECTORY)

    cmake_path(ABSOLUTE_PATH SHADER_SOURCE NORMALIZE)
    cmake_path(GET SHADER_SOURCE FILENAME SHADER_NAME)

    # Make directory
    file(MAKE_DIRECTORY ${SHADER_RELATIVE_PATH})
    
    # Build command
    list(APPEND SHADER_COMMAND COMMAND)
    list(APPEND SHADER_COMMAND "glslc")
    list(APPEND SHADER_COMMAND "${SHADER_SOURCE}")
    list(APPEND SHADER_COMMAND "-o")
    list(APPEND SHADER_COMMAND "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_RELATIVE_SOURCE}.spv")

    # Add product
    list(APPEND SHADER_PRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_RELATIVE_SOURCE}.spv")

  endforeach()

  add_custom_target(${TARGET_NAME} ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_RELATIVE_PATH}

    ${SHADER_COMMAND}
    COMMENT "Compiling Shaders [${TARGET_NAME}]"
    SOURCES ${SHADER_SOURCE_FILES}
    BYPRODUCTS ${SHADER_PRODUCTS}
  )
endfunction()