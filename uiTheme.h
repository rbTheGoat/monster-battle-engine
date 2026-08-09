#pragma once

#include <SFML/Graphics.hpp>

#include "pokemon.h"

namespace UiTheme
{
    // ==================================================
    // Core colors
    // ==================================================

    sf::Color backgroundOverlay();

    sf::Color panel();
    sf::Color strongPanel();

    sf::Color border();
    sf::Color borderBright();

    sf::Color primary();
    sf::Color primaryHover();

    sf::Color secondary();
    sf::Color secondaryHover();

    sf::Color danger();
    sf::Color dangerHover();

    sf::Color disabled();

    sf::Color textPrimary();
    sf::Color textSecondary();
    sf::Color textMuted();

    sf::Color hpHealthy();
    sf::Color hpMedium();
    sf::Color hpLow();

    sf::Color getTypeColor(Type type);

    // ==================================================
    // Panels
    // ==================================================

    void stylePanel(
        sf::RectangleShape& panelShape,
        bool strong = false
    );

    sf::RectangleShape makePanel(
        sf::Vector2f size,
        sf::Vector2f position,
        bool strong = false
    );

    // ==================================================
    // Buttons
    // ==================================================

    void styleButton(
        sf::RectangleShape& button,
        bool primaryButton = false
    );

    void styleDangerButton(
        sf::RectangleShape& button
    );

    void styleTypeButton(
        sf::RectangleShape& button,
        Type type
    );

    void updateButtonHover(
        sf::RectangleShape& button,
        sf::Vector2f mousePosition,
        bool enabled = true,
        bool primaryButton = false
    );

    void updateDangerButtonHover(
        sf::RectangleShape& button,
        sf::Vector2f mousePosition,
        bool enabled = true
    );

    void updateTypeButtonHover(
        sf::RectangleShape& button,
        sf::Vector2f mousePosition,
        Type type,
        bool enabled = true
    );

    // ==================================================
    // Text helpers
    // ==================================================

    void styleTitle(
        sf::Text& text
    );

    void styleSubtitle(
        sf::Text& text
    );

    void styleBodyText(
        sf::Text& text
    );

    void styleMutedText(
        sf::Text& text
    );

    void centerTextInRectangle(
        sf::Text& text,
        const sf::RectangleShape& rectangle,
        float verticalOffset = -2.0f
    );

    void centerTextHorizontally(
        sf::Text& text,
        float centerX
    );
}