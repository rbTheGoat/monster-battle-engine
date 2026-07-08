# Battle Turn Simulator

A C++ console-based turn-based battle simulator featuring type matchups, move accuracy, critical hits, status effects, enemy AI, battle summaries, and a persistent player profile system.

## Features

* Turn-based battle system
* Player Pokemon selection
* Random enemy selection
* Multiple moves per Pokemon
* Move accuracy system
* Critical hit system
* Random damage variation
* Type effectiveness system
* Status effects with turn durations

  * Burn
  * Poison
  * Sleep
  * Paralysis
* Status-based turn skipping
* End-of-round status damage
* Rule-based enemy AI
* Battle summary after each match
* Player profile save/load system using file handling
* Main menu and replay loop
* Input validation for invalid user input

## Tech Used

* C++
* Object-Oriented Programming
* STL Vector
* Structs
* Enums
* File Handling
* Random Number Generation
* Git and GitHub

## Main Concepts Implemented

### Object-Oriented Design

The project uses a `Pokemon` class to represent each battle character. Each object stores its own name, HP, type, moves, and current status condition.

### Move System

Moves are represented using a `Move` struct. Each move contains:

* Name
* Damage
* Accuracy
* Type
* Status effect
* Status chance
* Status duration

### Type Effectiveness

Damage changes depending on type matchups. For example, Electric-type attacks deal more damage to Water-type opponents.

### Status Effects

The game supports multiple status effects:

* Burn causes damage at the end of each round.
* Poison causes stronger end-of-round damage.
* Sleep prevents attacking.
* Paralysis gives a chance to skip a turn.

### Enemy AI

The enemy uses rule-based decision-making instead of choosing moves randomly. The AI evaluates moves based on:

* Damage
* Type effectiveness
* Accuracy
* Status effect usefulness
* Knockout potential
* Controlled randomness to avoid being too predictable

### Save System

The player profile is saved in a local file and tracks:

* Battles played
* Wins
* Losses
* Total damage dealt
* Total damage taken
* Total rounds played

## How to Compile

Use the following command:

```bash
g++ main.cpp pokemon.cpp pokemonDatabase.cpp saveSystem.cpp -o game
```

## How to Run

On Windows:

```bash
.\game.exe
```

On Linux/macOS:

```bash
./game
```

## Project Structure

```txt
pokemon.h
pokemon.cpp
pokemonDatabase.h
pokemonDatabase.cpp
saveSystem.h
saveSystem.cpp
main.cpp
README.md
.gitignore
```

## Future Improvements

* Add more Pokemon
* Add more moves
* Add more types
* Improve enemy AI further
* Add difficulty levels
* Add item system
* Add team battles
* Add graphical version using SFML
* Add sound effects and animations in the graphical version

## Status

Console version is mostly complete. The next major upgrade planned is a graphical version using SFML.
