#include "battle.h"

Battle::Battle(Pokemon player, Pokemon enemy, int selectedDifficulty)
    : playerPokemon(player),
      enemyPokemon(enemy),
      difficulty(selectedDifficulty),
      round(1),
      battleOver(false),
      playerWon(false) {
}

void Battle::playerAttack(int moveIndex) {
    if (battleOver) {
        return;
    }

    bool playerCanMove = playerPokemon.canAttack();

    if (playerCanMove) {
        int enemyHpBefore = enemyPokemon.getHp();
        Status enemyStatusBefore = enemyPokemon.getStatus();

        Move playerMove = playerPokemon.getMove(moveIndex);
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
        battleOver = true;
        playerWon = true;
    }
}

void Battle::enemyTurn() {
    if (battleOver) {
        return;
    }

    bool enemyCanMove = enemyPokemon.canAttack();

    if (enemyCanMove) {
        int playerHpBefore = playerPokemon.getHp();
        Status playerStatusBefore = playerPokemon.getStatus();

        Move enemyMove = enemyPokemon.chooseMoveByDifficulty(playerPokemon, difficulty);
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
        battleOver = true;
        playerWon = false;
    }
}

void Battle::applyEndRoundEffects() {
    if (battleOver) {
        return;
    }

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
        battleOver = true;
        playerWon = false;
        return;
    }

    if (enemyPokemon.isFainted()) {
        battleOver = true;
        playerWon = true;
        return;
    }

    round++;
}

bool Battle::isBattleOver() {
    return battleOver;
}

bool Battle::didPlayerWin() {
    return playerWon;
}

int Battle::getRound() {
    return round;
}

BattleStats Battle::getStats() {
    return stats;
}

Pokemon& Battle::getPlayerPokemon() {
    return playerPokemon;
}

Pokemon& Battle::getEnemyPokemon() {
    return enemyPokemon;
}