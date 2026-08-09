#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "battle.h"

bool loadPixelTexture(
    sf::Texture& texture,
    const std::string& path
);

void scaleSpriteToSize(
    sf::Sprite& sprite,
    const sf::Texture& texture,
    float targetWidth,
    float targetHeight
);

std::string getCreatureImagePath(
    const std::string& creatureName,
    bool useBackSprite
);

sf::Color getCreatureFallbackColor(
    const std::string& creatureName
);

void setupCreatureVisuals(
    Battle& battle,
    sf::Texture& playerTexture,
    sf::Texture& enemyTexture,
    sf::Sprite& playerSprite,
    sf::Sprite& enemySprite,
    bool& playerTextureLoaded,
    bool& enemyTextureLoaded,
    sf::RectangleShape& playerBody,
    sf::RectangleShape& enemyBody
);

void setCreaturePosition(
    sf::Sprite& sprite,
    sf::RectangleShape& body,
    sf::RectangleShape& eye,
    bool textureLoaded,
    sf::Vector2f spritePosition,
    sf::Vector2f bodyPosition,
    sf::Vector2f eyePosition
);

void setCreatureColor(
    sf::Sprite& sprite,
    sf::RectangleShape& body,
    bool textureLoaded,
    sf::Color color
);