project(NorthEngine)

file(GLOB COMMON_SOURCES Common/*.hpp Common/*.cpp)
file(GLOB ENGINE_SOURCES Engine/*.hpp Engine/*.cpp Engine/Components/*.hpp Engine/Components/*.cpp)
file(GLOB INPUT_SOURCES Input/*.hpp Input/*.cpp)
file(GLOB MATH_SOURCES Math/*.hpp Math/*.cpp)
file(GLOB PLATFORM_SOURCES Platform/*.hpp Platform/*.cpp)
file(GLOB GRAPHICS_SOURCES Graphics/*.hpp Graphics/*.cpp)

add_library(north STATIC
        ${COMMON_SOURCES}
        ${ENGINE_SOURCES}
        ${INPUT_SOURCES}
        ${MATH_SOURCES}
        ${PLATFORM_SOURCES}
        ${GRAPHICS_SOURCES}
)

# Sets defines for platform and windowing system
DetectPlatform(north)

target_link_libraries(north PUBLIC
        Vulkan::Vulkan
        glfw
        vk-bootstrap::vk-bootstrap
        VulkanMemoryAllocator
        glm::glm
        pugixml::static
)

target_include_directories(north PRIVATE
        ${CMAKE_SOURCE_DIR}/Code/Modules
        ${CMAKE_SOURCE_DIR}/Code/Tools
        ${CMAKE_SOURCE_DIR}/Code/Vendor
)