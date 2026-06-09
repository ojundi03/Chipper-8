cmake_minimum_required(VERSION 3.20)

#set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Target architectures for macOS")

project(Chipper-8 VERSION 0.1.0 LANGUAGES C CXX)

#find_package(fmt CONFIG REQUIRED)
#target_link_libraries(main PRIVATE fmt::fmt)

add_executable(Chipper-8 main.cpp)

find_package(imgui CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)

target_link_libraries(Chipper-8 PRIVATE
        imgui::imgui
        SDL3::SDL3
        "-framework OpenGL"
)