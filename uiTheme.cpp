#include "uiTheme.h"

#include <algorithm>
#include <cstdint>

namespace
{
    sf::Color brighten(
        sf::Color color,
        int amount
    )
    {
        auto brightenChannel =
            [amount](std::uint8_t channel)
            {
                return static_cast<std::uint8_t>(
                    std::clamp(
                        static_cast<int>(channel) + amount,
                        0,
                        255
                    )
                    );
            };

        return sf::Color(
            brightenChannel(color.r),
            brightenChannel(color.g),
            brightenChannel(color.b),
            color.a
        );
    }
}

// ==================================================
// Core colors
// ==================================================

sf::Color UiTheme::backgroundOverlay()
{
    return sf::Color(
        10,
        14,
        24,
        70
    );
}

sf::Color UiTheme::panel()
{
    return sf::Color(
        22,
        27,
        39,
        205
    );
}

sf::Color UiTheme::strongPanel()
{
    return sf::Color(
        15,
        19,
        29,
        235
    );
}

sf::Color UiTheme::border()
{
    return sf::Color(
        118,
        126,
        155,
        180
    );
}

sf::Color UiTheme::borderBright()
{
    return sf::Color(
        190,
        198,
        230,
        220
    );
}

sf::Color UiTheme::primary()
{
    return sf::Color(
        91,
        92,
        145,
        235
    );
}

sf::Color UiTheme::primaryHover()
{
    return sf::Color(
        119,
        121,
        184,
        250
    );
}

sf::Color UiTheme::secondary()
{
    return sf::Color(
        46,
        52,
        70,
        225
    );
}

sf::Color UiTheme::secondaryHover()
{
    return sf::Color(
        67,
        75,
        101,
        240
    );
}

sf::Color UiTheme::danger()
{
    return sf::Color(
        112,
        50,
        58,
        225
    );
}

sf::Color UiTheme::dangerHover()
{
    return sf::Color(
        150,
        65,
        74,
        245
    );
}

sf::Color UiTheme::disabled()
{
    return sf::Color(
        55,
        58,
        68,
        180
    );
}

sf::Color UiTheme::textPrimary()
{
    return sf::Color(
        245,
        245,
        248
    );
}

sf::Color UiTheme::textSecondary()
{
    return sf::Color(
        213,
        216,
        228
    );
}

sf::Color UiTheme::textMuted()
{
    return sf::Color(
        165,
        170,
        190
    );
}

sf::Color UiTheme::hpHealthy()
{
    return sf::Color(
        72,
        212,
        111
    );
}

sf::Color UiTheme::hpMedium()
{
    return sf::Color(
        228,
        190,
        67
    );
}

sf::Color UiTheme::hpLow()
{
    return sf::Color(
        220,
        73,
        81
    );
}

// ==================================================
// Type colors
// ==================================================

sf::Color UiTheme::getTypeColor(Type type)
{
    switch (type)
    {
    case Type::Fire:
        return sf::Color(
            224,
            94,
            65
        );

    case Type::Water:
        return sf::Color(
            65,
            150,
            225
        );

    case Type::Grass:
        return sf::Color(
            87,
            177,
            99
        );

    case Type::Electric:
        return sf::Color(
            232,
            195,
            59
        );

    case Type::Ghost:
        return sf::Color(
            145,
            91,
            190
        );

    case Type::Fighting:
        return sf::Color(
            190,
            82,
            70
        );

    case Type::Dragon:
        return sf::Color(
            104,
            93,
            205
        );

    case Type::Normal:
    default:
        return sf::Color(
            145,
            149,
            160
        );
    }
}

// ==================================================
// Panels
// ==================================================

void UiTheme::stylePanel(
    sf::RectangleShape& panelShape,
    bool strong
)
{
    panelShape.setFillColor(
        strong
        ? strongPanel()
        : panel()
    );

    panelShape.setOutlineThickness(
        2.0f
    );

    panelShape.setOutlineColor(
        border()
    );
}

sf::RectangleShape UiTheme::makePanel(
    sf::Vector2f size,
    sf::Vector2f position,
    bool strong
)
{
    sf::RectangleShape result(
        size
    );

    result.setPosition(
        position
    );

    stylePanel(
        result,
        strong
    );

    return result;
}

// ==================================================
// Standard buttons
// ==================================================

void UiTheme::styleButton(
    sf::RectangleShape& button,
    bool primaryButton
)
{
    button.setFillColor(
        primaryButton
        ? primary()
        : secondary()
    );

    button.setOutlineThickness(
        2.0f
    );

    button.setOutlineColor(
        primaryButton
        ? borderBright()
        : border()
    );
}

void UiTheme::styleDangerButton(
    sf::RectangleShape& button
)
{
    button.setFillColor(
        danger()
    );

    button.setOutlineThickness(
        2.0f
    );

    button.setOutlineColor(
        sf::Color(
            180,
            100,
            105,
            190
        )
    );
}

