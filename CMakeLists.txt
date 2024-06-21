cmake_minimum_required(VERSION 3.22.1)

project(EOOLS 
    LANGUAGES CXX
    VERSION 0.0.0
    DESCRIPTION "terminal game."
)

set(TEST_EXE_NAME "test")
set(PROJECT_MAIN "${CMAKE_SOURCE_DIR}/src/main.cpp")

#target_compile_options(${PROJECT_NAME} PRIVATE -O3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Vulkan
# if using vcpkg manager copy the file path from "vcpkg integrate install" into VSS "cmake toolchain" (manage configurations under debug)
find_package(vulkan REQUIRED)
# must include vulkan includes manually
include_directories(D:/vcpackage/vcpkg/packages/vulkan-headers_x64-windows/include)

# Include FTXUI
include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Include ASIO
FetchContent_Declare(asio-cmake
  GIT_REPOSITORY https://github.com/kingsamchen/asio-cmake.git
  GIT_TAG        origin/master
)

set(ASIO_CMAKE_ASIO_TAG asio-1-28-0)
FetchContent_MakeAvailable(asio-cmake)


# Include Catch2
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)

# files
file(GLOB_RECURSE SRCS
    src/*.cpp
)
list(REMOVE_ITEM SRCS ${PROJECT_MAIN})

file(GLOB_RECURSE TESTS
    tests/*.cpp
)

message("srcs : ${SRCS}")
message("tests : ${TESTS}")

add_executable(${PROJECT_NAME} ${SRCS} ${PROJECT_MAIN})
add_executable(${TEST_EXE_NAME} ${SRCS} ${TESTS} )

set(LIBS 
    ftxui::screen
    ftxui::dom
    ftxui::component
    asio
    ${VULKAN_LIBRARIES}
    Catch2::Catch2
)

target_link_libraries(${PROJECT_NAME}   PRIVATE ${LIBS})
target_link_libraries(${TEST_EXE_NAME}  PRIVATE ${LIBS})
