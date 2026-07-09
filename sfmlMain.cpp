#include <SFML/Graphics.hpp>

#include <optional>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <cmath>

#include "pokemon.h"
#include "pokemonDatabase.h"
#include "battle.h"

enum class Screen {
    MainMenu,
    DifficultySelect,
    CreatureSelect,
    BattleScreen,
    ResultScreen
};

enum class AnimationState {
    None,
    PlayerLunge,
    PlayerProjectile,
    PlayerBeam,
    PlayerStatus,
    PlayerImpact,
    EnemyFlash,
    EnemyLunge,
    PlayerFlash
};

// --------------------------------------------------
// Button helpers
// --------------------------------------------------

bool isButtonClicked(
    const sf::RectangleShape& button,
    sf::Vector2f mousePosition
) {
    return button
        .getGlobalBounds()
        .contains(mousePosition);
}

void updateButtonHover(
    sf::RectangleShape& button,
    sf::Vector2f mousePosition
) {
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

// --------------------------------------------------
// Creature creation
// --------------------------------------------------

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

// --------------------------------------------------
// HP helpers
// --------------------------------------------------

void updateHpBar(
    sf::RectangleShape& hpFill,
    int currentHp,
    int maxHp,
    float maxWidth
) {
    if (currentHp < 0) {
        currentHp = 0;
    }

    if (maxHp <= 0) {
        hpFill.setSize({
            0.0f,
            hpFill.getSize().y
            });

        return;
    }

    float hpPercent =
        static_cast<float>(currentHp) /
        static_cast<float>(maxHp);

    if (hpPercent < 0.0f) {
        hpPercent = 0.0f;
    }

    if (hpPercent > 1.0f) {
        hpPercent = 1.0f;
    }

    hpFill.setSize({
        maxWidth * hpPercent,
        hpFill.getSize().y
        });

    if (hpPercent > 0.5f) {
        hpFill.setFillColor(
            sf::Color(80, 220, 100)
        );
    }
    else if (hpPercent > 0.25f) {
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
) {
    return label +
        " HP: " +
        std::to_string(creature.getHp()) +
        " / " +
        std::to_string(maxHp);
}

std::string getMoveNameOrEmpty(
    Pokemon& creature,
    int index
) {
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
) {
    playerNameText.setString(
        "Player: " +
        battle
        .getPlayerPokemon()
        .getName()
    );

    enemyNameText.setString(
        "Enemy: " +
        battle
        .getEnemyPokemon()
        .getName()
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

// --------------------------------------------------
// Sprite helpers
// --------------------------------------------------

bool loadPixelTexture(
    sf::Texture& texture,
    const std::string& path
) {
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
) {
    sf::Vector2u size =
        texture.getSize();

    if (size.x == 0 || size.y == 0) {
        return;
    }

    sprite.setScale({
        targetWidth /
            static_cast<float>(size.x),

        targetHeight /
            static_cast<float>(size.y)
        });
}

std::string getCreatureImagePath(
    const std::string& creatureName,
    bool useBackSprite
) {
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
    const std::string& name
) {
    if (name == "Voltbit") {
        return sf::Color(240, 210, 70);
    }

    if (name == "Emberling") {
        return sf::Color(230, 95, 55);
    }

    if (name == "Mossback") {
        return sf::Color(90, 180, 90);
    }

    if (name == "Aquafin") {
        return sf::Color(80, 170, 240);
    }

    if (name == "Wildkin") {
        return sf::Color(170, 120, 75);
    }

    if (name == "Shadegeist") {
        return sf::Color(120, 80, 170);
    }

    if (name == "Ironfang") {
        return sf::Color(70, 100, 180);
    }

    if (name == "Skywyrm") {
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
) {
    std::string playerName =
        battle
        .getPlayerPokemon()
        .getName();

    std::string enemyName =
        battle
        .getEnemyPokemon()
        .getName();

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
        getCreatureFallbackColor(
            playerName
        )
    );

    enemyBody.setFillColor(
        getCreatureFallbackColor(
            enemyName
        )
    );

    playerSprite.setColor(
        sf::Color::White
    );

    enemySprite.setColor(
        sf::Color::White
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
) {
    if (textureLoaded) {
        sprite.setPosition(
            spritePosition
        );
    }
    else {
        body.setPosition(
            bodyPosition
        );

        eye.setPosition(
            eyePosition
        );
    }
}

void setCreatureColor(
    sf::Sprite& sprite,
    sf::RectangleShape& body,
    bool textureLoaded,
    sf::Color color
) {
    if (textureLoaded) {
        sprite.setColor(color);
    }
    else {
        body.setFillColor(color);
    }
}

sf::Vector2f interpolatePosition(
    sf::Vector2f start,
    sf::Vector2f end,
    float progress
) {
    return {
        start.x +
            (end.x - start.x) *
            progress,

        start.y +
            (end.y - start.y) *
            progress
    };
}

sf::Color getMoveEffectColor(
    Type type
) {
    if (type == Type::Fire) {
        return sf::Color(255, 90, 40);
    }

    if (type == Type::Water) {
        return sf::Color(70, 170, 255);
    }

    if (type == Type::Grass) {
        return sf::Color(80, 220, 100);
    }

    if (type == Type::Electric) {
        return sf::Color(255, 235, 60);
    }

    if (type == Type::Ghost) {
        return sf::Color(150, 80, 220);
    }

    if (type == Type::Dark) {
        return sf::Color(80, 50, 120);
    }

    if (type == Type::Steel) {
        return sf::Color(180, 200, 220);
    }

    if (type == Type::Fighting) {
        return sf::Color(210, 80, 60);
    }

    if (type == Type::Dragon) {
        return sf::Color(80, 120, 255);
    }

    if (type == Type::Flying) {
        return sf::Color(170, 220, 255);
    }

    if (type == Type::Poison) {
        return sf::Color(170, 60, 200);
    }

    return sf::Color(230, 230, 230);
}

AnimationState getPlayerAnimationState(
    MoveAnimation animation
) {
    if (animation == MoveAnimation::Projectile) {
        return AnimationState::PlayerProjectile;
    }

    if (animation == MoveAnimation::Beam) {
        return AnimationState::PlayerBeam;
    }

    if (animation == MoveAnimation::StatusPulse) {
        return AnimationState::PlayerStatus;
    }

    if (animation == MoveAnimation::Impact) {
        return AnimationState::PlayerImpact;
    }

    return AnimationState::PlayerLunge;
}

// --------------------------------------------------
// Main
// --------------------------------------------------

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
            << "Failed to load font.\n";

        return -1;
    }

    Screen currentScreen =
        Screen::MainMenu;

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

    AnimationState animationState =
        AnimationState::None;

    sf::Clock animationClock;

    int pendingMoveIndex = -1;

    std::string pendingMoveName;

    sf::Color pendingEffectColor =
        sf::Color::White;

    const sf::Vector2f
        playerSpriteDefault(
            180.0f,
            240.0f
        );

    const sf::Vector2f
        playerBodyDefault(
            220.0f,
            300.0f
        );

    const sf::Vector2f
        playerEyeDefault(
            295.0f,
            335.0f
        );

    const sf::Vector2f
        enemySpriteDefault(
            855.0f,
            130.0f
        );

    const sf::Vector2f
        enemyBodyDefault(
            900.0f,
            190.0f
        );

    const sf::Vector2f
        enemyEyeDefault(
            925.0f,
            225.0f
        );

    // --------------------------------------------------
    // Textures
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

    sf::Sprite playerSprite(
        playerTexture
    );

    sf::Sprite enemySprite(
        enemyTexture
    );

    sf::Sprite backgroundSprite(
        backgroundTexture
    );

    if (backgroundTextureLoaded) {
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

    sf::RectangleShape startButton({
        340.0f,
        70.0f
        });

    startButton.setPosition({
        470.0f,
        260.0f
        });

    sf::Text startText(
        font,
        "Start Battle",
        30
    );

    startText.setPosition({
        560.0f,
        278.0f
        });

    sf::RectangleShape exitButton({
        340.0f,
        70.0f
        });

    exitButton.setPosition({
        470.0f,
        360.0f
        });

    sf::Text exitText(
        font,
        "Exit",
        30
    );

    exitText.setPosition({
        610.0f,
        378.0f
        });

    // --------------------------------------------------
    // Difficulty
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

    sf::RectangleShape easyButton({
        420.0f,
        70.0f
        });

    easyButton.setPosition({
        430.0f,
        220.0f
        });

    sf::Text easyText(
        font,
        "Easy - Random Enemy Moves",
        25
    );

    easyText.setPosition({
        465.0f,
        240.0f
        });

    sf::RectangleShape normalButton({
        420.0f,
        70.0f
        });

    normalButton.setPosition({
        430.0f,
        320.0f
        });

    sf::Text normalText(
        font,
        "Normal - Smart Random AI",
        25
    );

    normalText.setPosition({
        475.0f,
        340.0f
        });

    sf::RectangleShape hardButton({
        420.0f,
        70.0f
        });

    hardButton.setPosition({
        430.0f,
        420.0f
        });

    sf::Text hardText(
        font,
        "Hard - Best Move AI",
        25
    );

    hardText.setPosition({
        510.0f,
        440.0f
        });

    sf::RectangleShape backButton({
        220.0f,
        55.0f
        });

    backButton.setPosition({
        40.0f,
        40.0f
        });

    sf::Text backText(
        font,
        "Back",
        24
    );

    backText.setPosition({
        115.0f,
        53.0f
        });

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

    sf::Text selectHint(
        font,
        "The enemy creature will be selected randomly.",
        24
    );

    selectHint.setPosition({
        390.0f,
        610.0f
        });

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

        creatureTexts[i].setPosition({
            x + 25.0f,
            y + 17.0f
            });
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

    sf::Text resultMessage(
        font,
        "",
        30
    );

    resultMessage.setPosition({
        420.0f,
        210.0f
        });

    sf::RectangleShape playAgainButton({
        340.0f,
        70.0f
        });

    playAgainButton.setPosition({
        470.0f,
        340.0f
        });

    sf::Text playAgainText(
        font,
        "Play Again",
        30
    );

    playAgainText.setPosition({
        560.0f,
        358.0f
        });

    sf::RectangleShape resultMenuButton({
        340.0f,
        70.0f
        });

    resultMenuButton.setPosition({
        470.0f,
        440.0f
        });

    sf::Text resultMenuText(
        font,
        "Main Menu",
        30
    );

    resultMenuText.setPosition({
        555.0f,
        458.0f
        });

    // --------------------------------------------------
    // Battle visuals
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

    const float hpBarMaxWidth =
        300.0f;

    sf::RectangleShape playerHpBack({
        hpBarMaxWidth,
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
        hpBarMaxWidth,
        28.0f
        });

    playerHpFill.setPosition({
        120.0f,
        610.0f
        });

    sf::RectangleShape enemyHpBack({
        hpBarMaxWidth,
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
        hpBarMaxWidth,
        28.0f
        });

    enemyHpFill.setPosition({
        860.0f,
        70.0f
        });

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

        moveTexts[i].setPosition({
            105.0f +
                static_cast<float>(i) *
                280.0f,

            666.0f
            });
    }

    sf::Text battleTitle(
        font,
        "Battle Turn Simulator",
        34
    );

    battleTitle.setPosition({
        430.0f,
        20.0f
        });

    sf::Text enemyNameText(
        font,
        "Enemy",
        24
    );

    enemyNameText.setPosition({
        860.0f,
        35.0f
        });

    sf::Text enemyHpText(
        font,
        "Enemy HP",
        22
    );

    enemyHpText.setPosition({
        860.0f,
        105.0f
        });

    sf::Text playerNameText(
        font,
        "Player",
        24
    );

    playerNameText.setPosition({
        120.0f,
        575.0f
        });

    sf::Text playerHpText(
        font,
        "Player HP",
        22
    );

    playerHpText.setPosition({
        120.0f,
        640.0f
        });

    sf::Text battleMessage(
        font,
        "Choose a move.",
        26
    );

    battleMessage.setPosition({
        455.0f,
        585.0f
        });

    // --------------------------------------------------
    // Effects
    // --------------------------------------------------

    sf::CircleShape projectileEffect(
        14.0f
    );

    projectileEffect.setOrigin({
        14.0f,
        14.0f
        });

    sf::RectangleShape beamEffect({
        510.0f,
        18.0f
        });

    beamEffect.setPosition({
        370.0f,
        270.0f
        });

    sf::CircleShape statusPulse(
        40.0f
    );

    statusPulse.setOrigin({
        40.0f,
        40.0f
        });

    statusPulse.setPosition({
        950.0f,
        230.0f
        });

    statusPulse.setFillColor(
        sf::Color::Transparent
    );

    statusPulse.setOutlineThickness(
        7.0f
    );

    auto updateAllBattleUi = [&]() {
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
            hpBarMaxWidth
        );

        updateHpBar(
            enemyHpFill,
            battle
            .getEnemyPokemon()
            .getHp(),
            enemyMaxHp,
            hpBarMaxWidth
        );
        };

    auto showBattleResult = [&]() {
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

        animationState =
            AnimationState::None;

        currentScreen =
            Screen::ResultScreen;
        };

    updateAllBattleUi();

    // --------------------------------------------------
    // Main loop
    // --------------------------------------------------

    while (window.isOpen()) {
        sf::Vector2i mousePixel =
            sf::Mouse::getPosition(window);

        sf::Vector2f mousePosition =
            window.mapPixelToCoords(
                mousePixel
            );

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
            Screen::BattleScreen &&
            !battle.isBattleOver() &&
            animationState ==
            AnimationState::None
            ) {
            for (int i = 0; i < 4; i++) {
                updateButtonHover(
                    moveButtons[i],
                    mousePosition
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

            if (
                const auto* mouseButton =
                event->getIf<
                sf::Event::
                MouseButtonPressed
                >()
                ) {
                if (
                    mouseButton->button ==
                    sf::Mouse::Button::Left
                    ) {
                    sf::Vector2f clickPosition =
                        window.mapPixelToCoords(
                            mouseButton->position
                        );

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
                                Screen::
                                DifficultySelect;
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
                                Screen::
                                CreatureSelect;
                        }
                        else if (
                            isButtonClicked(
                                normalButton,
                                clickPosition
                            )
                            ) {
                            difficulty = 2;

                            currentScreen =
                                Screen::
                                CreatureSelect;
                        }
                        else if (
                            isButtonClicked(
                                hardButton,
                                clickPosition
                            )
                            ) {
                            difficulty = 3;

                            currentScreen =
                                Screen::
                                CreatureSelect;
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

                    else if (
                        currentScreen ==
                        Screen::CreatureSelect
                        ) {
                        for (
                            int i = 0;
                            i < 8;
                            i++
                            ) {
                            if (
                                isButtonClicked(
                                    creatureButtons[i],
                                    clickPosition
                                )
                                ) {
                                int playerChoice =
                                    i + 1;

                                int enemyChoice =
                                    std::rand() %
                                    8 + 1;

                                while (
                                    enemyChoice ==
                                    playerChoice
                                    ) {
                                    enemyChoice =
                                        std::rand() %
                                        8 + 1;
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
                                    playerCreature
                                    .getHp();

                                enemyMaxHp =
                                    enemyCreature
                                    .getHp();

                                battle = Battle(
                                    playerCreature,
                                    enemyCreature,
                                    difficulty
                                );

                                animationState =
                                    AnimationState::None;

                                pendingMoveIndex =
                                    -1;

                                pendingMoveName.clear();

                                playerBody.setPosition(
                                    playerBodyDefault
                                );

                                playerEye.setPosition(
                                    playerEyeDefault
                                );

                                enemyBody.setPosition(
                                    enemyBodyDefault
                                );

                                enemyEye.setPosition(
                                    enemyEyeDefault
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

                                updateAllBattleUi();

                                battleMessage.setString(
                                    "Choose a move."
                                );

                                currentScreen =
                                    Screen::
                                    BattleScreen;

                                break;
                            }
                        }
                    }

                    else if (
                        currentScreen ==
                        Screen::BattleScreen &&
                        !battle.isBattleOver() &&
                        animationState ==
                        AnimationState::None
                        ) {
                        int selectedMoveIndex =
                            -1;

                        for (int i = 0; i < 4; i++) {
                            if (
                                isButtonClicked(
                                    moveButtons[i],
                                    clickPosition
                                )
                                ) {
                                selectedMoveIndex =
                                    i;

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

                            pendingMoveIndex =
                                selectedMoveIndex;

                            pendingMoveName =
                                selectedMove.name;

                            pendingEffectColor =
                                getMoveEffectColor(
                                    selectedMove.type
                                );

                            projectileEffect
                                .setFillColor(
                                    pendingEffectColor
                                );

                            sf::Color beamColor =
                                pendingEffectColor;

                            beamColor.a = 190;

                            beamEffect.setFillColor(
                                beamColor
                            );

                            sf::Color pulseColor =
                                pendingEffectColor;

                            pulseColor.a = 210;

                            statusPulse
                                .setOutlineColor(
                                    pulseColor
                                );

                            battleMessage.setString(
                                battle
                                .getPlayerPokemon()
                                .getName() +
                                " used " +
                                pendingMoveName +
                                "!"
                            );

                            animationState =
                                getPlayerAnimationState(
                                    selectedMove.animation
                                );

                            animationClock.restart();
                        }
                    }

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
                                Screen::
                                DifficultySelect;
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
            }
        }

        // --------------------------------------------------
        // Animation update
        // --------------------------------------------------

        float elapsed =
            animationClock
            .getElapsedTime()
            .asSeconds();

        if (
            currentScreen ==
            Screen::BattleScreen
            ) {
            if (
                animationState ==
                AnimationState::PlayerLunge
                ) {
                const float halfDuration =
                    0.15f;

                const float totalDuration =
                    0.30f;

                sf::Vector2f playerSpriteAttack(
                    playerSpriteDefault.x + 70.0f,
                    playerSpriteDefault.y - 25.0f
                );

                sf::Vector2f playerBodyAttack(
                    playerBodyDefault.x + 70.0f,
                    playerBodyDefault.y - 25.0f
                );

                sf::Vector2f playerEyeAttack(
                    playerEyeDefault.x + 70.0f,
                    playerEyeDefault.y - 25.0f
                );

                if (elapsed < halfDuration) {
                    float progress =
                        elapsed /
                        halfDuration;

                    setCreaturePosition(
                        playerSprite,
                        playerBody,
                        playerEye,
                        playerTextureLoaded,
                        interpolatePosition(
                            playerSpriteDefault,
                            playerSpriteAttack,
                            progress
                        ),
                        interpolatePosition(
                            playerBodyDefault,
                            playerBodyAttack,
                            progress
                        ),
                        interpolatePosition(
                            playerEyeDefault,
                            playerEyeAttack,
                            progress
                        )
                    );
                }
                else if (
                    elapsed <
                    totalDuration
                    ) {
                    float progress =
                        (elapsed - halfDuration) /
                        halfDuration;

                    setCreaturePosition(
                        playerSprite,
                        playerBody,
                        playerEye,
                        playerTextureLoaded,
                        interpolatePosition(
                            playerSpriteAttack,
                            playerSpriteDefault,
                            progress
                        ),
                        interpolatePosition(
                            playerBodyAttack,
                            playerBodyDefault,
                            progress
                        ),
                        interpolatePosition(
                            playerEyeAttack,
                            playerEyeDefault,
                            progress
                        )
                    );
                }
                else {
                    setCreaturePosition(
                        playerSprite,
                        playerBody,
                        playerEye,
                        playerTextureLoaded,
                        playerSpriteDefault,
                        playerBodyDefault,
                        playerEyeDefault
                    );

                    battle.playerAttack(
                        pendingMoveIndex
                    );

                    updateAllBattleUi();

                    animationState =
                        AnimationState::
                        EnemyFlash;

                    animationClock.restart();
                }
            }

            else if (
                animationState ==
                AnimationState::
                PlayerProjectile
                ) {
                const float duration =
                    0.40f;

                float progress =
                    elapsed / duration;

                if (progress > 1.0f) {
                    progress = 1.0f;
                }

                projectileEffect.setPosition(
                    interpolatePosition(
                        { 390.0f, 330.0f },
                        { 920.0f, 230.0f },
                        progress
                    )
                );

                if (elapsed >= duration) {
                    battle.playerAttack(
                        pendingMoveIndex
                    );

                    updateAllBattleUi();

                    animationState =
                        AnimationState::
                        EnemyFlash;

                    animationClock.restart();
                }
            }

            else if (
                animationState ==
                AnimationState::PlayerBeam
                ) {
                const float duration =
                    0.32f;

                float pulse =
                    12.0f +
                    std::sin(
                        elapsed * 45.0f
                    ) * 6.0f;

                beamEffect.setSize({
                    510.0f,
                    pulse
                    });

                if (elapsed >= duration) {
                    battle.playerAttack(
                        pendingMoveIndex
                    );

                    updateAllBattleUi();

                    animationState =
                        AnimationState::
                        EnemyFlash;

                    animationClock.restart();
                }
            }

            else if (
                animationState ==
                AnimationState::PlayerStatus
                ) {
                const float duration =
                    0.50f;

                float progress =
                    elapsed / duration;

                if (progress > 1.0f) {
                    progress = 1.0f;
                }

                float radius =
                    40.0f +
                    progress * 75.0f;

                statusPulse.setRadius(
                    radius
                );

                statusPulse.setOrigin({
                    radius,
                    radius
                    });

                if (elapsed >= duration) {
                    battle.playerAttack(
                        pendingMoveIndex
                    );

                    updateAllBattleUi();

                    animationState =
                        AnimationState::
                        EnemyFlash;

                    animationClock.restart();
                }
            }

            else if (
                animationState ==
                AnimationState::PlayerImpact
                ) {
                const float duration =
                    0.38f;

                float shakeOffset =
                    (
                        static_cast<int>(
                            elapsed * 55.0f
                            ) % 2 == 0
                        )
                    ? 10.0f
                    : -10.0f;

                setCreaturePosition(
                    enemySprite,
                    enemyBody,
                    enemyEye,
                    enemyTextureLoaded,
                    {
                        enemySpriteDefault.x +
                            shakeOffset,
                        enemySpriteDefault.y
                    },
                    {
                        enemyBodyDefault.x +
                            shakeOffset,
                        enemyBodyDefault.y
                    },
                    {
                        enemyEyeDefault.x +
                            shakeOffset,
                        enemyEyeDefault.y
                    }
                );

                if (elapsed >= duration) {
                    setCreaturePosition(
                        enemySprite,
                        enemyBody,
                        enemyEye,
                        enemyTextureLoaded,
                        enemySpriteDefault,
                        enemyBodyDefault,
                        enemyEyeDefault
                    );

                    battle.playerAttack(
                        pendingMoveIndex
                    );

                    updateAllBattleUi();

                    animationState =
                        AnimationState::
                        EnemyFlash;

                    animationClock.restart();
                }
            }

            else if (
                animationState ==
                AnimationState::EnemyFlash
                ) {
                if (elapsed < 0.22f) {
                    setCreatureColor(
                        enemySprite,
                        enemyBody,
                        enemyTextureLoaded,
                        sf::Color::Red
                    );
                }
                else {
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

                    if (battle.isBattleOver()) {
                        showBattleResult();
                    }
                    else {
                        battleMessage.setString(
                            battle
                            .getEnemyPokemon()
                            .getName() +
                            " is attacking..."
                        );

                        animationState =
                            AnimationState::
                            EnemyLunge;

                        animationClock.restart();
                    }
                }
            }

            else if (
                animationState ==
                AnimationState::EnemyLunge
                ) {
                const float halfDuration =
                    0.15f;

                const float totalDuration =
                    0.30f;

                sf::Vector2f enemySpriteAttack(
                    enemySpriteDefault.x - 70.0f,
                    enemySpriteDefault.y + 25.0f
                );

                sf::Vector2f enemyBodyAttack(
                    enemyBodyDefault.x - 70.0f,
                    enemyBodyDefault.y + 25.0f
                );

                sf::Vector2f enemyEyeAttack(
                    enemyEyeDefault.x - 70.0f,
                    enemyEyeDefault.y + 25.0f
                );

                if (elapsed < halfDuration) {
                    float progress =
                        elapsed /
                        halfDuration;

                    setCreaturePosition(
                        enemySprite,
                        enemyBody,
                        enemyEye,
                        enemyTextureLoaded,
                        interpolatePosition(
                            enemySpriteDefault,
                            enemySpriteAttack,
                            progress
                        ),
                        interpolatePosition(
                            enemyBodyDefault,
                            enemyBodyAttack,
                            progress
                        ),
                        interpolatePosition(
                            enemyEyeDefault,
                            enemyEyeAttack,
                            progress
                        )
                    );
                }
                else if (
                    elapsed <
                    totalDuration
                    ) {
                    float progress =
                        (elapsed - halfDuration) /
                        halfDuration;

                    setCreaturePosition(
                        enemySprite,
                        enemyBody,
                        enemyEye,
                        enemyTextureLoaded,
                        interpolatePosition(
                            enemySpriteAttack,
                            enemySpriteDefault,
                            progress
                        ),
                        interpolatePosition(
                            enemyBodyAttack,
                            enemyBodyDefault,
                            progress
                        ),
                        interpolatePosition(
                            enemyEyeAttack,
                            enemyEyeDefault,
                            progress
                        )
                    );
                }
                else {
                    setCreaturePosition(
                        enemySprite,
                        enemyBody,
                        enemyEye,
                        enemyTextureLoaded,
                        enemySpriteDefault,
                        enemyBodyDefault,
                        enemyEyeDefault
                    );

                    battle.enemyTurn();

                    if (!battle.isBattleOver()) {
                        battle
                            .applyEndRoundEffects();
                    }

                    updateAllBattleUi();

                    animationState =
                        AnimationState::
                        PlayerFlash;

                    animationClock.restart();
                }
            }

            else if (
                animationState ==
                AnimationState::PlayerFlash
                ) {
                if (elapsed < 0.22f) {
                    setCreatureColor(
                        playerSprite,
                        playerBody,
                        playerTextureLoaded,
                        sf::Color::Red
                    );
                }
                else {
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

                    if (battle.isBattleOver()) {
                        showBattleResult();
                    }
                    else {
                        battleMessage.setString(
                            "Round complete. "
                            "Choose another move."
                        );

                        pendingMoveIndex =
                            -1;

                        pendingMoveName.clear();

                        animationState =
                            AnimationState::None;
                    }
                }
            }
        }

        // --------------------------------------------------
        // Drawing
        // --------------------------------------------------

        window.clear();

        if (backgroundTextureLoaded) {
            window.draw(
                backgroundSprite
            );
        }
        else {
            window.draw(
                background
            );
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

            if (playerTextureLoaded) {
                window.draw(
                    playerSprite
                );
            }
            else {
                window.draw(playerBody);
                window.draw(playerEye);
            }

            if (
                animationState ==
                AnimationState::
                PlayerProjectile
                ) {
                window.draw(
                    projectileEffect
                );
            }

            if (
                animationState ==
                AnimationState::PlayerBeam
                ) {
                window.draw(
                    beamEffect
                );
            }

            if (
                animationState ==
                AnimationState::PlayerStatus
                ) {
                window.draw(
                    statusPulse
                );
            }

            if (enemyTextureLoaded) {
                window.draw(
                    enemySprite
                );
            }
            else {
                window.draw(enemyBody);
                window.draw(enemyEye);
            }

            window.draw(playerHpBack);
            window.draw(playerHpFill);

            window.draw(enemyHpBack);
            window.draw(enemyHpFill);

            for (int i = 0; i < 4; i++) {
                window.draw(
                    moveButtons[i]
                );

                window.draw(
                    moveTexts[i]
                );
            }

            window.draw(battleTitle);
            window.draw(enemyNameText);
            window.draw(enemyHpText);
            window.draw(playerNameText);
            window.draw(playerHpText);
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