cmake_minimum_required(VERSION 3.11)
project(VulkanGuide)


find_package(Vulkan REQUIRED)

add_subdirectory(Third-party/glfw)
add_subdirectory(Third-party/glm)
add_library(EngineLibrary STATIC Engine/HelloTriangle.hpp Engine/HelloTriangle.cpp)

target_link_libraries(EngineLibrary glm glfw Vulkan::Vulkan)

target_include_directories(EngineLibrary PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Engine ${Vulkan_INCLUDE_DIRS})
target_compile_features(EngineLibrary PRIVATE cxx_std_17)
target_compile_definitions(EngineLibrary PUBLIC GLFW_INCLUDE_NONE)

add_executable(${PROJECT_NAME} Sources/main.cpp)

#target_compile_definitions(${PROJECT_NAME} PUBLIC GLFW_INCLUDE_NONE)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Sources)
target_link_libraries(${PROJECT_NAME} EngineLibrary)
