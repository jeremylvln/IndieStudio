cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/mods)
set(POSITION_INDEPENDENT_CODE ON)

project(bomberman VERSION 1.0.0 LANGUAGES CXX)

# Irrlicht support either for Linux and Windows
set(Irrlicht_DIR "vendor/Irrlicht")
set(Irrlicht_FIND_REQUIRED TRUE)
include(FindIrrlicht)

find_package(Irrlicht REQUIRED)
include_directories(SYSTEM ${Irrlicht_INCLUDE_DIRS})

find_package(SFML 2 REQUIRED audio)
include_directories(SYSTEM ${SFML_ROOT}/include)

option(BUILD_GAME "Enable build for the game" ON)
option(BUILD_MODS "Enable build for the mods" ON)
option(BUILD_TESTS "Enable tests" OFF)

set(INDIE_API_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/api)
set(INDIE_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include)
set(INDIE_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INDIE_SRC
    ${INDIE_SRC_DIR}/Bootstraper.cpp
    ${INDIE_SRC_DIR}/Game.cpp
    ${INDIE_SRC_DIR}/Constants.cpp
    ${INDIE_SRC_DIR}/common/Configuration.cpp
    ${INDIE_SRC_DIR}/common/Logger.cpp
    ${INDIE_SRC_DIR}/common/SoundManager.cpp
    ${INDIE_SRC_DIR}/common/FontManager.cpp
    ${INDIE_SRC_DIR}/common/TextureManager.cpp
    ${INDIE_SRC_DIR}/common/MeshManager.cpp
    ${INDIE_SRC_DIR}/common/Scheduler.cpp
    ${INDIE_SRC_DIR}/ecs/Signature.cpp
    ${INDIE_SRC_DIR}/ecs/systems/ApplyExplosionSystem.cpp
    ${INDIE_SRC_DIR}/ecs/systems/EventSystem.cpp
    ${INDIE_SRC_DIR}/ecs/systems/ExplosionDurationSystem.cpp
    ${INDIE_SRC_DIR}/ecs/systems/MovePlayerSystem.cpp
    ${INDIE_SRC_DIR}/ecs/systems/PickUpPowerUpSystem.cpp
    ${INDIE_SRC_DIR}/ecs/systems/SetupExplosionSystem.cpp
    ${INDIE_SRC_DIR}/ecs/systems/SolidBombSystem.cpp
    ${INDIE_SRC_DIR}/ecs/systems/WinLooseSystem.cpp
    ${INDIE_SRC_DIR}/mod/LibraryWrapper.cpp
    ${INDIE_SRC_DIR}/mod/ModManager.cpp
    ${INDIE_SRC_DIR}/player/AIControlProvider.cpp
    ${INDIE_SRC_DIR}/player/ControlProviderManager.cpp
    ${INDIE_SRC_DIR}/player/KeyboardControlProvider.cpp
    ${INDIE_SRC_DIR}/scene/MainMenuScene.cpp
    ${INDIE_SRC_DIR}/scene/NewGameScene.cpp
    ${INDIE_SRC_DIR}/scene/PlayScene.cpp
    ${INDIE_SRC_DIR}/scene/SceneManager.cpp
    ${INDIE_SRC_DIR}/statistic/AchievementManager.cpp
    ${INDIE_SRC_DIR}/statistic/StatisticManager.cpp
    ${INDIE_SRC_DIR}/world/BasicWorldGenerator.cpp
    ${INDIE_SRC_DIR}/world/EmptyWorldGenerator.cpp
    ${INDIE_SRC_DIR}/world/MapPattern.cpp
    ${INDIE_SRC_DIR}/world/VerticalWorldGenerator.cpp
    ${INDIE_SRC_DIR}/world/World.cpp
    ${INDIE_SRC_DIR}/world/WorldManager.cpp
    ${INDIE_SRC_DIR}/world/WorldSettings.cpp
    ${INDIE_SRC_DIR}/utils/CRCUtils.cpp
)
set(INDIE_REQUIRED_LIBRARIES ${Irrlicht_LIBRARIES} sfml-system sfml-audio)

if (UNIX)
    set(INDIE_REQUIRED_LIBRARIES ${INDIE_REQUIRED_LIBRARIES} dl stdc++fs pthread)
endif ()

if (UNIX)
    add_compile_options(-Wall -Wextra)
endif ()

if (CMAKE_BUILD_TYPE EQUAL "Debug")
    add_compile_options(-g3)
endif ()

if (BUILD_GAME)
    add_executable(bomberman ${INDIE_SRC_DIR}/Main.cpp ${INDIE_SRC})

    target_include_directories(bomberman PRIVATE ${INDIE_API_INCLUDE_DIRS})
    target_include_directories(bomberman PRIVATE ${INDIE_INCLUDE_DIRS})
    target_link_libraries(bomberman ${INDIE_REQUIRED_LIBRARIES})
endif ()

if (BUILD_MODS)
    add_subdirectory(mods)
endif ()

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()
