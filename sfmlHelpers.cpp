#include "sfmlHelpers.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <string>

float clamp01(float value)
{
    return std::clamp(value, 0.0f, 1.0f);
}

float smoothStep(float value)
{
    value = clamp01(value);

    return value * value * (3.0f - 2.0f * value);
}

float pulse(float time, float speed)
{
    return 0.5f + 0.5f * std::sin(time * speed);
}

sf::Color withAlpha(
    sf::Color color,
    int alpha
)
{
    color.a = static_cast<std::uint8_t>(
        std::clamp(alpha, 0, 255)
        );

    return color;
}

sf::Vector2f interpolatePosition(
    sf::Vector2f start,
    sf::Vector2f end,
    float progress
)
{
    progress = clamp01(progress);

    return {
        start.x + (end.x - start.x) * progress,
        start.y + (end.y - start.y) * progress
    };
}

bool isButtonClicked(
    const sf::RectangleShape& button,
    sf::Vector2f mousePosition
)
{
    return button
        .getGlobalBounds()
        .contains(mousePosition);
}

void updateButtonHover(
    sf::RectangleShape& button,
    sf::Vector2f mousePosition,
    bool enabled
)
{
    if (!enabled) {
        button.setFillColor(
            sf::Color(70, 70, 85)
        );

        return;
    }

    if (
        button
        .getGlobalBounds()
        .contains(mousePosition)
        ) {
        button.setFillColor(
            sf::Color(120, 120, 170)
        );
    }
    else {
        button.setFillColor(
            sf::Color(90, 90, 120)
        );
    }
}

// ==================================================
// Status helpers
// ==================================================

std::string getStatusDisplayText(Status status)
{
    if (status == Status::None) {
        return "Status: None";
    }

    if (status == Status::Burn) {
        return "Status: Burned";
    }

    if (status == Status::Poison) {
        return "Status: Poisoned";
    }

    if (status == Status::Sleep) {
        return "Status: Asleep";
    }

    if (status == Status::Paralysis) {
        return "Status: Paralyzed";
    }

    return "Status: Unknown";
}

sf::Color getStatusColor(Status status)
{
    if (status == Status::None) {
        return sf::Color(90, 90, 90);
    }

    if (status == Status::Burn) {
        return sf::Color(230, 90, 40);
    }

    if (status == Status::Poison) {
        return sf::Color(150, 70, 190);
    }

    if (status == Status::Sleep) {
        return sf::Color(80, 130, 220);
    }

    if (status == Status::Paralysis) {
        return sf::Color(230, 210, 60);
    }

    return sf::Color::White;
}

void updateStatusDisplay(
    Battle& battle,
    sf::Text& playerStatusText,
    sf::Text& enemyStatusText,
    sf::RectangleShape& playerStatusIcon,
    sf::RectangleShape& enemyStatusIcon
)
{
    Status playerStatus =
        battle.getPlayerPokemon().getStatus();

    Status enemyStatus =
        battle.getEnemyPokemon().getStatus();

    playerStatusText.setString(
        getStatusDisplayText(playerStatus)
    );

    enemyStatusText.setString(
        getStatusDisplayText(enemyStatus)
    );

    playerStatusIcon.setFillColor(
        getStatusColor(playerStatus)
    );

    enemyStatusIcon.setFillColor(
        getStatusColor(enemyStatus)
    );
}

// ==================================================
// HP and text helpers
// ==================================================

void updateHpBar(
    sf::RectangleShape& hpFill,
    int currentHp,
    int maxHp,
    float maximumWidth
)
{
    currentHp = std::max(0, currentHp);

    if (maxHp <= 0) {
        hpFill.setSize({
            0.0f,
            hpFill.getSize().y
            });

        return;
    }

    float percentage =
        static_cast<float>(currentHp) /
        static_cast<float>(maxHp);

    percentage = clamp01(percentage);

    hpFill.setSize({
        maximumWidth * percentage,
        hpFill.getSize().y
        });

    if (percentage > 0.5f) {
        hpFill.setFillColor(
            sf::Color(80, 220, 100)
        );
    }
    else if (percentage > 0.25f) {
        hpFill.setFillColor(
            sf::Color(230, 200, 60)
        );
    }
    else {
        hpFill.setFillColor(
            sf::Color(220, 80, 80)
        );
    }
}

std::string makeHpText(
    const std::string& label,
    Pokemon& creature,
    int maxHp
)
{
    return label +
        " HP: " +
        std::to_string(creature.getHp()) +
        " / " +
        std::to_string(maxHp);
}

std::string getMoveNameOrEmpty(
    Pokemon& creature,
    int index
)
{
    if (
        index >= 0 &&
        index < creature.getMoveCount()
        ) {
        return creature
            .getMove(index)
            .name;
    }

    return "Empty";
}

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
)
{
    playerNameText.setString(
        "Player: " +
        battle.getPlayerPokemon().getName()
    );

    enemyNameText.setString(
        "Enemy: " +
        battle.getEnemyPokemon().getName()
    );

    playerHpText.setString(
        makeHpText(
            "Player",
            battle.getPlayerPokemon(),
            playerMaxHp
        )
    );

    enemyHpText.setString(
        makeHpText(
            "Enemy",
            battle.getEnemyPokemon(),
            enemyMaxHp
        )
    );

    moveText1.setString(
        getMoveNameOrEmpty(
            battle.getPlayerPokemon(),
            0
        )
    );

    moveText2.setString(
        getMoveNameOrEmpty(
            battle.getPlayerPokemon(),
            1
        )
    );

    moveText3.setString(
        getMoveNameOrEmpty(
            battle.getPlayerPokemon(),
            2
        )
    );

    moveText4.setString(
        getMoveNameOrEmpty(
            battle.getPlayerPokemon(),
            3
        )
    );
}