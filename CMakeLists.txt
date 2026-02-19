
cmake_minimum_required(VERSION 3.16)

project(CGCourse LANGUAGES CXX C)  # Include C since GLAD is C
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Optional (tidy): ensure C standard for glad
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# -----------------------------
# External libraries
# -----------------------------

# GLFW (as subdirectory)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
# If you prefer static GLFW:
set(GLFW_BUILD_SHARED_LIBRARY OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)

# GLAD (C library)
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)
# On Linux you may need dl:
if(UNIX AND NOT APPLE)
  target_link_libraries(glad PUBLIC dl)
endif()

# GLM (header-only)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE external)


# Make the include path for your sources available project-wide (optional)
# include_directories(${CMAKE_SOURCE_DIR}/src)

# Let src/ define the actual executable(s)
add_subdirectory(src)
