project(NorthEngine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Sandbox)

add_executable(sandbox main.cpp)
target_link_libraries(sandbox PRIVATE
        north
)
target_include_directories(sandbox PRIVATE ${CMAKE_SOURCE_DIR}/Code/Modules)