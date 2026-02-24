add_executable(code_04_robotic_arm_transformations_esercizio main_04.cpp)

find_package(OpenGL)


target_include_directories(code_04_robotic_arm_transformations_esercizio PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}        # anchors relative includes here
)

set_property(TARGET code_04_robotic_arm_transformations_esercizio PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

target_link_libraries(code_04_robotic_arm_transformations_esercizio
    PRIVATE
        glfw
        glad
	glm
        OpenGL::GL
)
	
# Collect all files inside the folder
file(GLOB_RECURSE COMMON_FILES
    ${CMAKE_SOURCE_DIR}/src/common/*
)


# Add them to the target so the IDE shows them
target_sources(code_04_robotic_arm_transformations_esercizio PRIVATE ${COMMON_FILES})

# Organize them into a virtual folder in the IDE
source_group(TREE ${CMAKE_SOURCE_DIR}/src/common PREFIX "common" FILES ${COMMON_FILES})


# Collect all files inside the folder
file(GLOB_RECURSE COMMON_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*
)

# Add them to the target so the IDE shows them
target_sources(code_04_robotic_arm_transformations_esercizio PRIVATE ${COMMON_FILES})

# Organize them into a virtual folder in the IDE
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/shaders PREFIX "shaders" FILES ${COMMON_FILES})
