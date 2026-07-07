#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include "pokemon.h"
#include "pokemonDatabase.h"

int getValidInput(int min, int max) {
    int choice;

    while (true) {
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Invalid input. Please enter a number between "
                      << min << " and " << max << ": ";
            continue;
        }

        if (choice >= min && choice <= max) {
            return choice;
        }

        std::cout << "Invalid choice. Please enter a number between "
                  << min << " and " << max << ": ";
    }
}

Pokemon getPokemonByChoice(int choice) {
    if (choice == 1) return getPikachu();
    if (choice == 2) return getCharmander();
    if (choice == 3) return getBulbasaur();
    if (choice == 4) return getSquirtle();
    if (choice == 5) return getEevee();
    if (choice == 6) return getGengar();
    if (choice == 7) return getLucario();
    if (choice == 8) return getDragonite();

    return getPikachu();
}

void showPokemonMenu() {
    std::cout << "1. Pikachu\n";
    std::cout << "2. Charmander\n";
    std::cout << "3. Bulbasaur\n";
    std::cout << "4. Squirtle\n";
    std::cout << "5. Eevee\n";
    std::cout << "6. Gengar\n";
    std::cout << "7. Lucario\n";
    std::cout << "8. Dragonite\n";
}

void showMainMenu() {
    std::cout << "\n_________ Monster Battle Engine _________\n";
    std::cout << "1. Start Battle\n";
    std::cout << "2. Instructions\n";
    std::cout << "3. Exit\n";
    std::cout << "Choose an option: ";
}

void showInstructions() {
    std::cout << "\n_________ Instructions _________\n";
    std::cout << "1. Choose your Pokemon.\n";
    std::cout << "2. The enemy Pokemon is selected randomly.\n";
    std::cout << "3. Each Pokemon has multiple moves.\n";
    std::cout << "4. Moves can deal damage, miss, land critical hits, or apply status effects.\n";
    std::cout << "5. Type matchups affect damage.\n";
    std::cout << "6. Burn and Poison deal damage at the end of each round.\n";
    std::cout << "7. Sleep prevents attacking.\n";
    std::cout << "8. Paralysis has a chance to stop a Pokemon from attacking.\n";
    std::cout << "9. Battle continues until one Pokemon faints.\n";
}

void startBattle() {
    std::cout << "\nChoose your Pokemon:\n";
    showPokemonMenu();

    int playerChoice = getValidInput(1, 8);
    Pokemon playerPokemon = getPokemonByChoice(playerChoice);

    int randomChoice = rand() % 8 + 1;

    while (randomChoice == playerChoice) {
        randomChoice = rand() % 8 + 1;
    }

    Pokemon enemyPokemon = getPokemonByChoice(randomChoice);

    std::cout << "\n--- Battle Start ---\n";
    playerPokemon.displayStats();
    enemyPokemon.displayStats();

    bool running = true;
    int round = 1;
    bool playerWon = false;

    while (running) {
        std::cout << "\n========= Round " << round << " =========\n";

        // PLAYER TURN
        std::cout << "\nChoose " << playerPokemon.getName() << "'s move:\n";
        playerPokemon.showMoves();

        int playerMoveChoice = getValidInput(1, playerPokemon.getMoveCount());
        Move playerMove = playerPokemon.getMove(playerMoveChoice - 1);

        if (playerPokemon.canAttack()) {
            playerPokemon.attack(enemyPokemon, playerMove);
        }

        if (enemyPokemon.isFainted()) {
            playerWon = true;
            running = false;
        }

        // ENEMY TURN
        if (running) {
            std::cout << "\nEnemy's turn...\n";

            Move enemyMove = enemyPokemon.chooseBestMove(playerPokemon);

            if (enemyPokemon.canAttack()) {
                enemyPokemon.attack(playerPokemon, enemyMove);
            }

            if (playerPokemon.isFainted()) {
                playerWon = false;
                running = false;
            }
        }

        // END OF ROUND STATUS EFFECTS
        if (running) {
            std::cout << "\n--- End of Round Effects ---\n";

            playerPokemon.applyStatusEffect();
            enemyPokemon.applyStatusEffect();

            if (playerPokemon.isFainted()) {
                playerWon = false;
                running = false;
            }

            if (enemyPokemon.isFainted()) {
                playerWon = true;
                running = false;
            }
        }

        // STATS AFTER ROUND
        if (running) {
            std::cout << "\n--- Current Stats ---\n";
            playerPokemon.displayStats();
            enemyPokemon.displayStats();

            round++;
        }
    }

    std::cout << "\n_________ Battle Over _________\n";
    std::cout << "Rounds fought: " << round << "\n";
    std::cout << "Your Pokemon: " << playerPokemon.getName()
              << " | HP remaining: " << playerPokemon.getHp() << "\n";
    std::cout << "Enemy Pokemon: " << enemyPokemon.getName()
              << " | HP remaining: " << enemyPokemon.getHp() << "\n";

    if (playerWon) {
        std::cout << "\nYou won in " << round << " rounds!\n";
    } else {
        std::cout << "\nYou lost in " << round << " rounds!\n";
    }
}

int main() {
    srand(time(0));

    bool gameRunning = true;

    while (gameRunning) {
        showMainMenu();

        int menuChoice = getValidInput(1, 3);

        if (menuChoice == 1) {
            startBattle();
        }

        else if (menuChoice == 2) {
            showInstructions();
        }

        else if (menuChoice == 3) {
            std::cout << "\nExiting game...\n";
            gameRunning = false;
        }
    }

    return 0;
}