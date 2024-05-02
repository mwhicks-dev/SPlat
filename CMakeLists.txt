cmake_minimum_required(VERSION 3.26)
project(SPlat)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g")

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

set(LA src/KeyPressHandler.cpp src/CharacterCollisionHandler.cpp src/UpdateAssetHandler.cpp src/ClientController.cpp src/DeleteAssetHandler.cpp)
set(LB src/UnorderedMapObjectModel.cpp src/GetAssetHandler.cpp src/MovingPlatformCollisionHandler.cpp src/Entrypoint.cpp src/ClientCreateMovingPlatformHandler.cpp)
set(LC src/UnorderedStandingConfig.cpp src/OrderedPriorityListener.cpp src/PlatformCollisionHandler.cpp src/CharacterUpdateHandler.cpp src/LocalTimeline.cpp)
set(LD src/Environment.cpp src/KeyReleaseHandler.cpp src/Platform.cpp src/AssetFactoryConfig.cpp src/Character.cpp)
set(LE src/ClientCreatePlatformHandler.cpp src/MovingPlatformUpdateHandler.cpp src/FauxServerController.cpp src/TimingConfig.cpp src/ClientCreateCharacterHandler.cpp)
set(LF src/ClientConfig.cpp src/MovingPlatform.cpp src/AbstractAssetFactory.cpp)
set(LG src/CharacterFactory.cpp src/Client.cpp src/PlatformFactory.cpp src/MovingPlatformFactory.cpp)
set(ALL_IMPLEMENTATIONS ${LA} ${LB} ${LC} ${LD} ${LE} ${LF} ${LG})

add_executable(gom_test test/TestGameObjectModel.cpp ${ALL_IMPLEMENTATIONS})
target_link_libraries(gom_test gtest gtest_main sfml-graphics)

add_executable(serialization_test test/TestSerialization.cpp ${ALL_IMPLEMENTATIONS})
target_link_libraries(serialization_test gtest gtest_main sfml-graphics)

add_executable(asset_factory_test test/TestAssetFactory.cpp ${ALL_IMPLEMENTATIONS})
target_link_libraries(asset_factory_test gtest gtest_main sfml-system sfml-graphics sfml-window)

add_executable(demo test/Demo.cpp ${ALL_IMPLEMENTATIONS})
target_link_libraries(demo pthread zmq sfml-system sfml-graphics sfml-window)
