#include <SFML/Graphics.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "pokemon.h"
#include "pokemonDatabase.h"
#include "battle.h"

#include "battleLog.h"
#include "creatureVisuals.h"
#include "moveAnimations.h"
#include "sfmlHelpers.h"
#include "saveSystem.h"
#include "profileScreen.h"
#include "audioManager.h"

// ==================================================
// Screens
// ==================================================

enum class Screen {
    NameEntry,
    MainMenu,
    DifficultySelect,
    CreatureSelect,
    BattleScreen,
    ResultScreen,
    ProfileScreen
};

// ==================================================
// Creature selection
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

    // ==================================================
    // Font
    // ==================================================

    sf::Font font;

    if (!font.openFromFile("assets/fonts/main.ttf")) {
        std::cout
            << "Failed to load font: "
            << "assets/fonts/main.ttf\n";

        return -1;
    }

    // ==================================================
    // Audio
    // ==================================================

    AudioManager audio;
    audio.load();

    // ==================================================
    // Profile / screen state
    // ==================================================

    Screen currentScreen =
        Screen::MainMenu;

    PlayerProfile profile =
        loadProfile();

    std::string enteredPlayerName =
        profile.playerName;

    Screen nameEntryReturnScreen =
        Screen::MainMenu;

    if (
        profile.playerName.empty() ||
        profile.playerName == "Player"
        ) {
        enteredPlayerName.clear();

        currentScreen =
            Screen::NameEntry;
    }

    bool profileUpdatedForBattle =
        false;

    std::vector<std::string> battleLog;

    // ==================================================
    // Battle state
    // ==================================================

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

    // ==================================================
    // Animation state
    // ==================================================

    bool animationActive = false;
    bool playerIsAttacking = true;
    bool impactApplied = false;

    sf::Clock animationClock;

    Move activeMove{};

    MoveVisual activeVisual =
        MoveVisual::Generic;

    int activePlayerMoveIndex = -1;

    float activeMoveDuration =
        0.85f;

    float activeImpactProgress =
        0.58f;

    // ==================================================
    // Creature default positions
    // ==================================================

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

    // ==================================================
    // Textures
    // ==================================================

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

    // ==================================================
    // Main menu
    // ==================================================

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

    sf::RectangleShape profileButton({
        340.0f,
        70.0f
        });

    profileButton.setPosition({
        470.0f,
        360.0f
        });

    profileButton.setFillColor(
        sf::Color(90, 90, 120)
    );

    sf::Text profileButtonText(
        font,
        "View Profile",
        30
    );

    profileButtonText.setPosition({
        550.0f,
        378.0f
        });

    profileButtonText.setFillColor(
        sf::Color::White
    );

    sf::RectangleShape exitButton({
        340.0f,
        70.0f
        });

    exitButton.setPosition({
        470.0f,
        460.0f
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
        478.0f
        });

    exitText.setFillColor(
        sf::Color::White
    );

    // ==================================================
    // Difficulty screen
    // ==================================================

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

    // ==================================================
    // Creature select
    // ==================================================

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

    // ==================================================
    // Result screen
    // ==================================================

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

    // ==================================================
    // Profile screen
    // ==================================================

    sf::Text profileTitle(
        font,
        "Player Profile",
        42
    );

    profileTitle.setPosition({
        480.0f,
        70.0f
        });

    profileTitle.setFillColor(
        sf::Color::White
    );

    std::vector<sf::Text> profileTexts;

    profileTexts.reserve(8);

    for (int i = 0; i < 8; i++) {
        sf::Text profileLine(
            font,
            "",
            24
        );

        profileLine.setPosition({
            390.0f,
            150.0f +
                static_cast<float>(i) *
                48.0f
            });

        profileLine.setFillColor(
            sf::Color::White
        );

        profileTexts.push_back(
            profileLine
        );
    }

    sf::RectangleShape profileChangeNameButton({
        260.0f,
        60.0f
        });

    profileChangeNameButton.setPosition({
        350.0f,
        570.0f
        });

    profileChangeNameButton.setFillColor(
        sf::Color(90, 90, 120)
    );

    sf::Text profileChangeNameText(
        font,
        "Change Name",
        26
    );

    profileChangeNameText.setPosition({
        400.0f,
        586.0f
        });

    profileChangeNameText.setFillColor(
        sf::Color::White
    );

    sf::RectangleShape profileBackButton({
        260.0f,
        60.0f
        });

    profileBackButton.setPosition({
        670.0f,
        570.0f
        });

    profileBackButton.setFillColor(
        sf::Color(90, 90, 120)
    );

    sf::Text profileBackText(
        font,
        "Back",
        28
    );

    profileBackText.setPosition({
        770.0f,
        585.0f
        });

    profileBackText.setFillColor(
        sf::Color::White
    );

    updateProfileTexts(
        profile,
        profileTexts
    );

    // ==================================================
    // Name entry screen
    // ==================================================

    sf::Text nameEntryTitle(
        font,
        "Enter Player Name",
        42
    );

    nameEntryTitle.setPosition({
        445.0f,
        150.0f
        });

    nameEntryTitle.setFillColor(
        sf::Color::White
    );

    sf::RectangleShape nameInputBox({
        500.0f,
        70.0f
        });

    nameInputBox.setPosition({
        390.0f,
        280.0f
        });

    nameInputBox.setFillColor(
        sf::Color(50, 50, 70)
    );

    nameInputBox.setOutlineThickness(
        2.0f
    );

    nameInputBox.setOutlineColor(
        sf::Color::White
    );

    sf::Text nameInputText(
        font,
        enteredPlayerName,
        28
    );

    nameInputText.setPosition({
        415.0f,
        298.0f
        });

    nameInputText.setFillColor(
        sf::Color::White
    );

    sf::Text nameEntryHint(
        font,
        "Type your name, then press Enter or Save",
        21
    );

    nameEntryHint.setPosition({
        420.0f,
        375.0f
        });

    nameEntryHint.setFillColor(
        sf::Color(210, 210, 220)
    );

    sf::Text nameLengthHint(
        font,
        "Maximum 18 characters",
        18
    );

    nameLengthHint.setPosition({
        520.0f,
        410.0f
        });

    nameLengthHint.setFillColor(
        sf::Color(175, 175, 190)
    );

    sf::RectangleShape nameSaveButton({
        220.0f,
        60.0f
        });

    nameSaveButton.setPosition({
        395.0f,
        480.0f
        });

    nameSaveButton.setFillColor(
        sf::Color(90, 90, 120)
    );

    sf::Text nameSaveText(
        font,
        "Save",
        28
    );

    nameSaveText.setPosition({
        470.0f,
        495.0f
        });

    nameSaveText.setFillColor(
        sf::Color::White
    );

    sf::RectangleShape nameCancelButton({
        220.0f,
        60.0f
        });

    nameCancelButton.setPosition({
        665.0f,
        480.0f
        });

    nameCancelButton.setFillColor(
        sf::Color(90, 90, 120)
    );

    sf::Text nameCancelText(
        font,
        "Cancel",
        28
    );

    nameCancelText.setPosition({
        730.0f,
        495.0f
        });

    nameCancelText.setFillColor(
        sf::Color::White
    );

    // ==================================================
    // Battle field
    // ==================================================

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

    // ==================================================
    // HP bars
    // ==================================================

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

    // ==================================================
    // Move buttons
    // ==================================================

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

    // ==================================================
    // Battle text
    // ==================================================

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

    // ==================================================
    // Battle log
    // ==================================================

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
    // Reset creature visuals
    // ==================================================

    auto resetCreatureVisualState = [&]()
        {
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

    // ==================================================
    // Refresh UI
    // ==================================================

    auto refreshBattleUi = [&]()
        {
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

    // ==================================================
    // Result
    // ==================================================

    auto showBattleResult = [&]()
        {
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

            // Only execute result events once.
            if (!profileUpdatedForBattle) {

                if (battle.didPlayerWin()) {
                    audio.playWin();
                }
                else {
                    audio.playLose();
                }

                updateProfileAfterBattleSFML(
                    profile,
                    battle.getStats(),
                    battle.didPlayerWin(),
                    battle.getRound()
                );

                updateProfileTexts(
                    profile,
                    profileTexts
                );

                profileUpdatedForBattle =
                    true;
            }

            currentScreen =
                Screen::ResultScreen;
        };

    // ==================================================
    // Move animation
    // ==================================================

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
                getMoveVisual(
                    move.name
                );

            playerIsAttacking =
                isPlayerMove;

            activePlayerMoveIndex =
                playerMoveIndex;

            activeMoveDuration =
                getMoveDuration(
                    activeVisual
                );

            activeImpactProgress =
                getMoveImpactProgress(
                    activeVisual
                );

            impactApplied = false;
            animationActive = true;

            // Elemental move sound.
            audio.playMove(
                activeMove.type
            );

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

    // ==================================================
    // End round
    // ==================================================

    auto finishRoundAfterEnemy = [&]()
        {
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

    // ==================================================
    // Enemy turn
    // ==================================================

    auto beginEnemyTurn = [&]()
        {
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

    // ==================================================
    // Save name
    // ==================================================

    auto saveEnteredPlayerName = [&]()
        {
            if (enteredPlayerName.empty()) {
                return;
            }

            profile.playerName =
                enteredPlayerName;

            saveProfile(
                profile
            );

            updateProfileTexts(
                profile,
                profileTexts
            );

            currentScreen =
                nameEntryReturnScreen;
        };

    refreshBattleUi();

    // ==================================================
    // Game loop
    // ==================================================

    while (window.isOpen()) {

        sf::Vector2i mousePixelPosition =
            sf::Mouse::getPosition(
                window
            );

        sf::Vector2f mousePosition =
            window.mapPixelToCoords(
                mousePixelPosition
            );

        // ==================================================
        // Hover
        // ==================================================

        if (
            currentScreen ==
            Screen::MainMenu
            ) {
            updateButtonHover(
                startButton,
                mousePosition
            );

            updateButtonHover(
                profileButton,
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

        if (
            currentScreen ==
            Screen::ProfileScreen
            ) {
            updateButtonHover(
                profileChangeNameButton,
                mousePosition
            );

            updateButtonHover(
                profileBackButton,
                mousePosition
            );
        }

        if (
            currentScreen ==
            Screen::NameEntry
            ) {
            updateButtonHover(
                nameSaveButton,
                mousePosition,
                !enteredPlayerName.empty()
            );

            updateButtonHover(
                nameCancelButton,
                mousePosition
            );
        }

        // ==================================================
        // Events
        // ==================================================

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

            // --------------------------------------------------
            // Name typing
            // --------------------------------------------------

            if (
                currentScreen ==
                Screen::NameEntry
                ) {
                const auto* textEntered =
                    event->getIf<
                    sf::Event::TextEntered
                    >();

                if (textEntered != nullptr) {

                    char32_t unicode =
                        textEntered->unicode;

                    if (unicode == 8) {

                        if (
                            !enteredPlayerName.empty()
                            ) {
                            enteredPlayerName.pop_back();
                        }
                    }

                    else if (
                        unicode == 13 ||
                        unicode == 10
                        ) {
                        saveEnteredPlayerName();
                    }

                    else if (
                        unicode >= 32 &&
                        unicode <= 126 &&
                        enteredPlayerName.size() < 18
                        ) {
                        enteredPlayerName +=
                            static_cast<char>(
                                unicode
                                );
                    }

                    nameInputText.setString(
                        enteredPlayerName
                    );
                }
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

            // ==================================================
            // Main menu
            // ==================================================

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
                    audio.playButton();

                    currentScreen =
                        Screen::DifficultySelect;
                }

                else if (
                    isButtonClicked(
                        profileButton,
                        clickPosition
                    )
                    ) {
                    audio.playButton();

                    updateProfileTexts(
                        profile,
                        profileTexts
                    );

                    currentScreen =
                        Screen::ProfileScreen;
                }

                else if (
                    isButtonClicked(
                        exitButton,
                        clickPosition
                    )
                    ) {
                    audio.playButton();

                    window.close();
                }
            }

            // ==================================================
            // Difficulty
            // ==================================================

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
                    audio.playButton();

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
                    audio.playButton();

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
                    audio.playButton();

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
                    audio.playButton();

                    currentScreen =
                        Screen::MainMenu;
                }
            }

            // ==================================================
            // Creature selection
            // ==================================================

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

                    audio.playButton();

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

                    profileUpdatedForBattle =
                        false;

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

            // ==================================================
            // Battle move
            // ==================================================

            else if (
                currentScreen ==
                Screen::BattleScreen &&
                !animationActive &&
                !battle.isBattleOver()
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
                    audio.playButton();

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

            // ==================================================
            // Result screen
            // ==================================================

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
                    audio.playButton();

                    currentScreen =
                        Screen::DifficultySelect;
                }

                else if (
                    isButtonClicked(
                        resultMenuButton,
                        clickPosition
                    )
                    ) {
                    audio.playButton();

                    currentScreen =
                        Screen::MainMenu;
                }
            }

            // ==================================================
            // Profile screen
            // ==================================================

            else if (
                currentScreen ==
                Screen::ProfileScreen
                ) {

                if (
                    isButtonClicked(
                        profileChangeNameButton,
                        clickPosition
                    )
                    ) {
                    audio.playButton();

                    enteredPlayerName =
                        profile.playerName;

                    nameInputText.setString(
                        enteredPlayerName
                    );

                    nameEntryTitle.setString(
                        "Change Player Name"
                    );

                    nameEntryReturnScreen =
                        Screen::ProfileScreen;

                    currentScreen =
                        Screen::NameEntry;
                }

                else if (
                    isButtonClicked(
                        profileBackButton,
                        clickPosition
                    )
                    ) {
                    audio.playButton();

                    currentScreen =
                        Screen::MainMenu;
                }
            }

            // ==================================================
            // Name entry buttons
            // ==================================================

            else if (
                currentScreen ==
                Screen::NameEntry
                ) {

                if (
                    isButtonClicked(
                        nameSaveButton,
                        clickPosition
                    ) &&
                    !enteredPlayerName.empty()
                    ) {
                    audio.playButton();

                    saveEnteredPlayerName();
                }

                else if (
                    isButtonClicked(
                        nameCancelButton,
                        clickPosition
                    )
                    ) {
                    audio.playButton();

                    enteredPlayerName =
                        profile.playerName;

                    nameInputText.setString(
                        enteredPlayerName
                    );

                    currentScreen =
                        nameEntryReturnScreen;
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

            float targetShake =
                0.0f;

            if (
                progress >=
                activeImpactProgress
                ) {
                targetShake =
                    (
                        static_cast<int>(
                            elapsed *
                            55.0f
                            ) %
                        2 ==
                        0
                        )
                    ? -9.0f
                    : 9.0f;
            }

            // ==================================================
            // Movement category
            // ==================================================

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

            float movementDistance =
                0.0f;

            if (strongLunge) {
                movementDistance =
                    160.0f;
            }

            else if (mediumLunge) {
                movementDistance =
                    85.0f;
            }

            // ==================================================
            // Burrow Rush
            // ==================================================

            if (
                activeVisual ==
                MoveVisual::BurrowRush
                ) {

                float sinkAmount =
                    0.0f;

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
                    sinkAmount =
                        1.0f;
                }

                sinkAmount =
                    clamp01(
                        sinkAmount
                    );

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

            // ==================================================
            // Normal lunges
            // ==================================================

            else if (
                movementDistance >
                0.0f
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

            // ==================================================
            // Phantom fade
            // ==================================================

            if (
                activeVisual ==
                MoveVisual::PhantomDive
                ) {

                int alpha =
                    95 +
                    static_cast<int>(
                        160.0f *
                        std::abs(
                            progress -
                            0.5f
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

            // ==================================================
            // Flashstep fade
            // ==================================================

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
                        ) *
                        90.0f
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

            // ==================================================
            // Target shake
            // ==================================================

            bool targetShouldShake =
                progress >=
                activeImpactProgress &&
                activeMove.damage >
                0;

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

            // ==================================================
            // Impact tint
            // ==================================================

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
                    Type::Ghost
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

            // ==================================================
            // Actual battle impact
            // ==================================================

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

                if (playerIsAttacking) {

                    battle.playerAttack(
                        activePlayerMoveIndex
                    );
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

                // Only play the impact sound when damage
                // actually landed. A missed/non-damaging
                // attack won't make a fake hit noise.
                if (
                    targetHpAfter <
                    targetHpBefore
                    ) {
                    audio.playHit();
                }

                addMoveResultToBattleLog(
                    battleLog,
                    targetName,
                    targetHpBefore,
                    targetHpAfter,
                    targetStatusBefore,
                    targetStatusAfter,
                    activeMove
                );

                updateBattleLogTexts(
                    battleLog,
                    battleLogTexts
                );

                impactApplied =
                    true;

                refreshBattleUi();
            }

            // ==================================================
            // Animation finished
            // ==================================================

            if (
                elapsed >=
                activeMoveDuration
                ) {

                resetCreatureVisualState();

                animationActive =
                    false;

                if (
                    battle.isBattleOver()
                    ) {
                    showBattleResult();
                }

                else if (
                    playerIsAttacking
                    ) {
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
            window.draw(
                background
            );
        }

        // ==================================================
        // Name entry
        // ==================================================

        if (
            currentScreen ==
            Screen::NameEntry
            ) {

            window.draw(
                nameEntryTitle
            );

            window.draw(
                nameInputBox
            );

            window.draw(
                nameInputText
            );

            window.draw(
                nameEntryHint
            );

            window.draw(
                nameLengthHint
            );

            window.draw(
                nameSaveButton
            );

            window.draw(
                nameCancelButton
            );

            window.draw(
                nameSaveText
            );

            window.draw(
                nameCancelText
            );
        }

        // ==================================================
        // Main menu
        // ==================================================

        else if (
            currentScreen ==
            Screen::MainMenu
            ) {

            window.draw(
                title
            );

            window.draw(
                startButton
            );

            window.draw(
                profileButton
            );

            window.draw(
                exitButton
            );

            window.draw(
                startText
            );

            window.draw(
                profileButtonText
            );

            window.draw(
                exitText
            );
        }

        // ==================================================
        // Difficulty
        // ==================================================

        else if (
            currentScreen ==
            Screen::DifficultySelect
            ) {

            window.draw(
                difficultyTitle
            );

            window.draw(
                easyButton
            );

            window.draw(
                normalButton
            );

            window.draw(
                hardButton
            );

            window.draw(
                backButton
            );

            window.draw(
                easyText
            );

            window.draw(
                normalText
            );

            window.draw(
                hardText
            );

            window.draw(
                backText
            );
        }

        // ==================================================
        // Creature selection
        // ==================================================

        else if (
            currentScreen ==
            Screen::CreatureSelect
            ) {

            window.draw(
                selectTitle
            );

            window.draw(
                selectHint
            );

            for (int i = 0; i < 8; i++) {

                window.draw(
                    creatureButtons[i]
                );

                window.draw(
                    creatureTexts[i]
                );
            }
        }

        // ==================================================
        // Battle screen
        // ==================================================

        else if (
            currentScreen ==
            Screen::BattleScreen
            ) {

            window.draw(
                ground
            );

            // Ground effects first.
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

            // Player.
            if (playerTextureLoaded) {

                window.draw(
                    playerSprite
                );
            }

            else {

                window.draw(
                    playerBody
                );

                window.draw(
                    playerEye
                );
            }

            // Enemy.
            if (enemyTextureLoaded) {

                window.draw(
                    enemySprite
                );
            }

            else {

                window.draw(
                    enemyBody
                );

                window.draw(
                    enemyEye
                );
            }

            // Foreground attack effects.
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

            window.draw(
                playerHpBack
            );

            window.draw(
                playerHpFill
            );

            window.draw(
                enemyHpBack
            );

            window.draw(
                enemyHpFill
            );

            for (int i = 0; i < 4; i++) {

                window.draw(
                    moveButtons[i]
                );

                window.draw(
                    moveTexts[i]
                );
            }

            window.draw(
                battleTitle
            );

            window.draw(
                enemyNameText
            );

            window.draw(
                enemyHpText
            );

            window.draw(
                enemyStatusText
            );

            window.draw(
                enemyStatusIcon
            );

            window.draw(
                playerNameText
            );

            window.draw(
                playerHpText
            );

            window.draw(
                playerStatusText
            );

            window.draw(
                playerStatusIcon
            );

            window.draw(
                battleLogPanel
            );

            window.draw(
                battleLogTitle
            );

            for (
                std::size_t i = 0;
                i < battleLogTexts.size();
                i++
                ) {

                window.draw(
                    battleLogTexts[i]
                );
            }

            window.draw(
                battleMessage
            );
        }

        // ==================================================
        // Result
        // ==================================================

        else if (
            currentScreen ==
            Screen::ResultScreen
            ) {

            window.draw(
                resultTitle
            );

            window.draw(
                resultMessage
            );

            window.draw(
                playAgainButton
            );

            window.draw(
                resultMenuButton
            );

            window.draw(
                playAgainText
            );

            window.draw(
                resultMenuText
            );
        }

        // ==================================================
        // Profile
        // ==================================================

        else if (
            currentScreen ==
            Screen::ProfileScreen
            ) {

            window.draw(
                profileTitle
            );

            for (
                std::size_t i = 0;
                i < profileTexts.size();
                i++
                ) {

                window.draw(
                    profileTexts[i]
                );
            }

            window.draw(
                profileChangeNameButton
            );

            window.draw(
                profileBackButton
            );

            window.draw(
                profileChangeNameText
            );

            window.draw(
                profileBackText
            );
        }

        window.display();
    }

    return 0;
}