cmake_minimum_required(VERSION 3.14)

# Basic example
add_executable(BasicExample 
    BasicExample/Main.cpp
)

target_link_libraries(BasicExample 
    PRIVATE 
    Engine
)

# Copy resources folder to build directory if it exists
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../resources)
    add_custom_command(TARGET BasicExample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../resources
        $<TARGET_FILE_DIR:BasicExample>/resources
    )
endif()