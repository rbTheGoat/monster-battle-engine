#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>

#include "pokemon.h"
#include "pokemonDatabase.h"
#include "battle.h"

// ==================================================
// Screens
// ==================================================

enum class Screen {
    MainMenu,
    DifficultySelect,
    CreatureSelect,
    BattleScreen,
    ResultScreen
};

// Each move has its own visual identity.
// This is separate from MoveAnimation so pokemon.h
// does not need another structural change.
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

// ==================================================
// General helpers
// ==================================================

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
    bool enabled = true
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
// Creature creation
// ==================================================

Pokemon getCreatureByChoice(int choice)
{
    if (choice == 1) return getVoltbit();
    if (choice == 2) return getEmberling();
    if (choice == 3) return getMossback();
    if (choice == 4) return getAquafin();
    if (choice == 5) return getWildkin();
    if (choice == 6) return getShadegeist();
    if (choice == 7) return getIronfang();
    if (choice == 8) return getSkywyrm();

    return getVoltbit();
}

// ==================================================
// Battle log helpers
// ==================================================

void addBattleLog(
    std::vector<std::string>& battleLog,
    const std::string& message
)
{
    if (message.empty()) {
        return;
    }

    battleLog.push_back(message);

    const std::size_t maximumLogLines = 5;

    while (battleLog.size() > maximumLogLines) {
        battleLog.erase(battleLog.begin());
    }
}

void updateBattleLogTexts(
    const std::vector<std::string>& battleLog,
    std::vector<sf::Text>& battleLogTexts
)
{
    for (
        std::size_t i = 0;
        i < battleLogTexts.size();
        i++
        ) {
        if (i < battleLog.size()) {
            battleLogTexts[i].setString(
                battleLog[i]
            );
        }
        else {
            battleLogTexts[i].setString("");
        }
    }
}

std::string getStatusEventText(Status status)
{
    if (status == Status::Burn) {
        return "burned";
    }

    if (status == Status::Poison) {
        return "poisoned";
    }

    if (status == Status::Sleep) {
        return "asleep";
    }

    if (status == Status::Paralysis) {
        return "paralyzed";
    }

    return "affected";
}

std::string getStatusConditionName(Status status)
{
    if (status == Status::Burn) {
        return "burn";
    }

    if (status == Status::Poison) {
        return "poison";
    }

    if (status == Status::Sleep) {
        return "sleep";
    }

    if (status == Status::Paralysis) {
        return "paralysis";
    }

    return "status effect";
}

