#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "battle.h"
#include "pokemon.h"

float clamp01(float value);

float smoothStep(float value);

float pulse(
    float time,
    float speed
);

sf::Color withAlpha(
    sf::Color color,
    int alpha
);

sf::Vector2f interpolatePosition(
    sf::Vector2f start,
    sf::Vector2f end,
    float progress
);

bool isButtonClicked(
    const sf::RectangleShape& button,
    sf::Vector2f mousePosition
);

void updateButtonHover(
    sf::RectangleShape& button,
    sf::Vector2f mousePosition,
    bool enabled = true
);

std::string getStatusDisplayText(Status status);

sf::Color getStatusColor(Status status);

void updateStatusDisplay(
    Battle& battle,
    sf::Text& playerStatusText,
    sf::Text& enemyStatusText,
    sf::RectangleShape& playerStatusIcon,
    sf::RectangleShape& enemyStatusIcon
);

void updateHpBar(
    sf::RectangleShape& hpFill,
    int currentHp,
    int maxHp,
    float maximumWidth
);

std::string makeHpText(
    const std::string& label,
    Pokemon& creature,
    int maxHp
);

std::string getMoveNameOrEmpty(
    Pokemon& creature,
    int index
);

void updateBattleText(
    Battle& battle,
    sf::Text& playerNameText,
    sf::Text& enemyNameText,
    sf::Text& playerHpText,
    sf::Text& enemyHpText,
    sf::Text& moveText1,
    sf::Text& moveText2,
    sf::Text& moveText3,
    sf::Text& moveText4,
    int playerMaxHp,
    int enemyMaxHp
);