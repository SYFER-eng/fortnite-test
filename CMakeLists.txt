cmake_minimum_required(VERSION 3.10)
project(FortniteTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find DirectX
find_package(DirectX REQUIRED)

add_executable(FortniteTest 
    src/fortnite_test.cpp
    src/installer.cpp
)

target_link_libraries(FortniteTest
    d3d9
    d3dx9
    winmm
)

target_include_directories(FortniteTest PRIVATE
    ${DirectX_INCLUDE_DIRS}
)
