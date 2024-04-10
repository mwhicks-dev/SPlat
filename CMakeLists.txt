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
    src/Asset.cpp src/Character.cpp src/Platform.cpp src/MovingPlatform.cpp src/GameObjectModel.cpp)
target_link_libraries(gom_test gtest gtest_main sfml-graphics)

add_executable(serialization_test test/TestSerialization.cpp)
target_link_libraries(serialization_test gtest gtest_main sfml-graphics)

add_executable(event_test test/TestEvents.cpp src/Event.cpp src/KeyEvents.cpp)
target_link_libraries(event_test gtest gtest_main sfml-graphics sfml-window)

add_executable(demo test/Demo.cpp
    src/Asset.cpp src/Character.cpp src/Platform.cpp src/MovingPlatform.cpp src/GameObjectModel.cpp 
    src/Event.cpp src/CreateAssetEvent.cpp
    src/Client.cpp)
target_link_libraries(demo sfml-system sfml-graphics sfml-window)
