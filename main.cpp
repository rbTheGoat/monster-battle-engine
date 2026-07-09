#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

#include "pokemon.h"
#include "pokemonDatabase.h"
#include "saveSystem.h"
#include "battle.h"

// --------------------------------------------------
// Input validation
// --------------------------------------------------

int getValidInput(int min, int max)
{
    int choice;

    while (true)
    {
        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout
                << "Invalid input. Please enter a number between "
                << min
                << " and "
                << max
                << ": ";

            continue;
        }

        if (choice >= min && choice <= max)
        {
            return choice;
        }

        std::cout
            << "Invalid choice. Please enter a number between "
            << min
            << " and "
            << max
            << ": ";
    }
}

char getYesNoInput()
{
    char choice;

    while (true)
    {
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y')
        {
            return 'y';
        }

        if (choice == 'n' || choice == 'N')
        {
            return 'n';
        }

        std::cout << "Invalid input. Please enter y or n: ";
    }
}

// --------------------------------------------------
// Creature selection
// --------------------------------------------------

Pokemon getCreatureByChoice(int choice)
{
    if (choice == 1)
    {
        return getVoltbit();
    }

    if (choice == 2)
    {
        return getEmberling();
    }

    if (choice == 3)
    {
        return getMossback();
    }

    if (choice == 4)
    {
        return getAquafin();
    }

    if (choice == 5)
    {
        return getWildkin();
    }

    if (choice == 6)
    {
        return getShadegeist();
    }

    if (choice == 7)
    {
        return getIronfang();
    }

    if (choice == 8)
    {
        return getSkywyrm();
    }

    return getVoltbit();
}

void showCreatureMenu()
{
    std::cout << "1. Voltbit\n";
    std::cout << "2. Emberling\n";
    std::cout << "3. Mossback\n";
    std::cout << "4. Aquafin\n";
    std::cout << "5. Wildkin\n";
    std::cout << "6. Shadegeist\n";
    std::cout << "7. Ironfang\n";
    std::cout << "8. Skywyrm\n";
}

// --------------------------------------------------
// Menu displays
// --------------------------------------------------

void showDifficultyMenu()
{
    std::cout << "\nChoose difficulty:\n";
    std::cout << "1. Easy   - Enemy chooses random moves\n";
    std::cout << "2. Normal - Enemy uses smart AI with some randomness\n";
    std::cout << "3. Hard   - Enemy always chooses the best move\n";
    std::cout << "Choose an option: ";
}

void showMainMenu()
{
    std::cout << "\n========= Monster Battle Engine =========\n";
    std::cout << "1. Start Battle\n";
    std::cout << "2. View Player Profile\n";
    std::cout << "3. Change Player Name\n";
    std::cout << "4. Instructions\n";
    std::cout << "5. Reset Profile\n";
    std::cout << "6. Exit\n";
    std::cout << "Choose an option: ";
}

void showInstructions()
{
    std::cout << "\n========= Instructions =========\n";
    std::cout << "1. Choose your creature.\n";
    std::cout << "2. The enemy creature is selected randomly.\n";
    std::cout << "3. Each creature has multiple moves.\n";
    std::cout << "4. Moves can deal damage, miss, land critical hits, or apply status effects.\n";
    std::cout << "5. Type matchups affect damage.\n";
    std::cout << "6. Burn and Poison deal damage at the end of each round.\n";
    std::cout << "7. Sleep prevents a creature from attacking.\n";
    std::cout << "8. Paralysis has a chance to stop a creature from attacking.\n";
    std::cout << "9. Your battle history is saved in your player profile.\n";
}

// --------------------------------------------------
// Battle summary and profile update
// --------------------------------------------------

void showBattleSummary(
    Pokemon& playerCreature,
    Pokemon& enemyCreature,
    BattleStats stats,
    bool playerWon,
    int round
)
{
    std::cout << "\n========= Battle Summary =========\n";

    if (playerWon)
    {
        std::cout
            << "Winner: "
            << playerCreature.getName()
            << "\n";
    }
    else
    {
        std::cout
            << "Winner: "
            << enemyCreature.getName()
            << "\n";
    }

    std::cout
        << "Rounds fought: "
        << round
        << "\n\n";

    std::cout
        << "Your creature: "
        << playerCreature.getName()
        << " | HP remaining: "
        << playerCreature.getHp()
        << "\n";

    std::cout
        << "Enemy creature: "
        << enemyCreature.getName()
        << " | HP remaining: "
        << enemyCreature.getHp()
        << "\n\n";

    std::cout << "--- Damage Stats ---\n";

    std::cout
        << "Player direct damage dealt: "
        << stats.playerDirectDamage
        << "\n";

    std::cout
        << "Enemy direct damage dealt: "
        << stats.enemyDirectDamage
        << "\n";

    std::cout
        << "Status damage taken by player: "
        << stats.statusDamageToPlayer
        << "\n";

    std::cout
        << "Status damage taken by enemy: "
        << stats.statusDamageToEnemy
        << "\n\n";

    std::cout << "--- Status Stats ---\n";

    std::cout
        << "Statuses applied by player: "
        << stats.playerStatusesApplied
        << "\n";

    std::cout
        << "Statuses applied by enemy: "
        << stats.enemyStatusesApplied
        << "\n";

    std::cout
        << "Player turns skipped by status: "
        << stats.playerTurnsSkipped
        << "\n";

    std::cout
        << "Enemy turns skipped by status: "
        << stats.enemyTurnsSkipped
        << "\n";

    if (playerWon)
    {
        std::cout
            << "\nYou won in "
            << round
            << " rounds!\n";
    }
    else
    {
        std::cout
            << "\nYou lost in "
            << round
            << " rounds!\n";
    }
}

