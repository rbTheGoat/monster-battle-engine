#include "pokemon.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

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
    return "Unknown";
}

float getTypeMultiplier(Type attackerType, Type defenderType) {
    if (attackerType == Type::Fire     && defenderType == Type::Grass)    return 2.0f;
    if (attackerType == Type::Water    && defenderType == Type::Fire)     return 2.0f;
    if (attackerType == Type::Grass    && defenderType == Type::Water)    return 2.0f;
    if (attackerType == Type::Electric && defenderType == Type::Water)    return 2.0f;
    if (attackerType == Type::Ghost    && defenderType == Type::Ghost)    return 2.0f;
    if (attackerType == Type::Fighting && defenderType == Type::Normal)   return 2.0f;
    if (attackerType == Type::Dragon   && defenderType == Type::Dragon)   return 2.0f;

    if (attackerType == Type::Fire     && defenderType == Type::Water)    return 0.5f;
    if (attackerType == Type::Water    && defenderType == Type::Grass)    return 0.5f;
    if (attackerType == Type::Grass    && defenderType == Type::Fire)     return 0.5f;
    if (attackerType == Type::Normal   && defenderType == Type::Ghost)    return 0.5f;

    return 1.0f;
}

void Pokemon::attack(Pokemon& enemy, Move move) {
    std::cout << name << " used " << move.name << "!\n";

    int hitRoll = rand() % 100 + 1;

    if (hitRoll > move.accuracy) {
        std::cout << "But it missed!\n";
        return;
    }

    int randFactor = rand() % 16 + 85;
    int actualDamage = move.damage * randFactor / 100;

    int critRoll = rand() % 100 + 1;

    if (critRoll <= 10) {
        actualDamage = actualDamage * 1.5;
        std::cout << "Critical Hit!\n";
    }

    float multiplier = getTypeMultiplier(move.type, enemy.getType());
    actualDamage = actualDamage * multiplier;

    if (multiplier == 2.0f) std::cout << "It's super effective!\n";
    if (multiplier == 0.5f) std::cout << "It's not very effective...\n";

    enemy.takeDamage(actualDamage);
    std::cout << "It dealt " << actualDamage << " damage!\n";

    if (move.statusEffect != Status::None && enemy.getStatus() == Status::None) {
        int statusRoll = rand() % 100 + 1;

        if (statusRoll <= move.statusChance) {
            enemy.setStatus(move.statusEffect, 3);

            std::cout << enemy.getName()
                      << " is now "
                      << enemy.getStatusName()
                      << " for 3 turns!\n";
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
    std::cout << "Status: " << getStatusName() << std::endl;
}

void Pokemon::addMove(Move move) {
    moves.push_back(move);
}

void Pokemon::showMoves() {
    for (int i = 0; i < moves.size(); i++) {
        std::cout << i + 1 << ". " << moves[i].name
                  << "  Power: " << moves[i].damage
                  << "  Accuracy: " << moves[i].accuracy << "%\n";
    }
}

Move Pokemon::getMove(int index) {
    return moves[index];
}

int Pokemon::getHp() {
    return hp;
}

Move Pokemon::chooseBestMove(Pokemon& enemy) {
    int enemyHp = enemy.getHp();
    Move strongestMove = moves[0];
    Move weakestKillingMove = moves[0];
    bool foundKillingMove = false;

    for (int i = 0; i < moves.size(); i++) {
        if (moves[i].damage > strongestMove.damage) {
            strongestMove = moves[i];
        }

        if (moves[i].damage >= enemyHp) {
            if (!foundKillingMove || moves[i].damage < weakestKillingMove.damage) {
                weakestKillingMove = moves[i];
                foundKillingMove = true;
            }
        }
    }

    if (foundKillingMove) {
        return weakestKillingMove;
    }

    return strongestMove;
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
    if (status == Status::Burn) return "Burn";
    if (status == Status::Poison) return "Poison";
    if (status == Status::Sleep) return "Sleep";
    if (status == Status::Paralysis) return "Paralysis";
    return "Unknown";
}

void Pokemon::applyStatusEffect() {
    if (status == Status::None) {
        return;
    }

    if (status == Status::Burn) {
        std::cout << name << " is hurt by burn!\n";
        takeDamage(5);
    }

    if (status == Status::Poison) {
        std::cout << name << " is hurt by poison!\n";
        takeDamage(8);
    }

    statusTurnsRemaining--;

    if (statusTurnsRemaining <= 0) {
        std::cout << name << " is no longer " << getStatusName() << "!\n";

        status = Status::None;
        statusTurnsRemaining = 0;
    }
}