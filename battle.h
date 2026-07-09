#pragma once

#include "pokemon.h"

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

class Battle {
private:
    Pokemon playerPokemon;
    Pokemon enemyPokemon;

    BattleStats stats;

    int difficulty;
    int round;

    bool battleOver;
    bool playerWon;

    // Tracks whether the enemy passed its status check
    // and is ready to execute the selected move.
    bool enemyReadyToAttack;

public:
    Battle(
        Pokemon player,
        Pokemon enemy,
        int selectedDifficulty
    );

    void playerAttack(int moveIndex);

    // Split enemy turn for SFML animation support.
    bool prepareEnemyTurn();
    Move chooseEnemyMove();
    void executeEnemyMove(const Move& enemyMove);

    // Preserved for the console version.
    void enemyTurn();

    void applyEndRoundEffects();

    bool isBattleOver();
    bool didPlayerWin();

    int getRound();
    BattleStats getStats();

    Pokemon& getPlayerPokemon();
    Pokemon& getEnemyPokemon();
};