void updateProfileAfterBattle(
    PlayerProfile& profile,
    BattleStats stats,
    bool playerWon,
    int round
)
{
    profile.battlesPlayed++;

    if (playerWon)
    {
        profile.wins++;
    }
    else
    {
        profile.losses++;
    }

    profile.totalDamageDealt +=
        stats.playerDirectDamage +
        stats.statusDamageToEnemy;

    profile.totalDamageTaken +=
        stats.enemyDirectDamage +
        stats.statusDamageToPlayer;

    profile.totalRoundsPlayed += round;

    saveProfile(profile);
}

// --------------------------------------------------
// Battle flow
// --------------------------------------------------

void startBattle(PlayerProfile& profile)
{
    showDifficultyMenu();

    int difficulty = getValidInput(1, 3);

    std::cout << "\nChoose your creature:\n";

    showCreatureMenu();

    int playerChoice = getValidInput(1, 8);

    Pokemon playerCreature =
        getCreatureByChoice(playerChoice);

    int randomChoice =
        std::rand() % 8 + 1;

    while (randomChoice == playerChoice)
    {
        randomChoice =
            std::rand() % 8 + 1;
    }

    Pokemon enemyCreature =
        getCreatureByChoice(randomChoice);

    Battle battle(
        playerCreature,
        enemyCreature,
        difficulty
    );

    std::cout << "\n--- Battle Start ---\n";

    battle.getPlayerPokemon().displayStats();
    battle.getEnemyPokemon().displayStats();

    while (!battle.isBattleOver())
    {
        std::cout
            << "\n========= Round "
            << battle.getRound()
            << " =========\n";

        std::cout
            << "\nChoose "
            << battle.getPlayerPokemon().getName()
            << "'s move:\n";

        battle.getPlayerPokemon().showMoves();

        int playerMoveChoice =
            getValidInput(
                1,
                battle
                    .getPlayerPokemon()
                    .getMoveCount()
            );

        battle.playerAttack(
            playerMoveChoice - 1
        );

        if (!battle.isBattleOver())
        {
            std::cout << "\nEnemy's turn...\n";

            battle.enemyTurn();
        }

        if (!battle.isBattleOver())
        {
            std::cout
                << "\n--- End of Round Effects ---\n";

            battle.applyEndRoundEffects();
        }

        if (!battle.isBattleOver())
        {
            std::cout
                << "\n--- Current Stats ---\n";

            battle
                .getPlayerPokemon()
                .displayStats();

            battle
                .getEnemyPokemon()
                .displayStats();
        }
    }

    BattleStats stats =
        battle.getStats();

    bool playerWon =
        battle.didPlayerWin();

    int round =
        battle.getRound();

    showBattleSummary(
        battle.getPlayerPokemon(),
        battle.getEnemyPokemon(),
        stats,
        playerWon,
        round
    );

    updateProfileAfterBattle(
        profile,
        stats,
        playerWon,
        round
    );

    std::cout << "\nProfile saved.\n";
}

// --------------------------------------------------
// Main
// --------------------------------------------------

int main()
{
    std::srand(
        static_cast<unsigned int>(
            std::time(nullptr)
        )
    );

    PlayerProfile profile =
        loadProfile();

    bool gameRunning = true;

    while (gameRunning)
    {
        showMainMenu();

        int menuChoice =
            getValidInput(1, 6);

        if (menuChoice == 1)
        {
            startBattle(profile);
        }

        else if (menuChoice == 2)
        {
            displayProfile(profile);
        }

        else if (menuChoice == 3)
        {
            changePlayerName(profile);
        }

        else if (menuChoice == 4)
        {
            showInstructions();
        }

        else if (menuChoice == 5)
        {
            std::cout
                << "\nAre you sure you want to reset your profile? (y/n): ";

            char confirmReset =
                getYesNoInput();

            if (confirmReset == 'y')
            {
                resetProfile();

                profile =
                    createDefaultProfile();

                std::cout
                    << "Profile reset successfully.\n";
            }
            else
            {
                std::cout
                    << "Profile reset cancelled.\n";
            }
        }

        else if (menuChoice == 6)
        {
            std::cout << "\nExiting game...\n";

            gameRunning = false;
        }
    }

    return 0;
}