#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "battle.h"
#include "saveSystem.h"

void updateProfileAfterBattleSFML(
    PlayerProfile& profile,
    const BattleStats& stats,
    bool playerWon,
    int round
);

void updateProfileTexts(
    const PlayerProfile& profile,
    std::vector<sf::Text>& profileTexts
);