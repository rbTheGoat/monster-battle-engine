#pragma once
#include <string>
#include <vector>
//Using enum class so that the names are locked inside the enum so that they dont crash if something named the same as them is included later in the code or library
enum class Type{
    Fire,
    Water,
    Grass,
    Electric,
    Normal,
    Ghost,
    Fighting,
    Dragon
};

enum class Status{
    None,
    Burn,
    Poison,
    Sleep,
    Paralysis
};

struct Move{
    std::string name;
    int damage;
    int accuracy;
    Type type;   // attack effectiveness should come from the move's type, not the pokemon's type
    Status statusEffect;
    int statusChance; //chance to apply the status 
    int statusDuration;
};

class Pokemon{
    private:
    std::string name;
    int hp;
    std::vector <Move> moves;
    Type type;
    Status status;
    int statusTurnsRemaining;

    public:
    Pokemon(std::string pokemonName, int pokemonHp, Type pokemonType); //constructor,,runs automatically when an object is created

    std::string getName();
    Type getType();
    std::string getTypeName();
    void attack(Pokemon& enemy,Move move );
    void addMove(Move move);
    void showMoves();
    Move getMove(int index);
    void takeDamage(int damage);
    bool isFainted();
    void displayStats();
    int getHp();
    Status getStatus();
    void setStatus(Status newStatus, int turns);
    void applyStatusEffect();
    std::string getStatusName();
    Move chooseBestMove(Pokemon& enemy);
    bool canAttack();
    int getMoveCount();
};