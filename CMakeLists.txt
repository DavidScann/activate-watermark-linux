cmake_minimum_required(VERSION 3.16)
project(activate-watermark-linux VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

find_package(LayerShellQt QUIET)

add_executable(activate-watermark
    src/main.cpp
    src/WatermarkWindow.cpp
    src/TrayManager.cpp
)

target_link_libraries(activate-watermark PRIVATE Qt6::Widgets)

if(LayerShellQt_FOUND)
    message(STATUS "LayerShellQt found - Wayland overlay support enabled")
    target_link_libraries(activate-watermark PRIVATE LayerShellQt::Interface)
    target_compile_definitions(activate-watermark PRIVATE HAVE_LAYERSHELL)
endif()

target_sources(activate-watermark PRIVATE
    src/WatermarkWindow.h
    src/TrayManager.h
)
