#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "pokemon.h"

enum class MoveVisual {
    ArcPounce,
    CoilDischarge,
    Flashstep,

    CinderClaw,
    FlameSpit,
    TailSweep,

    VineCoil,
    ThornBurst,
    DreamSpores,
    VenomBloom,

    RiptideRush,
    RazorFin,
    TidalLance,

    BurrowRush,
    SandSlam,
    TremorStomp,

    PhantomDive,
    VoidOrb,
    DreadGaze,
    ShadowTalon,

    AlloyCharge,
    HornBreaker,
    SteelJab,
    Groundbreaker,

    WyrmClaw,
    GaleSweep,
    AzureBeam,

    Generic
};

MoveVisual getMoveVisual(
    const std::string& moveName
);

sf::Color getMoveEffectColor(Type type);

float getMoveDuration(MoveVisual visual);

float getMoveImpactProgress(MoveVisual visual);

void drawMoveEffects(
    sf::RenderWindow& window,
    MoveVisual visual,
    Type moveType,
    bool playerAttacking,
    float elapsed,
    float duration
);