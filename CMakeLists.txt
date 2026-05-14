cmake_minimum_required(VERSION 3.16)
project(activate-watermark-linux VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(LayerShellQt QUIET)

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(WAYLAND_CLIENT wayland-client)
endif()

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

if(WAYLAND_CLIENT_FOUND)
    message(STATUS "wayland-client found - Wayland input region clearing enabled")
    target_compile_definitions(activate-watermark PRIVATE HAVE_WAYLAND_CLIENT)
    target_include_directories(activate-watermark PRIVATE ${WAYLAND_CLIENT_INCLUDE_DIRS})
    target_link_libraries(activate-watermark PRIVATE ${WAYLAND_CLIENT_LIBRARIES})
    set(QT_NO_PRIVATE_MODULE_WARNING ON)
    find_package(Qt6 QUIET COMPONENTS GuiPrivate)
    if(Qt6GuiPrivate_FOUND)
        target_link_libraries(activate-watermark PRIVATE Qt6::GuiPrivate)
    endif()
endif()

target_sources(activate-watermark PRIVATE
    src/WatermarkWindow.h
    src/TrayManager.h
)
