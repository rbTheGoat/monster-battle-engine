#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include "pokemon.h"
#include "pokemonDatabase.h"
#include "saveSystem.h"

struct BattleStats {
    int playerDirectDamage = 0;
    int enemyDirectDamage = 0;

    int statusDamageToPlayer = 0;
    int statusDamageToEnemy = 0;

    int playerStatusesApplied = 0;
    int enemyStatusesApplied = 0;

    int playerTurnsSkipped = 0;
    int enemyTurnsSkipped = 0;
};

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

        if (choice == 'y' || choice == 'Y') {
            return 'y';
        }

        if (choice == 'n' || choice == 'N') {
            return 'n';
        }

        std::cout << "Invalid input. Please enter y or n: ";
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
    std::cout << "\n========= Monster Battle Engine =========\n";
    std::cout << "1. Start Battle\n";
    std::cout << "2. View Player Profile\n";
    std::cout << "3. Change Player Name\n";
    std::cout << "4. Instructions\n";
    std::cout << "5. Reset Profile\n";
    std::cout << "6. Exit\n";
    std::cout << "Choose an option: ";
}

void showDifficultyMenu() {
    std::cout << "\nChoose difficulty:\n";
    std::cout << "1. Easy   - Enemy chooses random moves\n";
    std::cout << "2. Normal - Enemy uses smart AI with some randomness\n";
    std::cout << "3. Hard   - Enemy always chooses the best move\n";
    std::cout << "Choose an option: ";
}

void showInstructions() {
    std::cout << "\n========= Instructions =========\n";
    std::cout << "1. Choose your Pokemon.\n";
    std::cout << "2. The enemy Pokemon is selected randomly.\n";
    std::cout << "3. Each Pokemon has multiple moves.\n";
    std::cout << "4. Moves can deal damage, miss, land critical hits, or apply status effects.\n";
    std::cout << "5. Type matchups affect damage.\n";
    std::cout << "6. Burn and Poison deal damage at the end of each round.\n";
    std::cout << "7. Sleep prevents attacking.\n";
    std::cout << "8. Paralysis has a chance to stop a Pokemon from attacking.\n";
    std::cout << "9. Your battle history is saved in your player profile.\n";
}

void showBattleSummary(
    Pokemon& playerPokemon,
    Pokemon& enemyPokemon,
    BattleStats stats,
    bool playerWon,
    int round
) {
    std::cout << "\n========= Battle Summary =========\n";

    if (playerWon) {
        std::cout << "Winner: " << playerPokemon.getName() << "\n";
    } else {
        std::cout << "Winner: " << enemyPokemon.getName() << "\n";
    }

    std::cout << "Rounds fought: " << round << "\n\n";

    std::cout << "Your Pokemon: " << playerPokemon.getName()
              << " | HP remaining: " << playerPokemon.getHp() << "\n";

    std::cout << "Enemy Pokemon: " << enemyPokemon.getName()
              << " | HP remaining: " << enemyPokemon.getHp() << "\n\n";

    std::cout << "--- Damage Stats ---\n";
    std::cout << "Player direct damage dealt: " << stats.playerDirectDamage << "\n";
    std::cout << "Enemy direct damage dealt: " << stats.enemyDirectDamage << "\n";
    std::cout << "Status damage taken by player: " << stats.statusDamageToPlayer << "\n";
    std::cout << "Status damage taken by enemy: " << stats.statusDamageToEnemy << "\n\n";

    std::cout << "--- Status Stats ---\n";
    std::cout << "Statuses applied by player: " << stats.playerStatusesApplied << "\n";
    std::cout << "Statuses applied by enemy: " << stats.enemyStatusesApplied << "\n";
    std::cout << "Player turns skipped by status: " << stats.playerTurnsSkipped << "\n";
    std::cout << "Enemy turns skipped by status: " << stats.enemyTurnsSkipped << "\n";

    if (playerWon) {
        std::cout << "\nYou won in " << round << " rounds!\n";
    } else {
        std::cout << "\nYou lost in " << round << " rounds!\n";
    }
}