void addMoveResultToBattleLog(
    std::vector<std::string>& battleLog,
    const std::string& targetName,
    int hpBefore,
    int hpAfter,
    Status statusBefore,
    Status statusAfter,
    const Move& move
)
{
    int damageTaken =
        hpBefore - hpAfter;

    bool damageLogged = false;
    bool statusLogged = false;

    if (damageTaken > 0) {
        addBattleLog(
            battleLog,
            targetName +
            " took " +
            std::to_string(damageTaken) +
            " damage."
        );

        damageLogged = true;
    }

    if (
        statusBefore == Status::None &&
        statusAfter != Status::None
        ) {
        addBattleLog(
            battleLog,
            targetName +
            " became " +
            getStatusEventText(statusAfter) +
            "."
        );

        statusLogged = true;
    }

    if (!damageLogged && !statusLogged) {
        if (move.damage > 0) {
            addBattleLog(
                battleLog,
                move.name +
                " caused no damage."
            );
        }
        else {
            addBattleLog(
                battleLog,
                move.name +
                " had no visible effect."
            );
        }
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

// ==================================================
// Texture and creature helpers
// ==================================================

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

// ==================================================
// Move visual mapping
// ==================================================

MoveVisual getMoveVisual(
    const std::string& moveName
)
{
    if (moveName == "Arc Pounce") {
        return MoveVisual::ArcPounce;
    }

    if (moveName == "Coil Discharge") {
        return MoveVisual::CoilDischarge;
    }

    if (moveName == "Flashstep") {
        return MoveVisual::Flashstep;
    }

    if (moveName == "Cinder Claw") {
        return MoveVisual::CinderClaw;
    }

    if (moveName == "Flame Spit") {
        return MoveVisual::FlameSpit;
    }

    if (moveName == "Tail Sweep") {
        return MoveVisual::TailSweep;
    }

    if (moveName == "Vine Coil") {
        return MoveVisual::VineCoil;
    }

    if (moveName == "Thorn Burst") {
        return MoveVisual::ThornBurst;
    }

    if (moveName == "Dream Spores") {
        return MoveVisual::DreamSpores;
    }

    if (moveName == "Venom Bloom") {
        return MoveVisual::VenomBloom;
    }

    if (moveName == "Riptide Rush") {
        return MoveVisual::RiptideRush;
    }

    if (moveName == "Razor Fin") {
        return MoveVisual::RazorFin;
    }

    if (moveName == "Tidal Lance") {
        return MoveVisual::TidalLance;
    }

    if (moveName == "Burrow Rush") {
        return MoveVisual::BurrowRush;
    }

    if (moveName == "Sand Slam") {
        return MoveVisual::SandSlam;
    }

    if (moveName == "Tremor Stomp") {
        return MoveVisual::TremorStomp;
    }

    if (moveName == "Phantom Dive") {
        return MoveVisual::PhantomDive;
    }

    if (moveName == "Void Orb") {
        return MoveVisual::VoidOrb;
    }

    if (moveName == "Dread Gaze") {
        return MoveVisual::DreadGaze;
    }

    if (moveName == "Shadow Talon") {
        return MoveVisual::ShadowTalon;
    }

    if (moveName == "Alloy Charge") {
        return MoveVisual::AlloyCharge;
    }

    if (moveName == "Horn Breaker") {
        return MoveVisual::HornBreaker;
    }

    if (moveName == "Steel Jab") {
        return MoveVisual::SteelJab;
    }

    if (moveName == "Groundbreaker") {
        return MoveVisual::Groundbreaker;
    }

    if (moveName == "Wyrm Claw") {
        return MoveVisual::WyrmClaw;
    }

    if (moveName == "Gale Sweep") {
        return MoveVisual::GaleSweep;
    }

    if (moveName == "Azure Beam") {
        return MoveVisual::AzureBeam;
    }

    return MoveVisual::Generic;
}

sf::Color getMoveEffectColor(Type type)
{
    if (type == Type::Electric) {
        return sf::Color(255, 230, 60);
    }

    if (type == Type::Fire) {
        return sf::Color(255, 95, 35);
    }

    if (type == Type::Water) {
        return sf::Color(55, 180, 255);
    }

    if (type == Type::Grass) {
        return sf::Color(85, 220, 100);
    }

    if (type == Type::Ghost) {
        return sf::Color(155, 75, 225);
    }

    if (type == Type::Dark) {
        return sf::Color(80, 45, 125);
    }

    if (type == Type::Steel) {
        return sf::Color(195, 210, 225);
    }

    if (type == Type::Fighting) {
        return sf::Color(225, 80, 55);
    }

    if (type == Type::Dragon) {
        return sf::Color(75, 120, 255);
    }

    if (type == Type::Flying) {
        return sf::Color(190, 225, 255);
    }

    if (type == Type::Poison) {
        return sf::Color(180, 60, 205);
    }

    return sf::Color(235, 235, 235);
}

float getMoveDuration(MoveVisual visual)
{
    if (
        visual == MoveVisual::AzureBeam ||
        visual == MoveVisual::CoilDischarge
        ) {
        return 1.20f;
    }

    if (
        visual == MoveVisual::DreamSpores ||
        visual == MoveVisual::VenomBloom ||
        visual == MoveVisual::DreadGaze
        ) {
        return 1.10f;
    }

    if (
        visual == MoveVisual::BurrowRush ||
        visual == MoveVisual::PhantomDive
        ) {
        return 1.00f;
    }

    return 0.85f;
}

float getMoveImpactProgress(MoveVisual visual)
{
    if (
        visual == MoveVisual::AzureBeam ||
        visual == MoveVisual::CoilDischarge
        ) {
        return 0.68f;
    }

    if (
        visual == MoveVisual::DreamSpores ||
        visual == MoveVisual::VenomBloom ||
        visual == MoveVisual::DreadGaze
        ) {
        return 0.72f;
    }

    return 0.58f;
}

// ==================================================
// Shape drawing helpers
// ==================================================

void drawLine(
    sf::RenderWindow& window,
    sf::Vector2f start,
    sf::Vector2f end,
    float thickness,
    sf::Color color
)
{
    float dx = end.x - start.x;
    float dy = end.y - start.y;

    float length =
        std::sqrt(dx * dx + dy * dy);

    if (length <= 0.0f) {
        return;
    }

    float angle =
        std::atan2(dy, dx) *
        180.0f /
        3.14159265358979323846f;

    sf::RectangleShape line({
        length,
        thickness
        });

    line.setOrigin({
        0.0f,
        thickness / 2.0f
        });

    line.setPosition(start);
    line.setRotation(sf::degrees(angle));
    line.setFillColor(color);

    window.draw(line);
}

void drawRing(
    sf::RenderWindow& window,
    sf::Vector2f center,
    float radius,
    float thickness,
    sf::Color color
)
{
    sf::CircleShape ring(radius);

    ring.setOrigin({
        radius,
        radius
        });

    ring.setPosition(center);
    ring.setFillColor(
        sf::Color::Transparent
    );

    ring.setOutlineThickness(thickness);
    ring.setOutlineColor(color);

    window.draw(ring);
}

void drawFilledCircle(
    sf::RenderWindow& window,
    sf::Vector2f center,
    float radius,
    sf::Color color
)
{
    sf::CircleShape circle(radius);

    circle.setOrigin({
        radius,
        radius
        });

    circle.setPosition(center);
    circle.setFillColor(color);

    window.draw(circle);
}

void drawSparkBurst(
    sf::RenderWindow& window,
    sf::Vector2f center,
    float radius,
    sf::Color color,
    int amount
)
{
    for (int i = 0; i < amount; i++) {
        float angle =
            (
                6.28318530718f *
                static_cast<float>(i)
                ) /
            static_cast<float>(amount);

        sf::Vector2f inner({
            center.x + std::cos(angle) * radius * 0.3f,
            center.y + std::sin(angle) * radius * 0.3f
            });

        sf::Vector2f outer({
            center.x + std::cos(angle) * radius,
            center.y + std::sin(angle) * radius
            });

        drawLine(
            window,
            inner,
            outer,
            4.0f,
            color
        );
    }
}

void drawLightningBolt(
    sf::RenderWindow& window,
    sf::Vector2f start,
    sf::Vector2f end,
    sf::Color color,
    float time
)
{
    const int segments = 8;

    sf::Vector2f previous = start;

    for (int i = 1; i <= segments; i++) {
        float progress =
            static_cast<float>(i) /
            static_cast<float>(segments);

        sf::Vector2f current =
            interpolatePosition(
                start,
                end,
                progress
            );

        if (i < segments) {
            float offset =
                std::sin(
                    time * 70.0f +
                    static_cast<float>(i) * 2.7f
                ) * 18.0f;

            current.x += offset;
        }

        drawLine(
            window,
            previous,
            current,
            8.0f,
            withAlpha(color, 245)
        );

        drawLine(
            window,
            previous,
            current,
            3.0f,
            sf::Color::White
        );

        previous = current;
    }
}

void drawFloatingParticles(
    sf::RenderWindow& window,
    sf::Vector2f center,
    float time,
    sf::Color color,
    int amount,
    float spread,
    float radius
)
{
    for (int i = 0; i < amount; i++) {
        float phase =
            static_cast<float>(i) * 1.85f;

        float x =
            center.x +
            std::sin(
                time * 2.8f + phase
            ) * spread;

        float y =
            center.y +
            std::cos(
                time * 2.0f + phase
            ) * spread * 0.55f -
            std::fmod(
                time * 45.0f +
                static_cast<float>(i) * 17.0f,
                spread
            );

        drawFilledCircle(
            window,
            { x, y },
            radius +
            static_cast<float>(i % 3),
            withAlpha(
                color,
                130 + (i % 3) * 35
            )
        );
    }
}

// ==================================================
// Move-effect drawing
// ==================================================

void drawMoveEffects(
    sf::RenderWindow& window,
    MoveVisual visual,
    Type moveType,
    bool playerAttacking,
    float elapsed,
    float duration
)
{
    float progress =
        clamp01(elapsed / duration);

    float impactProgress =
        getMoveImpactProgress(visual);

    sf::Vector2f playerCenter({
        285.0f,
        335.0f
        });

    sf::Vector2f enemyCenter({
        955.0f,
        230.0f
        });

    sf::Vector2f attackerCenter =
        playerAttacking
        ? playerCenter
        : enemyCenter;

    sf::Vector2f targetCenter =
        playerAttacking
        ? enemyCenter
        : playerCenter;

    sf::Color color =
        getMoveEffectColor(moveType);

    float direction =
        playerAttacking
        ? 1.0f
        : -1.0f;

    // --------------------------------------------------
    // VOLTBIT
    // --------------------------------------------------

    if (visual == MoveVisual::ArcPounce) {
        if (progress > 0.30f) {
            float boltStrength =
                clamp01(
                    (progress - 0.30f) /
                    0.25f
                );

            drawLightningBolt(
                window,
                {
                    targetCenter.x,
                    -20.0f
                },
                targetCenter,
                withAlpha(
                    sf::Color(255, 235, 70),
                    static_cast<int>(
                        255.0f * boltStrength
                        )
                ),
                elapsed
            );

            drawSparkBurst(
                window,
                targetCenter,
                45.0f +
                pulse(elapsed, 25.0f) *
                20.0f,
                sf::Color(255, 235, 70),
                10
            );
        }

        return;
    }

    if (visual == MoveVisual::CoilDischarge) {
        float chargeProgress =
            clamp01(progress / 0.48f);

        for (int i = 0; i < 3; i++) {
            drawRing(
                window,
                attackerCenter,
                30.0f +
                chargeProgress * 65.0f +
                static_cast<float>(i) * 16.0f,
                4.0f,
                withAlpha(
                    color,
                    200 - i * 45
                )
            );
        }

        if (progress > 0.48f) {
            drawLightningBolt(
                window,
                attackerCenter,
                targetCenter,
                color,
                elapsed
            );

            drawLightningBolt(
                window,
                {
                    attackerCenter.x,
                    attackerCenter.y - 18.0f
                },
                {
                    targetCenter.x,
                    targetCenter.y + 22.0f
                },
                withAlpha(color, 190),
                elapsed + 0.13f
            );
        }

        return;
    }

    if (visual == MoveVisual::Flashstep) {
        float travel =
            progress < impactProgress
            ? progress / impactProgress
            : 1.0f;

        sf::Vector2f current =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(travel)
            );

        for (int i = 1; i <= 4; i++) {
            drawLine(
                window,
                {
                    current.x -
                        direction *
                        static_cast<float>(i) *
                        42.0f,
                    current.y +
                        static_cast<float>(i % 2) *
                        8.0f
                },
                {
                    current.x -
                        direction *
                        static_cast<float>(i) *
                        8.0f,
                    current.y
                },
                5.0f,
                withAlpha(
                    sf::Color::White,
                    190 - i * 32
                )
            );
        }

        if (progress > impactProgress - 0.06f) {
            drawSparkBurst(
                window,
                targetCenter,
                55.0f,
                sf::Color::White,
                8
            );
        }

        return;
    }

    // --------------------------------------------------
    // EMBERLING
    // --------------------------------------------------

    if (visual == MoveVisual::CinderClaw) {
        if (progress > 0.38f) {
            float slashProgress =
                clamp01(
                    (progress - 0.38f) /
                    0.26f
                );

            for (int i = -1; i <= 1; i++) {
                sf::Vector2f start({
                    targetCenter.x -
                        direction * 55.0f,
                    targetCenter.y -
                        55.0f +
                        static_cast<float>(i) * 16.0f
                    });

                sf::Vector2f end({
                    targetCenter.x +
                        direction *
                        55.0f *
                        slashProgress,
                    targetCenter.y +
                        50.0f +
                        static_cast<float>(i) * 16.0f
                    });

                drawLine(
                    window,
                    start,
                    end,
                    7.0f,
                    withAlpha(
                        sf::Color(255, 115, 35),
                        220
                    )
                );
            }

            drawFloatingParticles(
                window,
                targetCenter,
                elapsed,
                sf::Color(255, 125, 40),
                8,
                55.0f,
                4.0f
            );
        }

        return;
    }

    if (visual == MoveVisual::FlameSpit) {
        float travel =
            clamp01(progress / impactProgress);

        sf::Vector2f fireballPosition =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(travel)
            );

        if (progress < impactProgress) {
            drawFilledCircle(
                window,
                fireballPosition,
                18.0f +
                pulse(elapsed, 30.0f) *
                7.0f,
                sf::Color(255, 90, 25)
            );

            drawFilledCircle(
                window,
                fireballPosition,
                9.0f,
                sf::Color(255, 225, 90)
            );

            for (int i = 1; i <= 4; i++) {
                drawFilledCircle(
                    window,
                    {
                        fireballPosition.x -
                            direction *
                            static_cast<float>(i) *
                            16.0f,
                        fireballPosition.y +
                            std::sin(
                                elapsed * 20.0f +
                                static_cast<float>(i)
                            ) * 7.0f
                    },
                    7.0f -
                    static_cast<float>(i),
                    withAlpha(
                        sf::Color(255, 115, 35),
                        210 - i * 35
                    )
                );
            }
        }
        else {
            float explosion =
                clamp01(
                    (progress - impactProgress) /
                    (1.0f - impactProgress)
                );

            drawRing(
                window,
                targetCenter,
                25.0f + explosion * 85.0f,
                10.0f,
                withAlpha(
                    sf::Color(255, 80, 20),
                    static_cast<int>(
                        255.0f *
                        (1.0f - explosion)
                        )
                )
            );

            drawSparkBurst(
                window,
                targetCenter,
                45.0f + explosion * 55.0f,
                sf::Color(255, 135, 35),
                12
            );
        }

        return;
    }

    if (visual == MoveVisual::TailSweep) {
        if (progress > 0.30f) {
            float sweep =
                clamp01(
                    (progress - 0.30f) /
                    0.35f
                );

            for (int i = 0; i < 3; i++) {
                float offset =
                    static_cast<float>(i) *
                    15.0f;

                drawLine(
                    window,
                    {
                        targetCenter.x -
                            direction *
                            (80.0f - offset),
                        targetCenter.y +
                            45.0f + offset
                    },
                    {
                        targetCenter.x +
                            direction *
                            70.0f *
                            sweep,
                        targetCenter.y -
                            35.0f + offset
                    },
                    6.0f,
                    withAlpha(
                        sf::Color(255, 160, 70),
                        220 - i * 45
                    )
                );
            }
        }

        return;
    }

    // --------------------------------------------------
    // MOSSBACK
    // --------------------------------------------------

    if (visual == MoveVisual::VineCoil) {
        if (progress > 0.25f) {
            float coilProgress =
                clamp01(
                    (progress - 0.25f) /
                    0.55f
                );

            for (int i = 0; i < 4; i++) {
                float radius =
                    95.0f -
                    coilProgress * 45.0f +
                    static_cast<float>(i) * 10.0f;

                drawRing(
                    window,
                    {
                        targetCenter.x,
                        targetCenter.y +
                            static_cast<float>(i - 2) *
                            12.0f
                    },
                    radius,
                    7.0f,
                    withAlpha(
                        sf::Color(60, 190, 85),
                        210 - i * 25
                    )
                );
            }
        }

        return;
    }

    if (visual == MoveVisual::ThornBurst) {
        for (int i = 0; i < 3; i++) {
            float localProgress =
                clamp01(
                    (
                        progress -
                        static_cast<float>(i) * 0.12f
                        ) /
                    0.58f
                );

            sf::Vector2f position =
                interpolatePosition(
                    attackerCenter,
                    targetCenter,
                    smoothStep(localProgress)
                );

            sf::RectangleShape thorn({
                36.0f,
                8.0f
                });

            thorn.setOrigin({
                18.0f,
                4.0f
                });

            thorn.setPosition({
                position.x,
                position.y +
                    static_cast<float>(i - 1) *
                    25.0f
                });

            thorn.setFillColor(
                sf::Color(90, 220, 95)
            );

            thorn.setRotation(
                sf::degrees(
                    playerAttacking
                    ? -10.0f
                    : 170.0f
                )
            );

            window.draw(thorn);
        }

        if (progress > impactProgress) {
            drawSparkBurst(
                window,
                targetCenter,
                50.0f,
                sf::Color(100, 235, 105),
                8
            );
        }

        return;
    }

    if (visual == MoveVisual::DreamSpores) {
        float fadeIn =
            clamp01(progress / 0.35f);

        drawFloatingParticles(
            window,
            targetCenter,
            elapsed,
            withAlpha(
                sf::Color(120, 150, 255),
                static_cast<int>(
                    220.0f * fadeIn
                    )
            ),
            16,
            95.0f,
            7.0f
        );

        for (int i = 0; i < 3; i++) {
            float radius =
                35.0f +
                std::fmod(
                    elapsed * 40.0f +
                    static_cast<float>(i) * 30.0f,
                    95.0f
                );

            drawRing(
                window,
                targetCenter,
                radius,
                3.0f,
                withAlpha(
                    sf::Color(105, 125, 235),
                    160 - i * 35
                )
            );
        }

        return;
    }

    if (visual == MoveVisual::VenomBloom) {
        float bloom =
            smoothStep(
                clamp01(progress / 0.55f)
            );

        for (int i = 0; i < 6; i++) {
            float angle =
                static_cast<float>(i) *
                1.0471975512f;

            sf::Vector2f petalCenter({
                targetCenter.x +
                    std::cos(angle) *
                    48.0f *
                    bloom,
                targetCenter.y +
                    std::sin(angle) *
                    35.0f *
                    bloom
                });

            drawFilledCircle(
                window,
                petalCenter,
                19.0f * bloom,
                withAlpha(
                    sf::Color(175, 55, 205),
                    185
                )
            );
        }

        drawFloatingParticles(
            window,
            targetCenter,
            elapsed,
            sf::Color(205, 80, 225),
            12,
            80.0f,
            6.0f
        );

        return;
    }

    // --------------------------------------------------
    // AQUAFIN
    // --------------------------------------------------

    if (visual == MoveVisual::RiptideRush) {
        float travel =
            clamp01(progress / impactProgress);

        sf::Vector2f current =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(travel)
            );

        for (int i = 0; i < 5; i++) {
            drawRing(
                window,
                {
                    current.x -
                        direction *
                        static_cast<float>(i) *
                        25.0f,
                    current.y +
                        static_cast<float>(i % 2) *
                        8.0f
                },
                20.0f +
                static_cast<float>(i) *
                7.0f,
                6.0f,
                withAlpha(
                    sf::Color(70, 190, 255),
                    210 - i * 32
                )
            );
        }

        if (progress > impactProgress) {
            drawSparkBurst(
                window,
                targetCenter,
                75.0f,
                sf::Color(100, 220, 255),
                14
            );
        }

        return;
    }

    if (visual == MoveVisual::RazorFin) {
        float travel =
            clamp01(progress / impactProgress);

        sf::Vector2f current =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(travel)
            );

        drawLine(
            window,
            {
                current.x -
                    direction * 38.0f,
                current.y + 45.0f
            },
            {
                current.x +
                    direction * 38.0f,
                current.y
            },
            9.0f,
            sf::Color(100, 225, 255)
        );

        drawLine(
            window,
            {
                current.x +
                    direction * 38.0f,
                current.y
            },
            {
                current.x -
                    direction * 28.0f,
                current.y - 42.0f
            },
            9.0f,
            sf::Color(100, 225, 255)
        );

        return;
    }

    if (visual == MoveVisual::TidalLance) {
        float travel =
            clamp01(progress / impactProgress);

        sf::Vector2f currentEnd =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(travel)
            );

        drawLine(
            window,
            attackerCenter,
            currentEnd,
            11.0f,
            sf::Color(70, 185, 255)
        );

        drawLine(
            window,
            attackerCenter,
            currentEnd,
            4.0f,
            sf::Color::White
        );

        if (progress > impactProgress) {
            drawSparkBurst(
                window,
                targetCenter,
                80.0f,
                sf::Color(85, 205, 255),
                16
            );

            for (int i = 0; i < 7; i++) {
                drawFilledCircle(
                    window,
                    {
                        targetCenter.x +
                            std::sin(
                                elapsed * 13.0f +
                                static_cast<float>(i)
                            ) * 65.0f,
                        targetCenter.y +
                            std::cos(
                                elapsed * 9.0f +
                                static_cast<float>(i)
                            ) * 50.0f
                    },
                    6.0f,
                    sf::Color(100, 215, 255)
                );
            }
        }

        return;
    }

    // --------------------------------------------------
    // WILDKIN
    // --------------------------------------------------

    if (visual == MoveVisual::BurrowRush) {
        float trailProgress =
            clamp01(
                (progress - 0.22f) /
                0.45f
            );

        sf::Vector2f trailPosition =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(trailProgress)
            );

        for (int i = 0; i < 8; i++) {
            float dustOffset =
                static_cast<float>(i) *
                18.0f;

            drawFilledCircle(
                window,
                {
                    trailPosition.x -
                        direction * dustOffset,
                    410.0f +
                        std::sin(
                            elapsed * 15.0f +
                            static_cast<float>(i)
                        ) * 8.0f
                },
                8.0f +
                static_cast<float>(i % 3) *
                3.0f,
                withAlpha(
                    sf::Color(190, 155, 95),
                    190 - i * 15
                )
            );
        }

        if (progress > impactProgress) {
            drawSparkBurst(
                window,
                targetCenter,
                60.0f,
                sf::Color(205, 170, 105),
                10
            );
        }

        return;
    }

    if (visual == MoveVisual::SandSlam) {
        if (progress > 0.28f) {
            drawFloatingParticles(
                window,
                targetCenter,
                elapsed,
                sf::Color(205, 170, 100),
                22,
                100.0f,
                8.0f
            );

            for (int i = 0; i < 4; i++) {
                drawRing(
                    window,
                    targetCenter,
                    35.0f +
                    static_cast<float>(i) *
                    22.0f +
                    pulse(
                        elapsed +
                        static_cast<float>(i),
                        8.0f
                    ) * 12.0f,
                    5.0f,
                    withAlpha(
                        sf::Color(210, 175, 105),
                        170 - i * 25
                    )
                );
            }
        }

        return;
    }

    if (visual == MoveVisual::TremorStomp) {
        if (progress > 0.25f) {
            float wave =
                clamp01(
                    (progress - 0.25f) /
                    0.65f
                );

            for (int i = 0; i < 4; i++) {
                float radius =
                    wave * 140.0f +
                    static_cast<float>(i) * 28.0f;

                drawRing(
                    window,
                    {
                        targetCenter.x,
                        408.0f
                    },
                    radius,
                    5.0f,
                    withAlpha(
                        sf::Color(205, 165, 95),
                        205 - i * 40
                    )
                );
            }

            for (int i = -2; i <= 2; i++) {
                drawLine(
                    window,
                    {
                        targetCenter.x +
                            static_cast<float>(i) *
                            30.0f,
                        405.0f
                    },
                    {
                        targetCenter.x +
                            static_cast<float>(i) *
                            45.0f,
                        435.0f
                    },
                    5.0f,
                    sf::Color(120, 90, 55)
                );
            }
        }

        return;
    }

    // --------------------------------------------------
    // SHADEGEIST
    // --------------------------------------------------

    if (visual == MoveVisual::PhantomDive) {
        float travel =
            clamp01(progress / impactProgress);

        sf::Vector2f current =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(travel)
            );

        for (int i = 1; i <= 5; i++) {
            drawRing(
                window,
                {
                    current.x -
                        direction *
                        static_cast<float>(i) *
                        30.0f,
                    current.y
                },
                18.0f +
                static_cast<float>(i) *
                5.0f,
                4.0f,
                withAlpha(
                    sf::Color(145, 65, 220),
                    190 - i * 28
                )
            );
        }

        if (progress > impactProgress) {
            drawRing(
                window,
                targetCenter,
                80.0f *
                (
                    1.0f -
                    clamp01(
                        (
                            progress -
                            impactProgress
                            ) /
                        0.25f
                    )
                    ),
                10.0f,
                sf::Color(165, 75, 235)
            );
        }

        return;
    }

    if (visual == MoveVisual::VoidOrb) {
        float travel =
            clamp01(progress / impactProgress);

        sf::Vector2f orbPosition =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(travel)
            );

        if (progress < impactProgress) {
            float orbRadius =
                20.0f +
                pulse(elapsed, 11.0f) *
                10.0f;

            drawFilledCircle(
                window,
                orbPosition,
                orbRadius,
                sf::Color(50, 25, 80)
            );

            drawRing(
                window,
                orbPosition,
                orbRadius + 12.0f,
                6.0f,
                sf::Color(165, 75, 230)
            );
        }
        else {
            float collapse =
                clamp01(
                    (
                        progress -
                        impactProgress
                        ) /
                    0.28f
                );

            drawRing(
                window,
                targetCenter,
                95.0f *
                (1.0f - collapse),
                12.0f,
                withAlpha(
                    sf::Color(160, 70, 225),
                    230
                )
            );

            drawFilledCircle(
                window,
                targetCenter,
                55.0f *
                (1.0f - collapse),
                withAlpha(
                    sf::Color(40, 20, 65),
                    230
                )
            );
        }

        return;
    }

    if (visual == MoveVisual::DreadGaze) {
        float gaze =
            smoothStep(
                clamp01(progress / 0.50f)
            );

        drawFilledCircle(
            window,
            targetCenter,
            55.0f * gaze,
            withAlpha(
                sf::Color(155, 70, 220),
                150
            )
        );

        drawFilledCircle(
            window,
            targetCenter,
            25.0f * gaze,
            sf::Color(30, 10, 45)
        );

        drawFilledCircle(
            window,
            targetCenter,
            8.0f * gaze,
            sf::Color(235, 210, 255)
        );

        for (int i = 0; i < 3; i++) {
            drawRing(
                window,
                targetCenter,
                70.0f +
                static_cast<float>(i) *
                25.0f +
                pulse(
                    elapsed +
                    static_cast<float>(i),
                    5.0f
                ) * 12.0f,
                4.0f,
                withAlpha(
                    sf::Color(150, 70, 220),
                    170 - i * 35
                )
            );
        }

        return;
    }

    if (visual == MoveVisual::ShadowTalon) {
        if (progress > 0.34f) {
            float slashProgress =
                clamp01(
                    (progress - 0.34f) /
                    0.30f
                );

            for (int i = -1; i <= 1; i++) {
                drawLine(
                    window,
                    {
                        targetCenter.x -
                            direction * 65.0f,
                        targetCenter.y -
                            65.0f +
                            static_cast<float>(i) *
                            20.0f
                    },
                    {
                        targetCenter.x +
                            direction *
                            65.0f *
                            slashProgress,
                        targetCenter.y +
                            60.0f +
                            static_cast<float>(i) *
                            20.0f
                    },
                    9.0f,
                    withAlpha(
                        sf::Color(105, 45, 165),
                        230
                    )
                );
            }
        }

        return;
    }

    // --------------------------------------------------
    // IRONFANG
    // --------------------------------------------------

    if (visual == MoveVisual::AlloyCharge) {
        float travel =
            clamp01(progress / impactProgress);

        sf::Vector2f current =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(travel)
            );

        for (int i = 1; i <= 6; i++) {
            drawLine(
                window,
                {
                    current.x -
                        direction *
                        static_cast<float>(i) *
                        30.0f,
                    current.y +
                        static_cast<float>(i % 2) *
                        5.0f
                },
                {
                    current.x -
                        direction *
                        static_cast<float>(i - 1) *
                        20.0f,
                    current.y
                },
                7.0f,
                withAlpha(
                    sf::Color(200, 215, 230),
                    210 - i * 25
                )
            );
        }

        if (progress > impactProgress) {
            drawSparkBurst(
                window,
                targetCenter,
                70.0f,
                sf::Color(220, 230, 240),
                14
            );
        }

        return;
    }

    if (visual == MoveVisual::HornBreaker) {
        if (progress > 0.32f) {
            float strike =
                clamp01(
                    (progress - 0.32f) /
                    0.32f
                );

            sf::Vector2f tip =
                interpolatePosition(
                    {
                        targetCenter.x -
                            direction * 90.0f,
                        targetCenter.y + 45.0f
                    },
                    targetCenter,
                    strike
                );

            drawLine(
                window,
                {
                    tip.x -
                        direction * 70.0f,
                    tip.y + 35.0f
                },
                tip,
                16.0f,
                sf::Color(210, 220, 230)
            );

            drawSparkBurst(
                window,
                tip,
                50.0f * strike,
                sf::Color::White,
                8
            );
        }

        return;
    }

    if (visual == MoveVisual::SteelJab) {
        float travel =
            clamp01(progress / impactProgress);

        sf::Vector2f end =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(travel)
            );

        drawLine(
            window,
            attackerCenter,
            end,
            8.0f,
            sf::Color(205, 220, 235)
        );

        drawLine(
            window,
            attackerCenter,
            end,
            3.0f,
            sf::Color::White
        );

        if (progress > impactProgress - 0.06f) {
            drawSparkBurst(
                window,
                targetCenter,
                52.0f,
                sf::Color::White,
                10
            );
        }

        return;
    }

    if (visual == MoveVisual::Groundbreaker) {
        if (progress > 0.24f) {
            float crackProgress =
                clamp01(
                    (progress - 0.24f) /
                    0.60f
                );

            for (int i = -3; i <= 3; i++) {
                sf::Vector2f start({
                    attackerCenter.x +
                        static_cast<float>(i) *
                        15.0f,
                    405.0f
                    });

                sf::Vector2f middle({
                    start.x +
                        direction *
                        100.0f *
                        crackProgress,
                    425.0f +
                        static_cast<float>(
                            std::abs(i)
                        ) * 5.0f
                    });

                sf::Vector2f end({
                    start.x +
                        direction *
                        220.0f *
                        crackProgress,
                    410.0f
                    });

                drawLine(
                    window,
                    start,
                    middle,
                    6.0f,
                    sf::Color(95, 75, 55)
                );

                drawLine(
                    window,
                    middle,
                    end,
                    6.0f,
                    sf::Color(95, 75, 55)
                );
            }

            drawRing(
                window,
                {
                    targetCenter.x,
                    410.0f
                },
                40.0f +
                crackProgress *
                150.0f,
                8.0f,
                withAlpha(
                    sf::Color(190, 155, 100),
                    190
                )
            );
        }

        return;
    }

    // --------------------------------------------------
    // SKYWYRM
    // --------------------------------------------------

    if (visual == MoveVisual::WyrmClaw) {
        if (progress > 0.34f) {
            float slash =
                clamp01(
                    (progress - 0.34f) /
                    0.32f
                );

            drawLine(
                window,
                {
                    targetCenter.x - 75.0f,
                    targetCenter.y - 65.0f
                },
                {
                    targetCenter.x +
                        75.0f * slash,
                    targetCenter.y + 65.0f
                },
                10.0f,
                sf::Color(90, 135, 255)
            );

            drawLine(
                window,
                {
                    targetCenter.x + 75.0f,
                    targetCenter.y - 65.0f
                },
                {
                    targetCenter.x -
                        75.0f * slash,
                    targetCenter.y + 65.0f
                },
                10.0f,
                sf::Color(125, 165, 255)
            );
        }

        return;
    }

    if (visual == MoveVisual::GaleSweep) {
        float travel =
            clamp01(progress / impactProgress);

        for (int i = 0; i < 5; i++) {
            float localProgress =
                clamp01(
                    travel -
                    static_cast<float>(i) *
                    0.08f
                );

            sf::Vector2f current =
                interpolatePosition(
                    attackerCenter,
                    targetCenter,
                    smoothStep(localProgress)
                );

            drawLine(
                window,
                {
                    current.x -
                        direction * 45.0f,
                    current.y -
                        35.0f +
                        static_cast<float>(i) *
                        18.0f
                },
                {
                    current.x +
                        direction * 45.0f,
                    current.y -
                        15.0f +
                        static_cast<float>(i) *
                        18.0f
                },
                6.0f,
                withAlpha(
                    sf::Color(205, 235, 255),
                    215 - i * 28
                )
            );
        }

        return;
    }

    if (visual == MoveVisual::AzureBeam) {
        float chargeProgress =
            clamp01(progress / 0.42f);

        float chargeRadius =
            12.0f +
            chargeProgress * 35.0f;

        sf::Vector2f chargeCenter({
            attackerCenter.x +
                direction * 45.0f,
            attackerCenter.y
            });

        drawFilledCircle(
            window,
            chargeCenter,
            chargeRadius,
            withAlpha(
                sf::Color(80, 135, 255),
                220
            )
        );

        drawRing(
            window,
            chargeCenter,
            chargeRadius + 15.0f,
            7.0f,
            sf::Color(170, 205, 255)
        );

        if (progress > 0.42f) {
            float beamProgress =
                clamp01(
                    (progress - 0.42f) /
                    0.24f
                );

            sf::Vector2f beamEnd =
                interpolatePosition(
                    chargeCenter,
                    targetCenter,
                    beamProgress
                );

            drawLine(
                window,
                chargeCenter,
                beamEnd,
                34.0f +
                pulse(elapsed, 28.0f) *
                12.0f,
                withAlpha(
                    sf::Color(75, 125, 255),
                    225
                )
            );

            drawLine(
                window,
                chargeCenter,
                beamEnd,
                13.0f,
                sf::Color(220, 235, 255)
            );
        }

        if (progress > impactProgress) {
            drawSparkBurst(
                window,
                targetCenter,
                95.0f,
                sf::Color(110, 160, 255),
                16
            );
        }

        return;
    }

    // --------------------------------------------------
    // Generic fallback
    // --------------------------------------------------

    float travel =
        clamp01(progress / impactProgress);

    sf::Vector2f effectPosition =
        interpolatePosition(
            attackerCenter,
            targetCenter,
            smoothStep(travel)
        );

    drawFilledCircle(
        window,
        effectPosition,
        18.0f,
        color
    );
}

