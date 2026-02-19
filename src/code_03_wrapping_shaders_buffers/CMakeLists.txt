add_executable(code_03_wrapping_shaders_buffers main_03.cpp)

find_package(OpenGL)


target_include_directories(code_03_wrapping_shaders_buffers PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}        # anchors relative includes here
)

set_property(TARGET code_03_wrapping_shaders_buffers PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

target_link_libraries(code_03_wrapping_shaders_buffers
    PRIVATE
        glfw
        glad
        OpenGL::GL
)

# Collect all files inside the folder
file(GLOB_RECURSE COMMON_FILES
    ${CMAKE_SOURCE_DIR}/src/common/*
)

# Add them to the target so the IDE shows them
target_sources(code_03_wrapping_shaders_buffers PRIVATE ${COMMON_FILES})

# Organize them into a virtual folder in the IDE
source_group(TREE ${CMAKE_SOURCE_DIR}/src/common PREFIX "common" FILES ${COMMON_FILES})


# Collect all files inside the folder
file(GLOB_RECURSE COMMON_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*
)

# Add them to the target so the IDE shows them
target_sources(code_03_wrapping_shaders_buffers PRIVATE ${COMMON_FILES})

# Organize them into a virtual folder in the IDE
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/shaders PREFIX "shaders" FILES ${COMMON_FILES})