#include "pokemonDatabase.h"

Pokemon getPikachu() {
    Pokemon pikachu("Pikachu", 100, Type::Electric);
    pikachu.addMove({"Thunderbolt", 35, 100, Type::Electric, Status::Paralysis, 10, 2});
    pikachu.addMove({"Quick Attack", 15, 100, Type::Normal, Status::None, 0, 0});
    pikachu.addMove({"Iron Tail", 25, 80, Type::Steel, Status::None, 0, 0});
    return pikachu;
}

Pokemon getCharmander() {
    Pokemon charmander("Charmander", 95, Type::Fire);
    charmander.addMove({"Scratch", 20, 100, Type::Normal, Status::None, 0, 0});
    charmander.addMove({"Ember", 30, 95, Type::Fire, Status::Burn, 20, 3});
    charmander.addMove({"Flamethrower", 40, 90, Type::Fire, Status::Burn, 15, 3});
    return charmander;
}

Pokemon getBulbasaur() {
    Pokemon bulbasaur("Bulbasaur", 110, Type::Grass);
    bulbasaur.addMove({"Vine Whip",    25, 100, Type::Grass, Status::None,  0,   0});
    bulbasaur.addMove({"Razor Leaf",   35,  90, Type::Grass, Status::None,  0,   0});
    bulbasaur.addMove({"Tackle",       15, 100, Type::Normal, Status::None, 0,   0});
    bulbasaur.addMove({"Sleep Powder",  0,  75, Type::Grass, Status::Sleep, 100, 2});
    return bulbasaur;
}

Pokemon getSquirtle() {
    Pokemon squirtle("Squirtle", 105, Type::Water);
    squirtle.addMove({"Water Gun", 30, 100, Type::Water, Status::None, 0, 0});
    squirtle.addMove({"Bubble", 20, 100, Type::Water, Status::None, 0, 0});
    squirtle.addMove({"Headbutt", 25, 90, Type::Normal, Status::None, 0, 0});
    return squirtle;
}

Pokemon getEevee() {
    Pokemon eevee("Eevee", 100, Type::Normal);
    eevee.addMove({"Quick Attack", 20, 100, Type::Normal, Status::None, 0, 0});
    eevee.addMove({"Bite", 30, 95, Type::Normal, Status::None, 0, 0});
    eevee.addMove({"Swift", 25, 100, Type::Normal, Status::None, 0, 0});
    return eevee;
}

Pokemon getGengar() {
    Pokemon gengar("Gengar", 90, Type::Ghost);
    gengar.addMove({"Shadow Ball", 40,  90, Type::Ghost,  Status::None,     0, 0});
    gengar.addMove({"Lick",        20, 100, Type::Ghost,  Status::Paralysis, 20, 2});
    gengar.addMove({"Dark Pulse", 35, 95, Type::Dark, Status::None, 0, 0});
    gengar.addMove({"Poison Gas", 0, 75, Type::Poison, Status::Poison, 100, 4});
    return gengar;
}

Pokemon getLucario() {
    Pokemon lucario("Lucario", 115, Type::Fighting);
    lucario.addMove({"Aura Sphere", 40, 100, Type::Fighting, Status::None, 0, 0});
    lucario.addMove({"Bullet Punch", 25, 100, Type::Steel, Status::None, 0, 0});
    lucario.addMove({"Force Palm", 30, 95, Type::Fighting, Status::Paralysis, 20, 2});
    return lucario;
}

Pokemon getDragonite() {
    Pokemon dragonite("Dragonite", 130, Type::Dragon);
    dragonite.addMove({"Dragon Claw", 40, 95, Type::Dragon, Status::None, 0, 0});
    dragonite.addMove({"Wing Attack", 30, 100, Type::Flying, Status::None, 0, 0});
    dragonite.addMove({"Hyper Beam", 50, 75, Type::Normal, Status::None, 0, 0});
    return dragonite;
}