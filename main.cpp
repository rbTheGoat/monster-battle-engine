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

char getYesNoInput() {
    char choice;

    while (true) {
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y') return 'y';
        if (choice == 'n' || choice == 'N') return 'n';

        std::cout << "Invalid input. Please enter y or n: ";
    }
}

Pokemon getPokemonByChoice(int choice) {
    if(choice == 1) return getPikachu();
    if(choice == 2) return getCharmander();
    if(choice == 3) return getBulbasaur();
    if(choice == 4) return getSquirtle();
    if(choice == 5) return getEevee();
    if(choice == 6) return getGengar();
    if(choice == 7) return getLucario();
    if(choice == 8) return getDragonite();
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

int main() {

    srand(time(0));

    std::cout << "Choose your Pokemon:\n";
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

            if (enemyPokemon.isFainted()) {
                playerWon = true;
                running = false;
            }
        }

        // ENEMY TURN
        if (running) {
            std::cout << "\nEnemy's turn...\n";

            if (enemyPokemon.canAttack()) {
                Move enemyMove = enemyPokemon.chooseBestMove(playerPokemon);
                enemyPokemon.attack(playerPokemon, enemyMove);

                if (playerPokemon.isFainted()) {
                    playerWon = false;
                    running = false;
                }
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

        // STATS
        if (running) {
            std::cout << "\n--- Current Stats ---\n";
            playerPokemon.displayStats();
            enemyPokemon.displayStats();

            std::cout << "\nContinue? (y/n): ";
            char continueChoice = getYesNoInput();

            if (continueChoice == 'n') {
                running = false;
            }

            round++;
        }
    }

    std::cout << "\n========= Battle Over =========\n";
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

    return 0;
}