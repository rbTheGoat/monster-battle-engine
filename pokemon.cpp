#include "pokemon.h"
#include <iostream>
#include <cstdlib>

// Battle balance constants
const int PERCENT_ROLL_MAX = 100;

const int MIN_DAMAGE_PERCENT = 85;
const int MAX_DAMAGE_PERCENT = 100;

const int CRIT_CHANCE = 10;
const float CRIT_MULTIPLIER = 1.5f;

const int BURN_DAMAGE = 5;
const int POISON_DAMAGE = 8;
const int PARALYSIS_SKIP_CHANCE = 25;

const int AI_BEST_MOVE_CHANCE = 80;
const float STATUS_SCORE_WEIGHT = 0.2f;
const float KO_SCORE_BONUS = 1000.0f;

Pokemon::Pokemon(std::string pokemonName, int pokemonHp, Type pokemonType) {
    name = pokemonName;
    hp = pokemonHp;
    type = pokemonType;
    status = Status::None;
    statusTurnsRemaining = 0;
}

Type Pokemon::getType() {
    return type;
}

std::string Pokemon::getTypeName() {
    if (type == Type::Fire)     return "Fire";
    if (type == Type::Water)    return "Water";
    if (type == Type::Grass)    return "Grass";
    if (type == Type::Electric) return "Electric";
    if (type == Type::Normal)   return "Normal";
    if (type == Type::Ghost)    return "Ghost";
    if (type == Type::Fighting) return "Fighting";
    if (type == Type::Dragon)   return "Dragon";
    if (type == Type::Dark)     return "Dark";
    if (type == Type::Steel)    return "Steel";
    if (type == Type::Flying)   return "Flying";
    if (type == Type::Poison)   return "Poison";

    return "Unknown";
}

float getTypeMultiplier(Type attackerType, Type defenderType) {

    // Super effective matchups
    if (attackerType == Type::Fire     && defenderType == Type::Grass)    return 2.0f;
    if (attackerType == Type::Fire     && defenderType == Type::Steel)    return 2.0f;

    if (attackerType == Type::Water    && defenderType == Type::Fire)     return 2.0f;

    if (attackerType == Type::Grass    && defenderType == Type::Water)    return 2.0f;

    if (attackerType == Type::Electric && defenderType == Type::Water)    return 2.0f;
    if (attackerType == Type::Electric && defenderType == Type::Flying)   return 2.0f;

    if (attackerType == Type::Ghost    && defenderType == Type::Ghost)    return 2.0f;

    if (attackerType == Type::Fighting && defenderType == Type::Normal)   return 2.0f;
    if (attackerType == Type::Fighting && defenderType == Type::Dark)     return 2.0f;
    if (attackerType == Type::Fighting && defenderType == Type::Steel)    return 2.0f;

    if (attackerType == Type::Dragon   && defenderType == Type::Dragon)   return 2.0f;

    if (attackerType == Type::Dark     && defenderType == Type::Ghost)    return 2.0f;

    if (attackerType == Type::Flying   && defenderType == Type::Grass)    return 2.0f;
    if (attackerType == Type::Flying   && defenderType == Type::Fighting) return 2.0f;

    if (attackerType == Type::Poison   && defenderType == Type::Grass)    return 2.0f;

    if (attackerType == Type::Steel    && defenderType == Type::Dragon)   return 2.0f;


    // Not very effective matchups
    if (attackerType == Type::Fire     && defenderType == Type::Water)    return 0.5f;

    if (attackerType == Type::Water    && defenderType == Type::Grass)    return 0.5f;

    if (attackerType == Type::Grass    && defenderType == Type::Fire)     return 0.5f;
    if (attackerType == Type::Grass    && defenderType == Type::Flying)   return 0.5f;
    if (attackerType == Type::Grass    && defenderType == Type::Poison)   return 0.5f;

    if (attackerType == Type::Electric && defenderType == Type::Grass)    return 0.5f;
    if (attackerType == Type::Electric && defenderType == Type::Dragon)   return 0.5f;

    if (attackerType == Type::Normal   && defenderType == Type::Ghost)    return 0.5f;

    if (attackerType == Type::Ghost    && defenderType == Type::Dark)     return 0.5f;

    if (attackerType == Type::Fighting && defenderType == Type::Ghost)    return 0.5f;

    if (attackerType == Type::Dark     && defenderType == Type::Fighting) return 0.5f;

    if (attackerType == Type::Flying   && defenderType == Type::Electric) return 0.5f;
    if (attackerType == Type::Flying   && defenderType == Type::Steel)    return 0.5f;

    if (attackerType == Type::Poison   && defenderType == Type::Ghost)    return 0.5f;
    if (attackerType == Type::Poison   && defenderType == Type::Steel)    return 0.5f;

    if (attackerType == Type::Steel    && defenderType == Type::Fire)     return 0.5f;
    if (attackerType == Type::Steel    && defenderType == Type::Water)    return 0.5f;
    if (attackerType == Type::Steel    && defenderType == Type::Electric) return 0.5f;

    return 1.0f;
}

