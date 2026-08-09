#include "creatureVisuals.h"

#include <filesystem>
#include <iostream>
#include <string>

bool loadPixelTexture(
    sf::Texture& texture,
    const std::string& path
)
{
    if (
        path.empty() ||
        !std::filesystem::exists(path)
        ) {
        return false;
    }

    if (!texture.loadFromFile(path)) {
        std::cout
            << "Failed to load texture: "
            << path
            << "\n";

        return false;
    }

    texture.setSmooth(false);

    return true;
}

void scaleSpriteToSize(
    sf::Sprite& sprite,
    const sf::Texture& texture,
    float targetWidth,
    float targetHeight
)
{
    sf::Vector2u textureSize =
        texture.getSize();

    if (
        textureSize.x == 0 ||
        textureSize.y == 0
        ) {
        return;
    }

    float scaleX =
        targetWidth /
        static_cast<float>(textureSize.x);

    float scaleY =
        targetHeight /
        static_cast<float>(textureSize.y);

    sprite.setScale({
        scaleX,
        scaleY
        });
}

std::string getCreatureImagePath(
    const std::string& creatureName,
    bool useBackSprite
)
{
    std::string suffix =
        useBackSprite
        ? "_back.png"
        : "_front.png";

    if (creatureName == "Voltbit") {
        return "assets/images/voltbit" + suffix;
    }

    if (creatureName == "Emberling") {
        return "assets/images/emberling" + suffix;
    }

    if (creatureName == "Mossback") {
        return "assets/images/mossback" + suffix;
    }

    if (creatureName == "Aquafin") {
        return "assets/images/aquafin" + suffix;
    }

    if (creatureName == "Wildkin") {
        return "assets/images/wildkin" + suffix;
    }

    if (creatureName == "Shadegeist") {
        return "assets/images/shadegeist" + suffix;
    }

    if (creatureName == "Ironfang") {
        return "assets/images/ironfang" + suffix;
    }

    if (creatureName == "Skywyrm") {
        return "assets/images/skywyrm" + suffix;
    }

    return "";
}

sf::Color getCreatureFallbackColor(
    const std::string& creatureName
)
{
    if (creatureName == "Voltbit") {
        return sf::Color(240, 210, 70);
    }

    if (creatureName == "Emberling") {
        return sf::Color(230, 95, 55);
    }

    if (creatureName == "Mossback") {
        return sf::Color(90, 180, 90);
    }

    if (creatureName == "Aquafin") {
        return sf::Color(80, 170, 240);
    }

    if (creatureName == "Wildkin") {
        return sf::Color(170, 120, 75);
    }

    if (creatureName == "Shadegeist") {
        return sf::Color(120, 80, 170);
    }

    if (creatureName == "Ironfang") {
        return sf::Color(70, 100, 180);
    }

    if (creatureName == "Skywyrm") {
        return sf::Color(230, 160, 80);
    }

    return sf::Color(180, 180, 180);
}

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
)
{
    std::string playerName =
        battle.getPlayerPokemon().getName();

    std::string enemyName =
        battle.getEnemyPokemon().getName();

    playerTextureLoaded =
        loadPixelTexture(
            playerTexture,
            getCreatureImagePath(
                playerName,
                true
            )
        );

    enemyTextureLoaded =
        loadPixelTexture(
            enemyTexture,
            getCreatureImagePath(
                enemyName,
                false
            )
        );

    playerBody.setFillColor(
        getCreatureFallbackColor(playerName)
    );

    enemyBody.setFillColor(
        getCreatureFallbackColor(enemyName)
    );

    if (playerTextureLoaded) {
        playerSprite.setTexture(
            playerTexture,
            true
        );

        playerSprite.setPosition({
            180.0f,
            240.0f
            });

        playerSprite.setColor(
            sf::Color::White
        );

        scaleSpriteToSize(
            playerSprite,
            playerTexture,
            210.0f,
            210.0f
        );
    }

    if (enemyTextureLoaded) {
        enemySprite.setTexture(
            enemyTexture,
            true
        );

        enemySprite.setPosition({
            855.0f,
            130.0f
            });

        enemySprite.setColor(
            sf::Color::White
        );

        scaleSpriteToSize(
            enemySprite,
            enemyTexture,
            200.0f,
            200.0f
        );
    }
}

void setCreaturePosition(
    sf::Sprite& sprite,
    sf::RectangleShape& body,
    sf::RectangleShape& eye,
    bool textureLoaded,
    sf::Vector2f spritePosition,
    sf::Vector2f bodyPosition,
    sf::Vector2f eyePosition
)
{
    if (textureLoaded) {
        sprite.setPosition(spritePosition);
    }
    else {
        body.setPosition(bodyPosition);
        eye.setPosition(eyePosition);
    }
}

void setCreatureColor(
    sf::Sprite& sprite,
    sf::RectangleShape& body,
    bool textureLoaded,
    sf::Color color
)
{
    if (textureLoaded) {
        sprite.setColor(color);
    }
    else {
        body.setFillColor(color);
    }
}