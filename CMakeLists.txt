cmake_minimum_required(VERSION 3.22.1)

project(EOOLS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

file(GLOB SOURCES
    src/*.hpp
    src/*.h   # ... yeah
    src/NetCommon/*.hpp
    src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
# target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/NetCommon/)

target_link_libraries(${PROJECT_NAME} 
  PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component
    asio
)
