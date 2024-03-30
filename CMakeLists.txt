cmake_minimum_required(VERSION 3.26)
project(SPlat)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(gom_test test/TestGameObjectModel.cpp
    src/Asset.cpp src/Character.cpp src/GameObjectModel.cpp)
target_link_libraries(gom_test gtest gtest_main sfml-graphics)