// ==================================================
// Move type buttons
// ==================================================

void UiTheme::styleTypeButton(
    sf::RectangleShape& button,
    Type type
)
{
    sf::Color accent =
        getTypeColor(type);

    button.setFillColor(
        sf::Color(
            29,
            33,
            46,
            235
        )
    );

    button.setOutlineThickness(
        3.0f
    );

    button.setOutlineColor(
        sf::Color(
            accent.r,
            accent.g,
            accent.b,
            220
        )
    );
}

// ==================================================
// Hover handling
// ==================================================

void UiTheme::updateButtonHover(
    sf::RectangleShape& button,
    sf::Vector2f mousePosition,
    bool enabled,
    bool primaryButton
)
{
    if (!enabled)
    {
        button.setFillColor(
            disabled()
        );

        button.setOutlineColor(
            sf::Color(
                85,
                88,
                100,
                150
            )
        );

        return;
    }

    bool hovering =
        button
        .getGlobalBounds()
        .contains(
            mousePosition
        );

    if (primaryButton)
    {
        button.setFillColor(
            hovering
            ? primaryHover()
            : primary()
        );

        button.setOutlineColor(
            hovering
            ? sf::Color::White
            : borderBright()
        );
    }
    else
    {
        button.setFillColor(
            hovering
            ? secondaryHover()
            : secondary()
        );

        button.setOutlineColor(
            hovering
            ? borderBright()
            : border()
        );
    }
}

void UiTheme::updateDangerButtonHover(
    sf::RectangleShape& button,
    sf::Vector2f mousePosition,
    bool enabled
)
{
    if (!enabled)
    {
        button.setFillColor(
            disabled()
        );

        return;
    }

    bool hovering =
        button
        .getGlobalBounds()
        .contains(
            mousePosition
        );

    button.setFillColor(
        hovering
        ? dangerHover()
        : danger()
    );

    button.setOutlineColor(
        hovering
        ? sf::Color(
            235,
            150,
            155
        )
        : sf::Color(
            180,
            100,
            105,
            190
        )
    );
}

void UiTheme::updateTypeButtonHover(
    sf::RectangleShape& button,
    sf::Vector2f mousePosition,
    Type type,
    bool enabled
)
{
    if (!enabled)
    {
        button.setFillColor(
            disabled()
        );

        button.setOutlineColor(
            sf::Color(
                75,
                78,
                90,
                150
            )
        );

        return;
    }

    sf::Color accent =
        getTypeColor(type);

    bool hovering =
        button
        .getGlobalBounds()
        .contains(
            mousePosition
        );

    if (hovering)
    {
        sf::Color hoverAccent =
            brighten(
                accent,
                35
            );

        button.setFillColor(
            sf::Color(
                accent.r / 4,
                accent.g / 4,
                accent.b / 4,
                240
            )
        );

        button.setOutlineColor(
            hoverAccent
        );
    }
    else
    {
        styleTypeButton(
            button,
            type
        );
    }
}

// ==================================================
// Text styling
// ==================================================

void UiTheme::styleTitle(
    sf::Text& text
)
{
    text.setFillColor(
        textPrimary()
    );

    text.setOutlineColor(
        sf::Color(
            10,
            12,
            18,
            190
        )
    );

    text.setOutlineThickness(
        1.5f
    );
}

void UiTheme::styleSubtitle(
    sf::Text& text
)
{
    text.setFillColor(
        textSecondary()
    );
}

void UiTheme::styleBodyText(
    sf::Text& text
)
{
    text.setFillColor(
        textPrimary()
    );
}

void UiTheme::styleMutedText(
    sf::Text& text
)
{
    text.setFillColor(
        textMuted()
    );
}

// ==================================================
// Text positioning
// ==================================================

void UiTheme::centerTextInRectangle(
    sf::Text& text,
    const sf::RectangleShape& rectangle,
    float verticalOffset
)
{
    sf::FloatRect bounds =
        text.getLocalBounds();

    text.setOrigin({
        bounds.position.x +
            bounds.size.x /
            2.0f,

        bounds.position.y +
            bounds.size.y /
            2.0f
        });

    sf::Vector2f rectanglePosition =
        rectangle.getPosition();

    sf::Vector2f rectangleSize =
        rectangle.getSize();

    text.setPosition({
        rectanglePosition.x +
            rectangleSize.x /
            2.0f,

        rectanglePosition.y +
            rectangleSize.y /
            2.0f +
            verticalOffset
        });
}

void UiTheme::centerTextHorizontally(
    sf::Text& text,
    float centerX
)
{
    sf::FloatRect bounds =
        text.getLocalBounds();

    text.setOrigin({
        bounds.position.x +
            bounds.size.x /
            2.0f,

        text.getOrigin().y
        });

    text.setPosition({
        centerX,
        text.getPosition().y
        });
}