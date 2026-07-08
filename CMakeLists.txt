cmake_minimum_required(VERSION 3.16)

project(BattleTurnSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(BattleTurnSimulator
    main.cpp
    pokemon.cpp
    pokemonDatabase.cpp
    saveSystem.cpp
    battle.cpp
)