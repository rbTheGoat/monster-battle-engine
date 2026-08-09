cmake_minimum_required(VERSION 3.16)

project(BattleTurnSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_MakeAvailable(SFML)

# ==================================================
# Console version
# ==================================================

add_executable(BattleTurnSimulatorConsole
    main.cpp
    pokemon.cpp
    pokemonDatabase.cpp
    saveSystem.cpp
    battle.cpp
)

# ==================================================
# SFML version
# ==================================================

add_executable(BattleTurnSimulatorSFML
    sfmlMain.cpp

    pokemon.cpp
    pokemonDatabase.cpp
    battle.cpp
    saveSystem.cpp

    sfmlHelpers.cpp
    battleLog.cpp
    creatureVisuals.cpp
    moveAnimations.cpp
    profileScreen.cpp
    audioManager.cpp
    uiTheme.cpp
)

target_link_libraries(
    BattleTurnSimulatorSFML
    PRIVATE

    SFML::Graphics
    SFML::Audio
)

# ==================================================
# Copy assets beside executable
# ==================================================

add_custom_command(
    TARGET BattleTurnSimulatorSFML
    POST_BUILD

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:BattleTurnSimulatorSFML>/assets"

    COMMENT "Copying game assets"
)