void updateProfileAfterBattle(
    PlayerProfile& profile,
    BattleStats stats,
    bool playerWon,
    int round
) {
    profile.battlesPlayed++;

    if (playerWon) {
        profile.wins++;
    } else {
        profile.losses++;
    }

    profile.totalDamageDealt += stats.playerDirectDamage + stats.statusDamageToEnemy;
    profile.totalDamageTaken += stats.enemyDirectDamage + stats.statusDamageToPlayer;
    profile.totalRoundsPlayed += round;

    saveProfile(profile);
}

void startBattle(PlayerProfile& profile) {
    BattleStats stats;

    showDifficultyMenu();
    int difficulty = getValidInput(1, 3);

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

        bool playerCanMove = playerPokemon.canAttack();

        if (playerCanMove) {
            int enemyHpBefore = enemyPokemon.getHp();
            Status enemyStatusBefore = enemyPokemon.getStatus();

            playerPokemon.attack(enemyPokemon, playerMove);

            int damageDealt = enemyHpBefore - enemyPokemon.getHp();

            if (damageDealt > 0) {
                stats.playerDirectDamage += damageDealt;
            }

            if (enemyStatusBefore == Status::None && enemyPokemon.getStatus() != Status::None) {
                stats.playerStatusesApplied++;
            }
        } else {
            stats.playerTurnsSkipped++;
        }

        if (enemyPokemon.isFainted()) {
            playerWon = true;
            running = false;
        }

        // ENEMY TURN
        if (running) {
            std::cout << "\nEnemy's turn...\n";

            Move enemyMove = enemyPokemon.chooseMoveByDifficulty(playerPokemon, difficulty);

            bool enemyCanMove = enemyPokemon.canAttack();

            if (enemyCanMove) {
                int playerHpBefore = playerPokemon.getHp();
                Status playerStatusBefore = playerPokemon.getStatus();

                enemyPokemon.attack(playerPokemon, enemyMove);

                int damageDealt = playerHpBefore - playerPokemon.getHp();

                if (damageDealt > 0) {
                    stats.enemyDirectDamage += damageDealt;
                }

                if (playerStatusBefore == Status::None && playerPokemon.getStatus() != Status::None) {
                    stats.enemyStatusesApplied++;
                }
            } else {
                stats.enemyTurnsSkipped++;
            }

            if (playerPokemon.isFainted()) {
                playerWon = false;
                running = false;
            }
        }

        // END OF ROUND STATUS EFFECTS
        if (running) {
            std::cout << "\n--- End of Round Effects ---\n";

            int playerHpBeforeStatus = playerPokemon.getHp();
            int enemyHpBeforeStatus = enemyPokemon.getHp();

            playerPokemon.applyStatusEffect();
            enemyPokemon.applyStatusEffect();

            int playerStatusDamage = playerHpBeforeStatus - playerPokemon.getHp();
            int enemyStatusDamage = enemyHpBeforeStatus - enemyPokemon.getHp();

            if (playerStatusDamage > 0) {
                stats.statusDamageToPlayer += playerStatusDamage;
            }

            if (enemyStatusDamage > 0) {
                stats.statusDamageToEnemy += enemyStatusDamage;
            }

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

    showBattleSummary(playerPokemon, enemyPokemon, stats, playerWon, round);
    updateProfileAfterBattle(profile, stats, playerWon, round);

    std::cout << "\nProfile saved.\n";
}

int main() {
    srand(time(0));

    PlayerProfile profile = loadProfile();

    bool gameRunning = true;

    while (gameRunning) {
        showMainMenu();

        int menuChoice = getValidInput(1, 6);

        if (menuChoice == 1) {
            startBattle(profile);
        }

        else if (menuChoice == 2) {
            displayProfile(profile);
        }

        else if (menuChoice == 3) {
            changePlayerName(profile);
        }

        else if (menuChoice == 4) {
            showInstructions();
        }

        else if (menuChoice == 5) {
            std::cout << "\nAre you sure you want to reset your profile? (y/n): ";
            char confirmReset = getYesNoInput();

            if (confirmReset == 'y') {
                resetProfile();
                profile = createDefaultProfile();
                std::cout << "Profile reset successfully.\n";
            } else {
                std::cout << "Profile reset cancelled.\n";
            }
        }

        else if (menuChoice == 6) {
            std::cout << "\nExiting game...\n";
            gameRunning = false;
        }
    }

    return 0;
}