# Battle Turn Simulator

A C++ console-based turn-based battle simulator inspired by classic monster battle systems. The project features type matchups, move accuracy, critical hits, status effects, difficulty-based enemy AI, battle summaries, input validation, and a persistent player profile system.

## Features

* Turn-based battle system
* Player Pokemon selection
* Random enemy Pokemon selection
* Multiple moves per Pokemon
* Move accuracy system
* Critical hit system
* Random damage variation
* Type effectiveness system
* Expanded move details display
* Status effects with turn durations:

  * Burn
  * Poison
  * Sleep
  * Paralysis
* Burn and Poison end-of-round damage
* Sleep-based turn skipping
* Paralysis-based chance to skip a turn
* Rule-based enemy AI
* Difficulty levels:

  * Easy: random enemy move selection
  * Normal: smart AI with controlled randomness
  * Hard: optimal AI move selection
* Main menu system
* Replay loop
* Instructions screen
* Battle summary after each match
* Player profile save/load system
* Change player name option
* Reset profile option
* Input validation for invalid numbers and text input
* Git/GitHub version control setup

## Tech Used

* C++
* Object-Oriented Programming
* STL Vector
* Structs
* Enums
* File Handling
* Random Number Generation
* Input Validation
* Git and GitHub

## Project Structure


pokemon.h
pokemon.cpp
pokemonDatabase.h
pokemonDatabase.cpp
saveSystem.h
saveSystem.cpp
main.cpp
README.md
.gitignore


## Core Systems

### Pokemon Class

The `Pokemon` class represents each battle character. Each Pokemon stores:

* Name
* HP
* Type
* Move list
* Current status condition
* Status duration

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

The simulator uses type matchups to modify damage. For example:

* Electric attacks are strong against Water.
* Fire attacks are strong against Grass.
* Normal attacks are weak against Ghost.
* Dragon attacks are strong against Dragon.

This makes move choice more strategic.

### Status Effects

The game supports multiple status effects:

* Burn deals damage at the end of each round.
* Poison deals stronger damage at the end of each round.
* Sleep prevents the Pokemon from attacking.
* Paralysis gives a chance for the Pokemon to lose its turn.

Each status effect lasts for a set number of turns.

### Enemy AI

The enemy uses rule-based decision-making. The AI evaluates moves using:

* Base damage
* Type effectiveness
* Accuracy
* Status effect usefulness
* Knockout potential
* Controlled randomness

The game also includes difficulty levels:

* Easy: enemy chooses random moves.
* Normal: enemy usually chooses strong moves but sometimes chooses a second-best option.
* Hard: enemy always chooses the best calculated move.

### Save System

The player profile is saved locally using file handling. The profile tracks:

* Player name
* Battles played
* Wins
* Losses
* Total damage dealt
* Total damage taken
* Total rounds played

The player can also reset the profile.

## How to Compile

Use this command:

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

## Example Gameplay Flow

```txt
========= Monster Battle Engine =========
1. Start Battle
2. View Player Profile
3. Change Player Name
4. Instructions
5. Reset Profile
6. Exit
```

During battle, the player chooses moves while the enemy uses AI to select its move. The battle continues until one Pokemon faints.

## What I Learned

This project helped me practice:

* Class design
* Encapsulation
* Structs and enums
* Vectors
* File I/O
* Game loop design
* Input validation
* Randomized game mechanics
* Rule-based AI
* Git/GitHub workflow
* Writing cleaner modular C++ code

## Future Improvements

* Add more Pokemon
* Add more moves
* Add more types
* Add items and healing
* Add team battles
* Add better battle animations in console
* Add unit tests
* Refactor internal names from Pokemon to a fully original monster system
* Create a graphical version using SFML

## Status

The console version is functionally complete. The next major planned upgrade is a graphical version using SFML.
