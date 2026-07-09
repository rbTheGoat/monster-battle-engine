#pragma once

#include <string>
#include <vector>

enum class Type {
    Fire,
    Water,
    Grass,
    Electric,
    Normal,
    Ghost,
    Fighting,
    Dragon,
    Dark,
    Steel,
    Flying,
    Poison
};

enum class Status {
    None,
    Burn,
    Poison,
    Sleep,
    Paralysis
};

enum class MoveAnimation {
    Lunge,
    Projectile,
    Beam,
    StatusPulse,
    Impact
};

struct Move {
    std::string name;
    int damage;
    int accuracy;
    Type type;
    Status statusEffect;
    int statusChance;
    int statusDuration;

    // Default value keeps older move initializers compatible.
    MoveAnimation animation = MoveAnimation::Lunge;
};

float getTypeMultiplier(
    Type attackerType,
    Type defenderType
);

class Pokemon {
private:
    std::string name;
    int hp;
    std::vector<Move> moves;
    Type type;

    Status status;
    int statusTurnsRemaining;

public:
    Pokemon(
        std::string pokemonName,
        int pokemonHp,
        Type pokemonType
    );

    std::string getName();

    Type getType();

    std::string getTypeName();

    void attack(
        Pokemon& enemy,
        Move move
    );

    void addMove(
        Move move
    );

    void showMoves();

    Move getMove(
        int index
    );

    int getMoveCount();

    void takeDamage(
        int damage
    );

    bool isFainted();

    void displayStats();

    int getHp();

    Status getStatus();

    void setStatus(
        Status newStatus,
        int turns
    );

    void applyStatusEffect();

    std::string getStatusName();

    bool canAttack();

    Move chooseBestMove(
        Pokemon& enemy
    );

    Move chooseMoveByDifficulty(
        Pokemon& enemy,
        int difficulty
    );
};