void Pokemon::attack(Pokemon& enemy, Move move) {
    std::cout << name << " used " << move.name << "!\n";

    int hitRoll = rand() % PERCENT_ROLL_MAX + 1;

    if (hitRoll > move.accuracy) {
        std::cout << "But it missed!\n";
        return;
    }

    int randFactor = rand() % (MAX_DAMAGE_PERCENT - MIN_DAMAGE_PERCENT + 1) + MIN_DAMAGE_PERCENT;
    int actualDamage = move.damage * randFactor / PERCENT_ROLL_MAX;

    int critRoll = rand() % PERCENT_ROLL_MAX + 1;

    if (critRoll <= CRIT_CHANCE && actualDamage > 0) {
        actualDamage = actualDamage * CRIT_MULTIPLIER;
        std::cout << "Critical Hit!\n";
    }

    float multiplier = getTypeMultiplier(move.type, enemy.getType());
    actualDamage = actualDamage * multiplier;

    if (multiplier == 2.0f && actualDamage > 0) {
        std::cout << "It's super effective!\n";
    }

    if (multiplier == 0.5f && actualDamage > 0) {
        std::cout << "It's not very effective...\n";
    }

    if (actualDamage > 0) {
        enemy.takeDamage(actualDamage);
        std::cout << "It dealt " << actualDamage << " damage!\n";
    } else {
        std::cout << "It did no direct damage!\n";
    }

    if (enemy.isFainted()) {
        return;
    }

    if (move.statusEffect != Status::None && enemy.getStatus() == Status::None) {
        int statusRoll = rand() % PERCENT_ROLL_MAX + 1;

        if (statusRoll <= move.statusChance) {
            enemy.setStatus(move.statusEffect, move.statusDuration);

            std::cout << enemy.getName()
                      << " is now "
                      << enemy.getStatusName()
                      << " for "
                      << move.statusDuration
                      << " turns!\n";
        }
    }
}

void Pokemon::takeDamage(int damage) {
    hp -= damage;

    if (hp < 0) {
        hp = 0;
    }
}

std::string Pokemon::getName() {
    return name;
}

void Pokemon::displayStats() {
    std::cout << "Name: " << name << std::endl;
    std::cout << "HP: " << hp << std::endl;
    std::cout << "Type: " << getTypeName() << std::endl;
    std::cout << "Status: " << getStatusName() << std::endl;
}

void Pokemon::addMove(Move move) {
    moves.push_back(move);
}

std::string getMoveTypeName(Type type) {
    if (type == Type::Fire)     return "Fire";
    if (type == Type::Water)    return "Water";
    if (type == Type::Grass)    return "Grass";
    if (type == Type::Electric) return "Electric";
    if (type == Type::Normal)   return "Normal";
    if (type == Type::Ghost)    return "Ghost";
    if (type == Type::Fighting) return "Fighting";
    if (type == Type::Dragon)   return "Dragon";
    if (type == Type::Dark)     return "Dark";
    if (type == Type::Steel)    return "Steel";
    if (type == Type::Flying)   return "Flying";
    if (type == Type::Poison)   return "Poison";

    return "Unknown";
}

std::string getMoveStatusName(Status status) {
    if (status == Status::None) return "None";
    if (status == Status::Burn) return "Burn";
    if (status == Status::Poison) return "Poison";
    if (status == Status::Sleep) return "Sleep";
    if (status == Status::Paralysis) return "Paralysis";

    return "Unknown";
}



void Pokemon::showMoves() {
    for (int i = 0; i < moves.size(); i++) {
        std::cout << i + 1 << ". " << moves[i].name
                  << " | Power: " << moves[i].damage
                  << " | Accuracy: " << moves[i].accuracy << "%"
                  << " | Type: " << getMoveTypeName(moves[i].type);

        if (moves[i].statusEffect != Status::None) {
            std::cout << " | Status: " << getMoveStatusName(moves[i].statusEffect)
                      << " (" << moves[i].statusChance << "%, "
                      << moves[i].statusDuration << " turns)";
        }

        std::cout << "\n";
    }
}

Move Pokemon::getMove(int index) {
    return moves[index];
}

int Pokemon::getMoveCount() {
    return moves.size();
}

int Pokemon::getHp() {
    return hp;
}

