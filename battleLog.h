#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "pokemon.h"

void addBattleLog(
    std::vector<std::string>& battleLog,
    const std::string& message
);

void updateBattleLogTexts(
    const std::vector<std::string>& battleLog,
    std::vector<sf::Text>& battleLogTexts
);

std::string getStatusEventText(Status status);

std::string getStatusConditionName(Status status);

void addMoveResultToBattleLog(
    std::vector<std::string>& battleLog,
    const std::string& targetName,
    int hpBefore,
    int hpAfter,
    Status statusBefore,
    Status statusAfter,
    const Move& move
);