// ==================================================
// Main
// ==================================================

int main()
{
    std::srand(
        static_cast<unsigned int>(
            std::time(nullptr)
            )
    );

    sf::RenderWindow window(
        sf::VideoMode({ 1280, 720 }),
        "Battle Turn Simulator"
    );

    window.setFramerateLimit(60);

    sf::Font font;

    if (
        !font.openFromFile(
            "assets/fonts/main.ttf"
        )
        ) {
        std::cout
            << "Failed to load font: "
            << "assets/fonts/main.ttf\n";

        return -1;
    }

    Screen currentScreen =
        Screen::MainMenu;

    std::vector<std::string> battleLog;

    int difficulty = 2;

    Pokemon playerCreature =
        getVoltbit();

    Pokemon enemyCreature =
        getAquafin();

    int playerMaxHp =
        playerCreature.getHp();

    int enemyMaxHp =
        enemyCreature.getHp();

    Battle battle(
        playerCreature,
        enemyCreature,
        difficulty
    );

    // --------------------------------------------------
    // Animation state
    // --------------------------------------------------

    bool animationActive = false;
    bool playerIsAttacking = true;
    bool impactApplied = false;

    sf::Clock animationClock;

    Move activeMove{};

    MoveVisual activeVisual =
        MoveVisual::Generic;

    int activePlayerMoveIndex = -1;

    float activeMoveDuration = 0.85f;
    float activeImpactProgress = 0.58f;

    // --------------------------------------------------
    // Default creature positions
    // --------------------------------------------------

    const sf::Vector2f playerSpriteDefault({
        180.0f,
        240.0f
        });

    const sf::Vector2f playerBodyDefault({
        220.0f,
        300.0f
        });

    const sf::Vector2f playerEyeDefault({
        295.0f,
        335.0f
        });

    const sf::Vector2f enemySpriteDefault({
        855.0f,
        130.0f
        });

    const sf::Vector2f enemyBodyDefault({
        900.0f,
        190.0f
        });

    const sf::Vector2f enemyEyeDefault({
        925.0f,
        225.0f
        });

    // --------------------------------------------------
    // Textures and sprites
    // --------------------------------------------------

    sf::Texture playerTexture;
    sf::Texture enemyTexture;
    sf::Texture backgroundTexture;

    bool playerTextureLoaded = false;
    bool enemyTextureLoaded = false;

    bool backgroundTextureLoaded =
        loadPixelTexture(
            backgroundTexture,
            "assets/images/background.png"
        );

    sf::Sprite playerSprite(playerTexture);
    sf::Sprite enemySprite(enemyTexture);
    sf::Sprite backgroundSprite(backgroundTexture);

    if (backgroundTextureLoaded) {
        backgroundSprite.setPosition({
            0.0f,
            0.0f
            });

        scaleSpriteToSize(
            backgroundSprite,
            backgroundTexture,
            1280.0f,
            720.0f
        );
    }

    sf::RectangleShape background({
        1280.0f,
        720.0f
        });

    background.setFillColor(
        sf::Color(32, 36, 48)
    );

    // --------------------------------------------------
    // Main menu
    // --------------------------------------------------

    sf::Text title(
        font,
        "Battle Turn Simulator",
        42
    );

    title.setPosition({
        390.0f,
        70.0f
        });

    title.setFillColor(
        sf::Color::White
    );

    sf::RectangleShape startButton({
        340.0f,
        70.0f
        });

    startButton.setPosition({
        470.0f,
        260.0f
        });

    startButton.setFillColor(
        sf::Color(90, 90, 120)
    );

    sf::Text startText(
        font,
        "Start Battle",
        30
    );

    startText.setPosition({
        560.0f,
        278.0f
        });

    startText.setFillColor(
        sf::Color::White
    );

    sf::RectangleShape exitButton({
        340.0f,
        70.0f
        });

    exitButton.setPosition({
        470.0f,
        360.0f
        });

    exitButton.setFillColor(
        sf::Color(90, 90, 120)
    );

    sf::Text exitText(
        font,
        "Exit",
        30
    );

    exitText.setPosition({
        610.0f,
        378.0f
        });

    exitText.setFillColor(
        sf::Color::White
    );

    // --------------------------------------------------
    // Difficulty screen
    // --------------------------------------------------

    sf::Text difficultyTitle(
        font,
        "Choose Difficulty",
        38
    );

    difficultyTitle.setPosition({
        460.0f,
        80.0f
        });

    difficultyTitle.setFillColor(
        sf::Color::White
    );

    sf::RectangleShape easyButton({
        420.0f,
        70.0f
        });

    easyButton.setPosition({
        430.0f,
        220.0f
        });

    easyButton.setFillColor(
        sf::Color(90, 90, 120)
    );

    sf::Text easyText(
        font,
        "Easy - Random Enemy Moves",
        25
    );

    easyText.setPosition({
        465.0f,
        240.0f
        });

    easyText.setFillColor(
        sf::Color::White
    );

    sf::RectangleShape normalButton({
        420.0f,
        70.0f
        });

    normalButton.setPosition({
        430.0f,
        320.0f
        });

    normalButton.setFillColor(
        sf::Color(90, 90, 120)
    );

    sf::Text normalText(
        font,
        "Normal - Smart Random AI",
        25
    );

    normalText.setPosition({
        475.0f,
        340.0f
        });

    normalText.setFillColor(
        sf::Color::White
    );

    sf::RectangleShape hardButton({
        420.0f,
        70.0f
        });

    hardButton.setPosition({
        430.0f,
        420.0f
        });

    hardButton.setFillColor(
        sf::Color(90, 90, 120)
    );

    sf::Text hardText(
        font,
        "Hard - Best Move AI",
        25
    );

    hardText.setPosition({
        510.0f,
        440.0f
        });

    hardText.setFillColor(
        sf::Color::White
    );

    sf::RectangleShape backButton({
        220.0f,
        55.0f
        });

    backButton.setPosition({
        40.0f,
        40.0f
        });

    backButton.setFillColor(
        sf::Color(90, 90, 120)
    );

    sf::Text backText(
        font,
        "Back",
        24
    );

    backText.setPosition({
        115.0f,
        53.0f
        });

    backText.setFillColor(
        sf::Color::White
    );

    // --------------------------------------------------
    // Creature selection
    // --------------------------------------------------

    sf::Text selectTitle(
        font,
        "Choose Your Creature",
        38
    );

    selectTitle.setPosition({
        410.0f,
        70.0f
        });

    selectTitle.setFillColor(
        sf::Color::White
    );

    sf::Text selectHint(
        font,
        "The enemy creature will be selected randomly.",
        24
    );

    selectHint.setPosition({
        390.0f,
        610.0f
        });

    selectHint.setFillColor(
        sf::Color::White
    );

    sf::RectangleShape creatureButtons[8];

    sf::Text creatureTexts[8] = {
        sf::Text(font, "Voltbit", 24),
        sf::Text(font, "Emberling", 24),
        sf::Text(font, "Mossback", 24),
        sf::Text(font, "Aquafin", 24),
        sf::Text(font, "Wildkin", 24),
        sf::Text(font, "Shadegeist", 24),
        sf::Text(font, "Ironfang", 24),
        sf::Text(font, "Skywyrm", 24)
    };

    for (int i = 0; i < 8; i++) {
        float x =
            190.0f +
            static_cast<float>(i % 2) *
            480.0f;

        float y =
            170.0f +
            static_cast<float>(i / 2) *
            95.0f;

        creatureButtons[i].setSize({
            420.0f,
            65.0f
            });

        creatureButtons[i].setPosition({
            x,
            y
            });

        creatureButtons[i].setFillColor(
            sf::Color(90, 90, 120)
        );

        creatureTexts[i].setPosition({
            x + 25.0f,
            y + 17.0f
            });

        creatureTexts[i].setFillColor(
            sf::Color::White
        );
    }

    // --------------------------------------------------
    // Result screen
    // --------------------------------------------------

    sf::Text resultTitle(
        font,
        "Battle Result",
        42
    );

    resultTitle.setPosition({
        485.0f,
        90.0f
        });

    resultTitle.setFillColor(
        sf::Color::White
    );

    sf::Text resultMessage(
        font,
        "",
        30
    );

    resultMessage.setPosition({
        420.0f,
        210.0f
        });

    resultMessage.setFillColor(
        sf::Color::White
    );

    sf::RectangleShape playAgainButton({
        340.0f,
        70.0f
        });

    playAgainButton.setPosition({
        470.0f,
        340.0f
        });

    playAgainButton.setFillColor(
        sf::Color(90, 90, 120)
    );

    sf::Text playAgainText(
        font,
        "Play Again",
        30
    );

    playAgainText.setPosition({
        560.0f,
        358.0f
        });

    playAgainText.setFillColor(
        sf::Color::White
    );

    sf::RectangleShape resultMenuButton({
        340.0f,
        70.0f
        });

    resultMenuButton.setPosition({
        470.0f,
        440.0f
        });

    resultMenuButton.setFillColor(
        sf::Color(90, 90, 120)
    );

    sf::Text resultMenuText(
        font,
        "Main Menu",
        30
    );

    resultMenuText.setPosition({
        555.0f,
        458.0f
        });

    resultMenuText.setFillColor(
        sf::Color::White
    );

    // --------------------------------------------------
    // Battle background and fallback creatures
    // --------------------------------------------------

    sf::RectangleShape ground({
        1000.0f,
        180.0f
        });

    ground.setPosition({
        140.0f,
        400.0f
        });

    ground.setFillColor(
        sf::Color(70, 90, 70)
    );

    sf::RectangleShape playerBody({
        120.0f,
        120.0f
        });

    playerBody.setPosition(
        playerBodyDefault
    );

    sf::RectangleShape playerEye({
        20.0f,
        20.0f
        });

    playerEye.setPosition(
        playerEyeDefault
    );

    playerEye.setFillColor(
        sf::Color::Black
    );

    sf::RectangleShape enemyBody({
        120.0f,
        120.0f
        });

    enemyBody.setPosition(
        enemyBodyDefault
    );

    sf::RectangleShape enemyEye({
        20.0f,
        20.0f
        });

    enemyEye.setPosition(
        enemyEyeDefault
    );

    enemyEye.setFillColor(
        sf::Color::Black
    );

    setupCreatureVisuals(
        battle,
        playerTexture,
        enemyTexture,
        playerSprite,
        enemySprite,
        playerTextureLoaded,
        enemyTextureLoaded,
        playerBody,
        enemyBody
    );

    // --------------------------------------------------
    // HP bars
    // --------------------------------------------------

    const float hpBarMaximumWidth =
        300.0f;

    sf::RectangleShape playerHpBack({
        hpBarMaximumWidth,
        28.0f
        });

    playerHpBack.setPosition({
        120.0f,
        610.0f
        });

    playerHpBack.setFillColor(
        sf::Color(60, 60, 60)
    );

    sf::RectangleShape playerHpFill({
        hpBarMaximumWidth,
        28.0f
        });

    playerHpFill.setPosition({
        120.0f,
        610.0f
        });

    sf::RectangleShape enemyHpBack({
        hpBarMaximumWidth,
        28.0f
        });

    enemyHpBack.setPosition({
        860.0f,
        70.0f
        });

    enemyHpBack.setFillColor(
        sf::Color(60, 60, 60)
    );

    sf::RectangleShape enemyHpFill({
        hpBarMaximumWidth,
        28.0f
        });

    enemyHpFill.setPosition({
        860.0f,
        70.0f
        });

    // --------------------------------------------------
    // Move buttons
    // --------------------------------------------------

    sf::RectangleShape moveButtons[4];

    sf::Text moveTexts[4] = {
        sf::Text(font, "Move 1", 22),
        sf::Text(font, "Move 2", 22),
        sf::Text(font, "Move 3", 22),
        sf::Text(font, "Move 4", 22)
    };

    for (int i = 0; i < 4; i++) {
        moveButtons[i].setSize({
            260.0f,
            60.0f
            });

        moveButtons[i].setPosition({
            80.0f +
                static_cast<float>(i) *
                280.0f,
            650.0f
            });

        moveButtons[i].setFillColor(
            sf::Color(90, 90, 120)
        );

        moveTexts[i].setPosition({
            105.0f +
                static_cast<float>(i) *
                280.0f,
            666.0f
            });

        moveTexts[i].setFillColor(
            sf::Color::White
        );
    }

    // --------------------------------------------------
    // Battle text and status indicators
    // --------------------------------------------------

    sf::Text battleTitle(
        font,
        "Battle Turn Simulator",
        34
    );

    battleTitle.setPosition({
        430.0f,
        20.0f
        });

    battleTitle.setFillColor(
        sf::Color::White
    );

    sf::Text enemyNameText(
        font,
        "Enemy",
        24
    );

    enemyNameText.setPosition({
        860.0f,
        35.0f
        });

    enemyNameText.setFillColor(
        sf::Color::White
    );

    sf::Text enemyHpText(
        font,
        "Enemy HP",
        22
    );

    enemyHpText.setPosition({
        860.0f,
        105.0f
        });

    enemyHpText.setFillColor(
        sf::Color::White
    );

    sf::Text enemyStatusText(
        font,
        "Status: None",
        20
    );

    enemyStatusText.setPosition({
        860.0f,
        135.0f
        });

    enemyStatusText.setFillColor(
        sf::Color::White
    );

    sf::RectangleShape enemyStatusIcon({
        22.0f,
        22.0f
        });

    enemyStatusIcon.setPosition({
        1125.0f,
        138.0f
        });

    enemyStatusIcon.setFillColor(
        getStatusColor(Status::None)
    );

    sf::Text playerNameText(
        font,
        "Player",
        24
    );

    playerNameText.setPosition({
        120.0f,
        575.0f
        });

    playerNameText.setFillColor(
        sf::Color::White
    );

    sf::Text playerHpText(
        font,
        "Player HP",
        22
    );

    playerHpText.setPosition({
        120.0f,
        640.0f
        });

    playerHpText.setFillColor(
        sf::Color::White
    );

    sf::Text playerStatusText(
        font,
        "Status: None",
        20
    );

    playerStatusText.setPosition({
        440.0f,
        610.0f
        });

    playerStatusText.setFillColor(
        sf::Color::White
    );

    sf::RectangleShape playerStatusIcon({
        22.0f,
        22.0f
        });

    playerStatusIcon.setPosition({
        650.0f,
        613.0f
        });

    playerStatusIcon.setFillColor(
        getStatusColor(Status::None)
    );

    sf::Text battleMessage(
        font,
        "Choose a move.",
        24
    );

    battleMessage.setPosition({
        430.0f,
        575.0f
        });

    battleMessage.setFillColor(
        sf::Color::White
    );

    // --------------------------------------------------
    // Battle log panel
    // --------------------------------------------------

    sf::RectangleShape battleLogPanel({
        490.0f,
        145.0f
        });

    battleLogPanel.setPosition({
        395.0f,
        415.0f
        });

    battleLogPanel.setFillColor(
        sf::Color(
            20,
            24,
            34,
            205
        )
    );

    battleLogPanel.setOutlineThickness(
        2.0f
    );

    battleLogPanel.setOutlineColor(
        sf::Color(
            120,
            125,
            150,
            220
        )
    );

    sf::Text battleLogTitle(
        font,
        "Battle Log",
        20
    );

    battleLogTitle.setPosition({
        415.0f,
        424.0f
        });

    battleLogTitle.setFillColor(
        sf::Color::White
    );

    std::vector<sf::Text> battleLogTexts;

    battleLogTexts.reserve(5);

    for (int i = 0; i < 5; i++) {
        sf::Text logLine(
            font,
            "",
            16
        );

        logLine.setPosition({
            415.0f,
            452.0f +
                static_cast<float>(i) *
                20.0f
            });

        logLine.setFillColor(
            sf::Color(
                225,
                225,
                230
            )
        );

        battleLogTexts.push_back(
            logLine
        );
    }

    // ==================================================
    // Local helper lambdas
    // ==================================================

    auto resetCreatureVisualState = [&]() {
        setCreaturePosition(
            playerSprite,
            playerBody,
            playerEye,
            playerTextureLoaded,
            playerSpriteDefault,
            playerBodyDefault,
            playerEyeDefault
        );

        setCreaturePosition(
            enemySprite,
            enemyBody,
            enemyEye,
            enemyTextureLoaded,
            enemySpriteDefault,
            enemyBodyDefault,
            enemyEyeDefault
        );

        setCreatureColor(
            playerSprite,
            playerBody,
            playerTextureLoaded,
            playerTextureLoaded
            ? sf::Color::White
            : getCreatureFallbackColor(
                battle
                .getPlayerPokemon()
                .getName()
            )
        );

        setCreatureColor(
            enemySprite,
            enemyBody,
            enemyTextureLoaded,
            enemyTextureLoaded
            ? sf::Color::White
            : getCreatureFallbackColor(
                battle
                .getEnemyPokemon()
                .getName()
            )
        );
        };

    auto refreshBattleUi = [&]() {
        updateBattleText(
            battle,
            playerNameText,
            enemyNameText,
            playerHpText,
            enemyHpText,
            moveTexts[0],
            moveTexts[1],
            moveTexts[2],
            moveTexts[3],
            playerMaxHp,
            enemyMaxHp
        );

        updateHpBar(
            playerHpFill,
            battle
            .getPlayerPokemon()
            .getHp(),
            playerMaxHp,
            hpBarMaximumWidth
        );

        updateHpBar(
            enemyHpFill,
            battle
            .getEnemyPokemon()
            .getHp(),
            enemyMaxHp,
            hpBarMaximumWidth
        );

        updateStatusDisplay(
            battle,
            playerStatusText,
            enemyStatusText,
            playerStatusIcon,
            enemyStatusIcon
        );
        };

    auto showBattleResult = [&]() {
        resetCreatureVisualState();

        animationActive = false;
        impactApplied = false;
        activePlayerMoveIndex = -1;

        if (battle.didPlayerWin()) {
            resultTitle.setString(
                "You Won!"
            );

            resultMessage.setString(
                battle
                .getEnemyPokemon()
                .getName() +
                " was defeated."
            );
        }
        else {
            resultTitle.setString(
                "You Lost!"
            );

            resultMessage.setString(
                battle
                .getPlayerPokemon()
                .getName() +
                " was defeated."
            );
        }

        currentScreen =
            Screen::ResultScreen;
        };

    auto beginMoveAnimation =
        [&](
            const Move& move,
            bool isPlayerMove,
            int playerMoveIndex
            )
        {
            resetCreatureVisualState();

            activeMove = move;

            activeVisual =
                getMoveVisual(move.name);

            playerIsAttacking =
                isPlayerMove;

            activePlayerMoveIndex =
                playerMoveIndex;

            activeMoveDuration =
                getMoveDuration(activeVisual);

            activeImpactProgress =
                getMoveImpactProgress(
                    activeVisual
                );

            impactApplied = false;
            animationActive = true;

            std::string attackerName;

            if (playerIsAttacking) {
                attackerName =
                    battle
                    .getPlayerPokemon()
                    .getName();
            }
            else {
                attackerName =
                    battle
                    .getEnemyPokemon()
                    .getName();
            }

            std::string actionMessage =
                attackerName +
                " used " +
                activeMove.name +
                ".";

            battleMessage.setString(
                attackerName +
                " used " +
                activeMove.name +
                "!"
            );

            addBattleLog(
                battleLog,
                actionMessage
            );

            updateBattleLogTexts(
                battleLog,
                battleLogTexts
            );

            animationClock.restart();
        };

    auto finishRoundAfterEnemy = [&]() {
        if (!battle.isBattleOver()) {
            int playerHpBeforeStatus =
                battle
                .getPlayerPokemon()
                .getHp();

            int enemyHpBeforeStatus =
                battle
                .getEnemyPokemon()
                .getHp();

            Status playerStatusBefore =
                battle
                .getPlayerPokemon()
                .getStatus();

            Status enemyStatusBefore =
                battle
                .getEnemyPokemon()
                .getStatus();

            battle.applyEndRoundEffects();

            int playerHpAfterStatus =
                battle
                .getPlayerPokemon()
                .getHp();

            int enemyHpAfterStatus =
                battle
                .getEnemyPokemon()
                .getHp();

            Status playerStatusAfter =
                battle
                .getPlayerPokemon()
                .getStatus();

            Status enemyStatusAfter =
                battle
                .getEnemyPokemon()
                .getStatus();

            int playerStatusDamage =
                playerHpBeforeStatus -
                playerHpAfterStatus;

            int enemyStatusDamage =
                enemyHpBeforeStatus -
                enemyHpAfterStatus;

            if (playerStatusDamage > 0) {
                addBattleLog(
                    battleLog,
                    battle
                    .getPlayerPokemon()
                    .getName() +
                    " took " +
                    std::to_string(
                        playerStatusDamage
                    ) +
                    " status damage."
                );
            }

            if (enemyStatusDamage > 0) {
                addBattleLog(
                    battleLog,
                    battle
                    .getEnemyPokemon()
                    .getName() +
                    " took " +
                    std::to_string(
                        enemyStatusDamage
                    ) +
                    " status damage."
                );
            }

            if (
                playerStatusBefore != Status::None &&
                playerStatusAfter == Status::None
                ) {
                addBattleLog(
                    battleLog,
                    battle
                    .getPlayerPokemon()
                    .getName() +
                    " recovered from " +
                    getStatusConditionName(
                        playerStatusBefore
                    ) +
                    "."
                );
            }

            if (
                enemyStatusBefore != Status::None &&
                enemyStatusAfter == Status::None
                ) {
                addBattleLog(
                    battleLog,
                    battle
                    .getEnemyPokemon()
                    .getName() +
                    " recovered from " +
                    getStatusConditionName(
                        enemyStatusBefore
                    ) +
                    "."
                );
            }

            updateBattleLogTexts(
                battleLog,
                battleLogTexts
            );
        }

        refreshBattleUi();

        if (battle.isBattleOver()) {
            showBattleResult();
            return;
        }

        animationActive = false;
        impactApplied = false;
        activePlayerMoveIndex = -1;

        resetCreatureVisualState();

        battleMessage.setString(
            "Round complete. Choose another move."
        );
        };

    auto beginEnemyTurn = [&]() {
        if (battle.isBattleOver()) {
            showBattleResult();
            return;
        }

        bool enemyCanAttack =
            battle.prepareEnemyTurn();

        if (!enemyCanAttack) {
            std::string skippedMessage =
                battle
                .getEnemyPokemon()
                .getName() +
                " could not attack.";

            battleMessage.setString(
                skippedMessage
            );

            addBattleLog(
                battleLog,
                skippedMessage
            );

            updateBattleLogTexts(
                battleLog,
                battleLogTexts
            );

            finishRoundAfterEnemy();
            return;
        }

        Move enemyMove =
            battle.chooseEnemyMove();

        beginMoveAnimation(
            enemyMove,
            false,
            -1
        );
        };

    refreshBattleUi();

    // ==================================================
    // Main loop
    // ==================================================

    while (window.isOpen()) {
        sf::Vector2i mousePixelPosition =
            sf::Mouse::getPosition(window);

        sf::Vector2f mousePosition =
            window.mapPixelToCoords(
                mousePixelPosition
            );

        // --------------------------------------------------
        // Hover states
        // --------------------------------------------------

        if (
            currentScreen ==
            Screen::MainMenu
            ) {
            updateButtonHover(
                startButton,
                mousePosition
            );

            updateButtonHover(
                exitButton,
                mousePosition
            );
        }

        if (
            currentScreen ==
            Screen::DifficultySelect
            ) {
            updateButtonHover(
                easyButton,
                mousePosition
            );

            updateButtonHover(
                normalButton,
                mousePosition
            );

            updateButtonHover(
                hardButton,
                mousePosition
            );

            updateButtonHover(
                backButton,
                mousePosition
            );
        }

        if (
            currentScreen ==
            Screen::CreatureSelect
            ) {
            for (int i = 0; i < 8; i++) {
                updateButtonHover(
                    creatureButtons[i],
                    mousePosition
                );
            }
        }

        if (
            currentScreen ==
            Screen::BattleScreen
            ) {
            bool moveButtonsEnabled =
                !animationActive &&
                !battle.isBattleOver();

            for (int i = 0; i < 4; i++) {
                updateButtonHover(
                    moveButtons[i],
                    mousePosition,
                    moveButtonsEnabled
                );
            }
        }

        if (
            currentScreen ==
            Screen::ResultScreen
            ) {
            updateButtonHover(
                playAgainButton,
                mousePosition
            );

            updateButtonHover(
                resultMenuButton,
                mousePosition
            );
        }

        // --------------------------------------------------
        // Events
        // --------------------------------------------------

        while (
            const std::optional<sf::Event> event =
            window.pollEvent()
            ) {
            if (
                event->is<
                sf::Event::Closed
                >()
                ) {
                window.close();
            }

            const auto* mouseButton =
                event->getIf<
                sf::Event::MouseButtonPressed
                >();

            if (
                mouseButton == nullptr ||
                mouseButton->button !=
                sf::Mouse::Button::Left
                ) {
                continue;
            }

            sf::Vector2f clickPosition =
                window.mapPixelToCoords(
                    mouseButton->position
                );

            // Main menu
            if (
                currentScreen ==
                Screen::MainMenu
                ) {
                if (
                    isButtonClicked(
                        startButton,
                        clickPosition
                    )
                    ) {
                    currentScreen =
                        Screen::DifficultySelect;
                }
                else if (
                    isButtonClicked(
                        exitButton,
                        clickPosition
                    )
                    ) {
                    window.close();
                }
            }

            // Difficulty
            else if (
                currentScreen ==
                Screen::DifficultySelect
                ) {
                if (
                    isButtonClicked(
                        easyButton,
                        clickPosition
                    )
                    ) {
                    difficulty = 1;

                    currentScreen =
                        Screen::CreatureSelect;
                }
                else if (
                    isButtonClicked(
                        normalButton,
                        clickPosition
                    )
                    ) {
                    difficulty = 2;

                    currentScreen =
                        Screen::CreatureSelect;
                }
                else if (
                    isButtonClicked(
                        hardButton,
                        clickPosition
                    )
                    ) {
                    difficulty = 3;

                    currentScreen =
                        Screen::CreatureSelect;
                }
                else if (
                    isButtonClicked(
                        backButton,
                        clickPosition
                    )
                    ) {
                    currentScreen =
                        Screen::MainMenu;
                }
            }

            // Creature selection
            else if (
                currentScreen ==
                Screen::CreatureSelect
                ) {
                for (int i = 0; i < 8; i++) {
                    if (
                        !isButtonClicked(
                            creatureButtons[i],
                            clickPosition
                        )
                        ) {
                        continue;
                    }

                    int playerChoice =
                        i + 1;

                    int enemyChoice =
                        std::rand() % 8 + 1;

                    while (
                        enemyChoice ==
                        playerChoice
                        ) {
                        enemyChoice =
                            std::rand() % 8 + 1;
                    }

                    playerCreature =
                        getCreatureByChoice(
                            playerChoice
                        );

                    enemyCreature =
                        getCreatureByChoice(
                            enemyChoice
                        );

                    playerMaxHp =
                        playerCreature.getHp();

                    enemyMaxHp =
                        enemyCreature.getHp();

                    battle = Battle(
                        playerCreature,
                        enemyCreature,
                        difficulty
                    );

                    battleLog.clear();

                    addBattleLog(
                        battleLog,
                        "Battle started: " +
                        battle
                        .getPlayerPokemon()
                        .getName() +
                        " vs " +
                        battle
                        .getEnemyPokemon()
                        .getName() +
                        "."
                    );

                    updateBattleLogTexts(
                        battleLog,
                        battleLogTexts
                    );

                    setupCreatureVisuals(
                        battle,
                        playerTexture,
                        enemyTexture,
                        playerSprite,
                        enemySprite,
                        playerTextureLoaded,
                        enemyTextureLoaded,
                        playerBody,
                        enemyBody
                    );

                    animationActive = false;
                    impactApplied = false;
                    activePlayerMoveIndex = -1;

                    resetCreatureVisualState();
                    refreshBattleUi();

                    battleMessage.setString(
                        "Choose a move."
                    );

                    currentScreen =
                        Screen::BattleScreen;

                    break;
                }
            }

            // Battle move selection
            else if (
                currentScreen ==
                Screen::BattleScreen &&
                !animationActive &&
                !battle.isBattleOver()
                ) {
                int selectedMoveIndex = -1;

                for (int i = 0; i < 4; i++) {
                    if (
                        isButtonClicked(
                            moveButtons[i],
                            clickPosition
                        )
                        ) {
                        selectedMoveIndex = i;
                        break;
                    }
                }

                if (
                    selectedMoveIndex >= 0 &&
                    selectedMoveIndex <
                    battle
                    .getPlayerPokemon()
                    .getMoveCount()
                    ) {
                    Move selectedMove =
                        battle
                        .getPlayerPokemon()
                        .getMove(
                            selectedMoveIndex
                        );

                    beginMoveAnimation(
                        selectedMove,
                        true,
                        selectedMoveIndex
                    );
                }
            }

            // Result screen
            else if (
                currentScreen ==
                Screen::ResultScreen
                ) {
                if (
                    isButtonClicked(
                        playAgainButton,
                        clickPosition
                    )
                    ) {
                    currentScreen =
                        Screen::DifficultySelect;
                }
                else if (
                    isButtonClicked(
                        resultMenuButton,
                        clickPosition
                    )
                    ) {
                    currentScreen =
                        Screen::MainMenu;
                }
            }
        }

        // ==================================================
        // Animation update
        // ==================================================

        if (
            currentScreen ==
            Screen::BattleScreen &&
            animationActive
            ) {
            float elapsed =
                animationClock
                .getElapsedTime()
                .asSeconds();

            float progress =
                clamp01(
                    elapsed /
                    activeMoveDuration
                );

            resetCreatureVisualState();

            float direction =
                playerIsAttacking
                ? 1.0f
                : -1.0f;

            float attackerTravelProgress =
                progress <
                activeImpactProgress
                ? smoothStep(
                    progress /
                    activeImpactProgress
                )
                : smoothStep(
                    1.0f -
                    (
                        progress -
                        activeImpactProgress
                        ) /
                    (
                        1.0f -
                        activeImpactProgress
                        )
                );

            attackerTravelProgress =
                clamp01(
                    attackerTravelProgress
                );

            float targetShake = 0.0f;

            if (
                progress >=
                activeImpactProgress
                ) {
                targetShake =
                    (
                        static_cast<int>(
                            elapsed * 55.0f
                            ) % 2 == 0
                        )
                    ? -9.0f
                    : 9.0f;
            }

            bool strongLunge =
                activeVisual ==
                MoveVisual::ArcPounce ||
                activeVisual ==
                MoveVisual::Flashstep ||
                activeVisual ==
                MoveVisual::CinderClaw ||
                activeVisual ==
                MoveVisual::RiptideRush ||
                activeVisual ==
                MoveVisual::PhantomDive ||
                activeVisual ==
                MoveVisual::AlloyCharge ||
                activeVisual ==
                MoveVisual::HornBreaker ||
                activeVisual ==
                MoveVisual::SteelJab;

            bool mediumLunge =
                activeVisual ==
                MoveVisual::TailSweep ||
                activeVisual ==
                MoveVisual::RazorFin ||
                activeVisual ==
                MoveVisual::ShadowTalon ||
                activeVisual ==
                MoveVisual::WyrmClaw;

            float movementDistance = 0.0f;

            if (strongLunge) {
                movementDistance = 160.0f;
            }
            else if (mediumLunge) {
                movementDistance = 85.0f;
            }

            if (
                activeVisual ==
                MoveVisual::BurrowRush
                ) {
                float sinkAmount = 0.0f;

                if (progress < 0.25f) {
                    sinkAmount =
                        progress /
                        0.25f;
                }
                else if (
                    progress >
                    activeImpactProgress
                    ) {
                    sinkAmount =
                        1.0f -
                        (
                            progress -
                            activeImpactProgress
                            ) /
                        (
                            1.0f -
                            activeImpactProgress
                            );
                }
                else {
                    sinkAmount = 1.0f;
                }

                sinkAmount =
                    clamp01(sinkAmount);

                if (playerIsAttacking) {
                    setCreaturePosition(
                        playerSprite,
                        playerBody,
                        playerEye,
                        playerTextureLoaded,
                        {
                            playerSpriteDefault.x +
                                direction *
                                480.0f *
                                attackerTravelProgress,
                            playerSpriteDefault.y +
                                120.0f *
                                sinkAmount
                        },
                        {
                            playerBodyDefault.x +
                                direction *
                                480.0f *
                                attackerTravelProgress,
                            playerBodyDefault.y +
                                120.0f *
                                sinkAmount
                        },
                        {
                            playerEyeDefault.x +
                                direction *
                                480.0f *
                                attackerTravelProgress,
                            playerEyeDefault.y +
                                120.0f *
                                sinkAmount
                        }
                    );
                }
                else {
                    setCreaturePosition(
                        enemySprite,
                        enemyBody,
                        enemyEye,
                        enemyTextureLoaded,
                        {
                            enemySpriteDefault.x +
                                direction *
                                480.0f *
                                attackerTravelProgress,
                            enemySpriteDefault.y +
                                120.0f *
                                sinkAmount
                        },
                        {
                            enemyBodyDefault.x +
                                direction *
                                480.0f *
                                attackerTravelProgress,
                            enemyBodyDefault.y +
                                120.0f *
                                sinkAmount
                        },
                        {
                            enemyEyeDefault.x +
                                direction *
                                480.0f *
                                attackerTravelProgress,
                            enemyEyeDefault.y +
                                120.0f *
                                sinkAmount
                        }
                    );
                }
            }
            else if (
                movementDistance > 0.0f
                ) {
                float verticalAmount =
                    (
                        activeVisual ==
                        MoveVisual::ArcPounce ||
                        activeVisual ==
                        MoveVisual::RiptideRush
                        )
                    ? -30.0f
                    : 0.0f;

                if (playerIsAttacking) {
                    setCreaturePosition(
                        playerSprite,
                        playerBody,
                        playerEye,
                        playerTextureLoaded,
                        {
                            playerSpriteDefault.x +
                                direction *
                                movementDistance *
                                attackerTravelProgress,
                            playerSpriteDefault.y +
                                verticalAmount *
                                attackerTravelProgress
                        },
                        {
                            playerBodyDefault.x +
                                direction *
                                movementDistance *
                                attackerTravelProgress,
                            playerBodyDefault.y +
                                verticalAmount *
                                attackerTravelProgress
                        },
                        {
                            playerEyeDefault.x +
                                direction *
                                movementDistance *
                                attackerTravelProgress,
                            playerEyeDefault.y +
                                verticalAmount *
                                attackerTravelProgress
                        }
                    );
                }
                else {
                    setCreaturePosition(
                        enemySprite,
                        enemyBody,
                        enemyEye,
                        enemyTextureLoaded,
                        {
                            enemySpriteDefault.x +
                                direction *
                                movementDistance *
                                attackerTravelProgress,
                            enemySpriteDefault.y +
                                verticalAmount *
                                attackerTravelProgress
                        },
                        {
                            enemyBodyDefault.x +
                                direction *
                                movementDistance *
                                attackerTravelProgress,
                            enemyBodyDefault.y +
                                verticalAmount *
                                attackerTravelProgress
                        },
                        {
                            enemyEyeDefault.x +
                                direction *
                                movementDistance *
                                attackerTravelProgress,
                            enemyEyeDefault.y +
                                verticalAmount *
                                attackerTravelProgress
                        }
                    );
                }
            }

            if (
                activeVisual ==
                MoveVisual::PhantomDive
                ) {
                int alpha =
                    95 +
                    static_cast<int>(
                        160.0f *
                        std::abs(
                            progress - 0.5f
                        ) *
                        2.0f
                        );

                if (playerIsAttacking) {
                    setCreatureColor(
                        playerSprite,
                        playerBody,
                        playerTextureLoaded,
                        withAlpha(
                            sf::Color(
                                175,
                                110,
                                235
                            ),
                            alpha
                        )
                    );
                }
                else {
                    setCreatureColor(
                        enemySprite,
                        enemyBody,
                        enemyTextureLoaded,
                        withAlpha(
                            sf::Color(
                                175,
                                110,
                                235
                            ),
                            alpha
                        )
                    );
                }
            }

            if (
                activeVisual ==
                MoveVisual::Flashstep &&
                progress <
                activeImpactProgress
                ) {
                int alpha =
                    90 +
                    static_cast<int>(
                        pulse(
                            elapsed,
                            28.0f
                        ) * 90.0f
                        );

                if (playerIsAttacking) {
                    setCreatureColor(
                        playerSprite,
                        playerBody,
                        playerTextureLoaded,
                        withAlpha(
                            sf::Color::White,
                            alpha
                        )
                    );
                }
                else {
                    setCreatureColor(
                        enemySprite,
                        enemyBody,
                        enemyTextureLoaded,
                        withAlpha(
                            sf::Color::White,
                            alpha
                        )
                    );
                }
            }

            bool targetShouldShake =
                progress >=
                activeImpactProgress &&
                activeMove.damage > 0;

            if (targetShouldShake) {
                if (playerIsAttacking) {
                    setCreaturePosition(
                        enemySprite,
                        enemyBody,
                        enemyEye,
                        enemyTextureLoaded,
                        {
                            enemySpriteDefault.x +
                                targetShake,
                            enemySpriteDefault.y
                        },
                        {
                            enemyBodyDefault.x +
                                targetShake,
                            enemyBodyDefault.y
                        },
                        {
                            enemyEyeDefault.x +
                                targetShake,
                            enemyEyeDefault.y
                        }
                    );
                }
                else {
                    setCreaturePosition(
                        playerSprite,
                        playerBody,
                        playerEye,
                        playerTextureLoaded,
                        {
                            playerSpriteDefault.x +
                                targetShake,
                            playerSpriteDefault.y
                        },
                        {
                            playerBodyDefault.x +
                                targetShake,
                            playerBodyDefault.y
                        },
                        {
                            playerEyeDefault.x +
                                targetShake,
                            playerEyeDefault.y
                        }
                    );
                }
            }

            if (
                progress >=
                activeImpactProgress
                ) {
                sf::Color targetColor =
                    getMoveEffectColor(
                        activeMove.type
                    );

                if (
                    activeVisual ==
                    MoveVisual::ArcPounce ||
                    activeVisual ==
                    MoveVisual::FlameSpit
                    ) {
                    targetColor =
                        sf::Color(
                            115,
                            65,
                            35
                        );
                }
                else if (
                    activeMove.type ==
                    Type::Electric
                    ) {
                    targetColor =
                        sf::Color(
                            255,
                            245,
                            120
                        );
                }
                else if (
                    activeMove.type ==
                    Type::Water
                    ) {
                    targetColor =
                        sf::Color(
                            115,
                            205,
                            255
                        );
                }
                else if (
                    activeMove.type ==
                    Type::Ghost ||
                    activeMove.type ==
                    Type::Dark
                    ) {
                    targetColor =
                        sf::Color(
                            145,
                            85,
                            180
                        );
                }

                if (playerIsAttacking) {
                    setCreatureColor(
                        enemySprite,
                        enemyBody,
                        enemyTextureLoaded,
                        targetColor
                    );
                }
                else {
                    setCreatureColor(
                        playerSprite,
                        playerBody,
                        playerTextureLoaded,
                        targetColor
                    );
                }
            }

            // ------------------------------------------
            // Apply actual battle logic at impact
            // ------------------------------------------

            if (
                !impactApplied &&
                progress >=
                activeImpactProgress
                ) {
                Pokemon& target =
                    playerIsAttacking
                    ? battle.getEnemyPokemon()
                    : battle.getPlayerPokemon();

                std::string targetName =
                    target.getName();

                int targetHpBefore =
                    target.getHp();

                Status targetStatusBefore =
                    target.getStatus();

                bool attackWasSkipped = false;

                if (playerIsAttacking) {
                    int skippedTurnsBefore =
                        battle
                        .getStats()
                        .playerTurnsSkipped;

                    battle.playerAttack(
                        activePlayerMoveIndex
                    );

                    int skippedTurnsAfter =
                        battle
                        .getStats()
                        .playerTurnsSkipped;

                    attackWasSkipped =
                        skippedTurnsAfter >
                        skippedTurnsBefore;
                }
                else {
                    battle.executeEnemyMove(
                        activeMove
                    );
                }

                int targetHpAfter =
                    target.getHp();

                Status targetStatusAfter =
                    target.getStatus();

                if (attackWasSkipped) {
                    addBattleLog(
                        battleLog,
                        battle
                        .getPlayerPokemon()
                        .getName() +
                        " could not attack."
                    );
                }
                else {
                    addMoveResultToBattleLog(
                        battleLog,
                        targetName,
                        targetHpBefore,
                        targetHpAfter,
                        targetStatusBefore,
                        targetStatusAfter,
                        activeMove
                    );
                }

                updateBattleLogTexts(
                    battleLog,
                    battleLogTexts
                );

                impactApplied = true;

                refreshBattleUi();
            }

            // ------------------------------------------
            // Finish animation
            // ------------------------------------------

            if (
                elapsed >=
                activeMoveDuration
                ) {
                resetCreatureVisualState();

                animationActive = false;

                if (battle.isBattleOver()) {
                    showBattleResult();
                }
                else if (playerIsAttacking) {
                    beginEnemyTurn();
                }
                else {
                    finishRoundAfterEnemy();
                }
            }
        }

        // ==================================================
        // Drawing
        // ==================================================

        window.clear();

        if (backgroundTextureLoaded) {
            window.draw(
                backgroundSprite
            );
        }
        else {
            window.draw(background);
        }

        if (
            currentScreen ==
            Screen::MainMenu
            ) {
            window.draw(title);

            window.draw(startButton);
            window.draw(exitButton);

            window.draw(startText);
            window.draw(exitText);
        }

        else if (
            currentScreen ==
            Screen::DifficultySelect
            ) {
            window.draw(
                difficultyTitle
            );

            window.draw(easyButton);
            window.draw(normalButton);
            window.draw(hardButton);
            window.draw(backButton);

            window.draw(easyText);
            window.draw(normalText);
            window.draw(hardText);
            window.draw(backText);
        }

        else if (
            currentScreen ==
            Screen::CreatureSelect
            ) {
            window.draw(selectTitle);
            window.draw(selectHint);

            for (int i = 0; i < 8; i++) {
                window.draw(
                    creatureButtons[i]
                );

                window.draw(
                    creatureTexts[i]
                );
            }
        }

        else if (
            currentScreen ==
            Screen::BattleScreen
            ) {
            window.draw(ground);

            if (
                animationActive &&
                (
                    activeVisual ==
                    MoveVisual::Groundbreaker ||
                    activeVisual ==
                    MoveVisual::TremorStomp ||
                    activeVisual ==
                    MoveVisual::BurrowRush
                    )
                ) {
                drawMoveEffects(
                    window,
                    activeVisual,
                    activeMove.type,
                    playerIsAttacking,
                    animationClock
                    .getElapsedTime()
                    .asSeconds(),
                    activeMoveDuration
                );
            }

            if (playerTextureLoaded) {
                window.draw(playerSprite);
            }
            else {
                window.draw(playerBody);
                window.draw(playerEye);
            }

            if (enemyTextureLoaded) {
                window.draw(enemySprite);
            }
            else {
                window.draw(enemyBody);
                window.draw(enemyEye);
            }

            if (
                animationActive &&
                activeVisual !=
                MoveVisual::Groundbreaker &&
                activeVisual !=
                MoveVisual::TremorStomp &&
                activeVisual !=
                MoveVisual::BurrowRush
                ) {
                drawMoveEffects(
                    window,
                    activeVisual,
                    activeMove.type,
                    playerIsAttacking,
                    animationClock
                    .getElapsedTime()
                    .asSeconds(),
                    activeMoveDuration
                );
            }

            window.draw(playerHpBack);
            window.draw(playerHpFill);

            window.draw(enemyHpBack);
            window.draw(enemyHpFill);

            for (int i = 0; i < 4; i++) {
                window.draw(moveButtons[i]);
                window.draw(moveTexts[i]);
            }

            window.draw(battleTitle);

            window.draw(enemyNameText);
            window.draw(enemyHpText);
            window.draw(enemyStatusText);
            window.draw(enemyStatusIcon);

            window.draw(playerNameText);
            window.draw(playerHpText);
            window.draw(playerStatusText);
            window.draw(playerStatusIcon);

            window.draw(battleLogPanel);
            window.draw(battleLogTitle);

            for (
                std::size_t i = 0;
                i < battleLogTexts.size();
                i++
                ) {
                window.draw(
                    battleLogTexts[i]
                );
            }

            window.draw(battleMessage);
        }

        else if (
            currentScreen ==
            Screen::ResultScreen
            ) {
            window.draw(resultTitle);
            window.draw(resultMessage);

            window.draw(playAgainButton);
            window.draw(resultMenuButton);

            window.draw(playAgainText);
            window.draw(resultMenuText);
        }

        window.display();
    }

    return 0;
}