Move Pokemon::chooseBestMove(Pokemon& enemy) {
    int bestMoveIndex = 0;
    int secondBestMoveIndex = 0;

    float bestScore = -1.0f;
    float secondBestScore = -1.0f;

    for (int i = 0; i < moves.size(); i++) {
        float multiplier = getTypeMultiplier(moves[i].type, enemy.getType());
        float accuracyFactor = moves[i].accuracy / 100.0f;

        float score = moves[i].damage * multiplier * accuracyFactor;

        if (moves[i].statusEffect != Status::None && enemy.getStatus() == Status::None) {
            score += moves[i].statusChance * STATUS_SCORE_WEIGHT;
        }

        float estimatedDamage = moves[i].damage * multiplier;

        if (estimatedDamage >= enemy.getHp()) {
            score += KO_SCORE_BONUS;
        }

        if (score > bestScore) {
            secondBestScore = bestScore;
            secondBestMoveIndex = bestMoveIndex;

            bestScore = score;
            bestMoveIndex = i;
        }
        else if (score > secondBestScore) {
            secondBestScore = score;
            secondBestMoveIndex = i;
        }
    }

    int aiRoll = rand() % PERCENT_ROLL_MAX + 1;

    if (aiRoll <= AI_BEST_MOVE_CHANCE || moves.size() == 1) {
        return moves[bestMoveIndex];
    }

    return moves[secondBestMoveIndex];
}

bool Pokemon::isFainted() {
    return hp <= 0;
}

Status Pokemon::getStatus() {
    return status;
}

void Pokemon::setStatus(Status newStatus, int turns) {
    status = newStatus;
    statusTurnsRemaining = turns;
}

std::string Pokemon::getStatusName() {
    if (status == Status::None) return "None";
    if (status == Status::Burn) return "Burned";
    if (status == Status::Poison) return "Poisoned";
    if (status == Status::Sleep) return "Asleep";
    if (status == Status::Paralysis) return "Paralyzed";

    return "Unknown";
}

void Pokemon::applyStatusEffect() {
    if (status == Status::None) {
        return;
    }

    if (status == Status::Burn) {
        std::cout << name << " is hurt by burn!\n";
        takeDamage(BURN_DAMAGE);
    }

    if (status == Status::Poison) {
        std::cout << name << " is hurt by poison!\n";
        takeDamage(POISON_DAMAGE);
    }

    statusTurnsRemaining--;

    if (statusTurnsRemaining <= 0) {
        std::cout << name << " is no longer " << getStatusName() << "!\n";

        status = Status::None;
        statusTurnsRemaining = 0;
    }
}

bool Pokemon::canAttack() {
    if (status == Status::Sleep) {
        std::cout << name << " is asleep and cannot move!\n";
        return false;
    }

    if (status == Status::Paralysis) {
        int paralysisRoll = rand() % PERCENT_ROLL_MAX + 1;

        if (paralysisRoll <= PARALYSIS_SKIP_CHANCE) {
            std::cout << name << " is paralyzed and cannot move!\n";
            return false;
        }
    }

    return true;
}


Move Pokemon::chooseMoveByDifficulty(Pokemon& enemy, int difficulty) {

    // Easy difficulty: fully random move
    if (difficulty == 1) {
        int randomIndex = rand() % moves.size();
        return moves[randomIndex];
    }

    int bestMoveIndex = 0;
    int secondBestMoveIndex = 0;

    float bestScore = -1.0f;
    float secondBestScore = -1.0f;

    for (int i = 0; i < moves.size(); i++) {
        float multiplier = getTypeMultiplier(moves[i].type, enemy.getType());
        float accuracyFactor = moves[i].accuracy / 100.0f;

        float score = moves[i].damage * multiplier * accuracyFactor;

        if (moves[i].statusEffect != Status::None && enemy.getStatus() == Status::None) {
            score += moves[i].statusChance * 0.2f;
        }

        float estimatedDamage = moves[i].damage * multiplier;

        if (estimatedDamage >= enemy.getHp()) {
            score += 1000.0f;
        }

        if (score > bestScore) {
            secondBestScore = bestScore;
            secondBestMoveIndex = bestMoveIndex;

            bestScore = score;
            bestMoveIndex = i;
        }
        else if (score > secondBestScore) {
            secondBestScore = score;
            secondBestMoveIndex = i;
        }
    }

    // Hard difficulty: always best move
    if (difficulty == 3) {
        return moves[bestMoveIndex];
    }

    // Normal difficulty: 80% best move, 20% second-best move
    int aiRoll = rand() % 100 + 1;

    if (aiRoll <= 80 || moves.size() == 1) {
        return moves[bestMoveIndex];
    }

    return moves[secondBestMoveIndex];
}