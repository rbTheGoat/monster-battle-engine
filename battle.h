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

public:
    Battle(Pokemon player, Pokemon enemy, int selectedDifficulty);

    void playerAttack(int moveIndex);
    void enemyTurn();
    void applyEndRoundEffects();

    bool isBattleOver();
    bool didPlayerWin();

    int getRound();
    BattleStats getStats();

    Pokemon& getPlayerPokemon();
    Pokemon& getEnemyPokemon();
};