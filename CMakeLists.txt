cmake_minimum_required(VERSION 3.26)
project(SPlat)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -DDEBUG")

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

set(LA src/CharacterCollisionHandler.cpp src/TickEvent.cpp src/Config.cpp src/Listener.cpp src/Controller.cpp src/MovingPlatformCollisionHandler.cpp src/AssetEvents.cpp src/PlatformEvents.cpp src/PlatformCollisionHandler.cpp src/CharacterUpdateHandler.cpp src/LocalTimeline.cpp src/Environment.cpp src/Platform.cpp src/AssetFactoryConfig.cpp src/Character.cpp src/CharacterEvents.cpp src/MovingPlatformUpdateHandler.cpp src/KeyEvents.cpp src/TimingConfig.cpp src/MovingPlatformEvents.cpp src/MovingPlatform.cpp src/AbstractAssetFactory.cpp)
set(LB src/GameObjectModel.cpp src/CharacterFactory.cpp src/PlatformFactory.cpp src/MovingPlatformFactory.cpp)
set(LC src/Client.cpp)
set(ALL_IMPLEMENTATIONS ${LA} ${LB} ${LC})

add_executable(gom_test test/TestGameObjectModel.cpp ${ALL_IMPLEMENTATIONS})
target_link_libraries(gom_test gtest gtest_main sfml-graphics)

add_executable(serialization_test test/TestSerialization.cpp ${ALL_IMPLEMENTATIONS})
target_link_libraries(serialization_test gtest gtest_main sfml-graphics)

add_executable(asset_factory_test test/TestAssetFactory.cpp ${ALL_IMPLEMENTATIONS})
target_link_libraries(asset_factory_test gtest gtest_main sfml-system sfml-graphics sfml-window)

add_executable(demo test/Demo.cpp ${ALL_IMPLEMENTATIONS})
target_link_libraries(demo pthread sfml-system sfml-graphics sfml-window)
