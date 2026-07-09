#include <SFML/Graphics.hpp>

#include <optional>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <filesystem>

#include "pokemon.h"
#include "pokemonDatabase.h"
#include "battle.h"

enum class Screen {
    MainMenu,
    DifficultySelect,
    PokemonSelect,
    BattleScreen,
    ResultScreen
};

// --------------------------------------------------
// General button helpers
// --------------------------------------------------

bool isButtonClicked(
    sf::RectangleShape& button,
    sf::Vector2f mousePosition
) {
    return button.getGlobalBounds().contains(mousePosition);
}

void updateButtonHover(
    sf::RectangleShape& button,
    sf::Vector2f mousePosition
) {
    if (button.getGlobalBounds().contains(mousePosition)) {
        button.setFillColor(sf::Color(120, 120, 170));
    }
    else {
        button.setFillColor(sf::Color(90, 90, 120));
    }
}

// --------------------------------------------------
// Pokémon creation
// --------------------------------------------------

Pokemon getPokemonByChoice(int choice) {
    if (choice == 1) return getPikachu();
    if (choice == 2) return getCharmander();
    if (choice == 3) return getBulbasaur();
    if (choice == 4) return getSquirtle();
    if (choice == 5) return getEevee();
    if (choice == 6) return getGengar();
    if (choice == 7) return getLucario();
    if (choice == 8) return getDragonite();

    return getPikachu();
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

    float hpPercent =
        static_cast<float>(currentHp) /
        static_cast<float>(maxHp);

    hpFill.setSize({
        maxWidth * hpPercent,
        hpFill.getSize().y
        });

    if (hpPercent > 0.5f) {
        hpFill.setFillColor(sf::Color(80, 220, 100));
    }
    else if (hpPercent > 0.25f) {
        hpFill.setFillColor(sf::Color(230, 200, 60));
    }
    else {
        hpFill.setFillColor(sf::Color(220, 80, 80));
    }
}

std::string makeHpText(
    const std::string& label,
    Pokemon& pokemon,
    int maxHp
) {
    return label +
        " HP: " +
        std::to_string(pokemon.getHp()) +
        " / " +
        std::to_string(maxHp);
}

std::string getMoveNameOrEmpty(
    Pokemon& pokemon,
    int index
) {
    if (index < pokemon.getMoveCount()) {
        return pokemon.getMove(index).name;
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

// --------------------------------------------------
// Texture and sprite helpers
// --------------------------------------------------

bool loadPixelTexture(
    sf::Texture& texture,
    const std::string& path
) {
    // Missing images are allowed because we have fallback rectangles.
    if (!std::filesystem::exists(path)) {
        return false;
    }

    if (!texture.loadFromFile(path)) {
        std::cout
            << "Failed to load texture: "
            << path
            << "\n";

        return false;
    }

    // Prevents pixel art from becoming blurry.
    texture.setSmooth(false);

    return true;
}

void scaleSpriteToSize(
    sf::Sprite& sprite,
    sf::Texture& texture,
    float targetWidth,
    float targetHeight
) {
    sf::Vector2u textureSize = texture.getSize();

    if (textureSize.x == 0 || textureSize.y == 0) {
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

std::string getPokemonImagePath(
    const std::string& pokemonName
) {
    if (pokemonName == "Pikachu") {
        return "assets/images/pikachu.png";
    }

    if (pokemonName == "Charmander") {
        return "assets/images/charmander.png";
    }

    if (pokemonName == "Bulbasaur") {
        return "assets/images/bulbasaur.png";
    }

    if (pokemonName == "Squirtle") {
        return "assets/images/squirtle.png";
    }

    if (pokemonName == "Eevee") {
        return "assets/images/eevee.png";
    }

    if (pokemonName == "Gengar") {
        return "assets/images/gengar.png";
    }

    if (pokemonName == "Lucario") {
        return "assets/images/lucario.png";
    }

    if (pokemonName == "Dragonite") {
        return "assets/images/dragonite.png";
    }

    return "";
}

sf::Color getPokemonFallbackColor(
    const std::string& pokemonName
) {
    if (pokemonName == "Pikachu") {
        return sf::Color(240, 210, 70);
    }

    if (pokemonName == "Charmander") {
        return sf::Color(230, 95, 55);
    }

    if (pokemonName == "Bulbasaur") {
        return sf::Color(90, 180, 90);
    }

    if (pokemonName == "Squirtle") {
        return sf::Color(80, 170, 240);
    }

    if (pokemonName == "Eevee") {
        return sf::Color(170, 120, 75);
    }

    if (pokemonName == "Gengar") {
        return sf::Color(120, 80, 170);
    }

    if (pokemonName == "Lucario") {
        return sf::Color(70, 100, 180);
    }

    if (pokemonName == "Dragonite") {
        return sf::Color(230, 160, 80);
    }

    return sf::Color(180, 180, 180);
}

void setupPokemonVisuals(
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
        battle.getPlayerPokemon().getName();

    std::string enemyName =
        battle.getEnemyPokemon().getName();

    playerTextureLoaded = loadPixelTexture(
        playerTexture,
        getPokemonImagePath(playerName)
    );

    enemyTextureLoaded = loadPixelTexture(
        enemyTexture,
        getPokemonImagePath(enemyName)
    );

    // These colors are used when PNG files do not exist.
    playerBody.setFillColor(
        getPokemonFallbackColor(playerName)
    );

    enemyBody.setFillColor(
        getPokemonFallbackColor(enemyName)
    );

    if (playerTextureLoaded) {
        playerSprite.setTexture(
            playerTexture,
            true
        );

        playerSprite.setPosition({
            190.0f,
            260.0f
            });

        scaleSpriteToSize(
            playerSprite,
            playerTexture,
            180.0f,
            180.0f
        );
    }

    if (enemyTextureLoaded) {
        enemySprite.setTexture(
            enemyTexture,
            true
        );

        enemySprite.setPosition({
            870.0f,
            150.0f
            });

        scaleSpriteToSize(
            enemySprite,
            enemyTexture,
            180.0f,
            180.0f
        );
    }
}

// --------------------------------------------------
// Main
// --------------------------------------------------

int main() {
    std::srand(
        static_cast<unsigned int>(
            std::time(nullptr)
            )
    );

    sf::RenderWindow window(
        sf::VideoMode({ 1280, 720 }),
        "Battle Turn Simulator - 2D Prototype"
    );

    window.setFramerateLimit(60);

    sf::Font font;

    if (!font.openFromFile("assets/fonts/main.ttf")) {
        std::cout
            << "Failed to load font: "
            << "assets/fonts/main.ttf\n";

        return -1;
    }

    Screen currentScreen = Screen::MainMenu;

    // 1 = Easy
    // 2 = Normal
    // 3 = Hard
    int difficulty = 2;

    Pokemon playerPokemon = getPikachu();
    Pokemon enemyPokemon = getSquirtle();

    int playerMaxHp = playerPokemon.getHp();
    int enemyMaxHp = enemyPokemon.getHp();

    Battle battle(
        playerPokemon,
        enemyPokemon,
        difficulty
    );

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

    // --------------------------------------------------
    // Fallback background
    // --------------------------------------------------

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

    title.setFillColor(sf::Color::White);

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

    startText.setFillColor(sf::Color::White);

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

    exitText.setFillColor(sf::Color::White);

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

    difficultyTitle.setFillColor(sf::Color::White);

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

    easyText.setFillColor(sf::Color::White);

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

    normalText.setFillColor(sf::Color::White);

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

    hardText.setFillColor(sf::Color::White);

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

    backText.setFillColor(sf::Color::White);

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

    resultTitle.setFillColor(sf::Color::White);

    sf::Text resultMessage(
        font,
        "Result goes here",
        30
    );

    resultMessage.setPosition({
        420.0f,
        210.0f
        });

    resultMessage.setFillColor(sf::Color::White);

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

    playAgainText.setFillColor(sf::Color::White);

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

    resultMenuText.setFillColor(sf::Color::White);

    // --------------------------------------------------
    // Pokémon selection
    // --------------------------------------------------

    sf::RectangleShape pokemonButtons[8];

    sf::Text pokemonTexts[8] = {
        sf::Text(font, "Pikachu", 24),
        sf::Text(font, "Charmander", 24),
        sf::Text(font, "Bulbasaur", 24),
        sf::Text(font, "Squirtle", 24),
        sf::Text(font, "Eevee", 24),
        sf::Text(font, "Gengar", 24),
        sf::Text(font, "Lucario", 24),
        sf::Text(font, "Dragonite", 24)
    };

    for (int i = 0; i < 8; i++) {
        float x =
            190.0f +
            static_cast<float>(i % 2) * 480.0f;

        float y =
            170.0f +
            static_cast<float>(i / 2) * 95.0f;

        pokemonButtons[i].setSize({
            420.0f,
            65.0f
            });

        pokemonButtons[i].setPosition({
            x,
            y
            });

        pokemonButtons[i].setFillColor(
            sf::Color(90, 90, 120)
        );

        pokemonTexts[i].setPosition({
            x + 25.0f,
            y + 17.0f
            });

        pokemonTexts[i].setFillColor(
            sf::Color::White
        );
    }

    sf::Text selectTitle(
        font,
        "Choose Your Pokemon",
        38
    );

    selectTitle.setPosition({
        420.0f,
        70.0f
        });

    selectTitle.setFillColor(sf::Color::White);

    sf::Text selectHint(
        font,
        "Enemy will be selected randomly.",
        24
    );

    selectHint.setPosition({
        455.0f,
        610.0f
        });

    selectHint.setFillColor(sf::Color::White);

    // --------------------------------------------------
    // Battle screen
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

    playerBody.setPosition({
        220.0f,
        300.0f
        });

    sf::RectangleShape playerEye({
        20.0f,
        20.0f
        });

    playerEye.setPosition({
        295.0f,
        335.0f
        });

    playerEye.setFillColor(sf::Color::Black);

    sf::RectangleShape enemyBody({
        120.0f,
        120.0f
        });

    enemyBody.setPosition({
        900.0f,
        190.0f
        });

    sf::RectangleShape enemyEye({
        20.0f,
        20.0f
        });

    enemyEye.setPosition({
        925.0f,
        225.0f
        });

    enemyEye.setFillColor(sf::Color::Black);

    setupPokemonVisuals(
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

    const float hpBarMaxWidth = 300.0f;

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

    sf::RectangleShape moveButton1({
        260.0f,
        60.0f
        });

    moveButton1.setPosition({
        80.0f,
        650.0f
        });

    sf::RectangleShape moveButton2({
        260.0f,
        60.0f
        });

    moveButton2.setPosition({
        360.0f,
        650.0f
        });

    sf::RectangleShape moveButton3({
        260.0f,
        60.0f
        });

    moveButton3.setPosition({
        640.0f,
        650.0f
        });

    sf::RectangleShape moveButton4({
        260.0f,
        60.0f
        });

    moveButton4.setPosition({
        920.0f,
        650.0f
        });

    sf::Text battleTitle(
        font,
        "Battle Turn Simulator",
        34
    );

    battleTitle.setPosition({
        430.0f,
        20.0f
        });

    battleTitle.setFillColor(sf::Color::White);

    sf::Text enemyNameText(
        font,
        "Enemy",
        24
    );

    enemyNameText.setPosition({
        860.0f,
        35.0f
        });

    enemyNameText.setFillColor(sf::Color::White);

    sf::Text enemyHpText(
        font,
        "Enemy HP",
        22
    );

    enemyHpText.setPosition({
        860.0f,
        105.0f
        });

    enemyHpText.setFillColor(sf::Color::White);

    sf::Text playerNameText(
        font,
        "Player",
        24
    );

    playerNameText.setPosition({
        120.0f,
        575.0f
        });

    playerNameText.setFillColor(sf::Color::White);

    sf::Text playerHpText(
        font,
        "Player HP",
        22
    );

    playerHpText.setPosition({
        120.0f,
        640.0f
        });

    playerHpText.setFillColor(sf::Color::White);

    sf::Text battleMessage(
        font,
        "Choose a move.",
        26
    );

    battleMessage.setPosition({
        455.0f,
        585.0f
        });

    battleMessage.setFillColor(sf::Color::White);

    sf::Text moveText1(font, "Move 1", 22);
    moveText1.setPosition({ 105.0f, 666.0f });
    moveText1.setFillColor(sf::Color::White);

    sf::Text moveText2(font, "Move 2", 22);
    moveText2.setPosition({ 385.0f, 666.0f });
    moveText2.setFillColor(sf::Color::White);

    sf::Text moveText3(font, "Move 3", 22);
    moveText3.setPosition({ 665.0f, 666.0f });
    moveText3.setFillColor(sf::Color::White);

    sf::Text moveText4(font, "Move 4", 22);
    moveText4.setPosition({ 945.0f, 666.0f });
    moveText4.setFillColor(sf::Color::White);

    updateBattleText(
        battle,
        playerNameText,
        enemyNameText,
        playerHpText,
        enemyHpText,
        moveText1,
        moveText2,
        moveText3,
        moveText4,
        playerMaxHp,
        enemyMaxHp
    );

    updateHpBar(
        playerHpFill,
        battle.getPlayerPokemon().getHp(),
        playerMaxHp,
        hpBarMaxWidth
    );

    updateHpBar(
        enemyHpFill,
        battle.getEnemyPokemon().getHp(),
        enemyMaxHp,
        hpBarMaxWidth
    );

    // --------------------------------------------------
    // Main game loop
    // --------------------------------------------------

    while (window.isOpen()) {
        sf::Vector2i mousePixelPosition =
            sf::Mouse::getPosition(window);

        sf::Vector2f mousePosition =
            window.mapPixelToCoords(
                mousePixelPosition
            );

        if (currentScreen == Screen::MainMenu) {
            updateButtonHover(startButton, mousePosition);
            updateButtonHover(exitButton, mousePosition);
        }

        if (currentScreen == Screen::DifficultySelect) {
            updateButtonHover(easyButton, mousePosition);
            updateButtonHover(normalButton, mousePosition);
            updateButtonHover(hardButton, mousePosition);
            updateButtonHover(backButton, mousePosition);
        }

        if (currentScreen == Screen::PokemonSelect) {
            for (int i = 0; i < 8; i++) {
                updateButtonHover(
                    pokemonButtons[i],
                    mousePosition
                );
            }
        }

        if (
            currentScreen == Screen::BattleScreen &&
            !battle.isBattleOver()
            ) {
            updateButtonHover(moveButton1, mousePosition);
            updateButtonHover(moveButton2, mousePosition);
            updateButtonHover(moveButton3, mousePosition);
            updateButtonHover(moveButton4, mousePosition);
        }

        if (currentScreen == Screen::ResultScreen) {
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
            const std::optional event =
            window.pollEvent()
            ) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (
                const auto* mouseButton =
                event->getIf<
                sf::Event::MouseButtonPressed
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

                    // Main menu
                    if (currentScreen == Screen::MainMenu) {
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

                    // Difficulty screen
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
                                Screen::PokemonSelect;
                        }
                        else if (
                            isButtonClicked(
                                normalButton,
                                clickPosition
                            )
                            ) {
                            difficulty = 2;
                            currentScreen =
                                Screen::PokemonSelect;
                        }
                        else if (
                            isButtonClicked(
                                hardButton,
                                clickPosition
                            )
                            ) {
                            difficulty = 3;
                            currentScreen =
                                Screen::PokemonSelect;
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

                    // Pokémon selection screen
                    else if (
                        currentScreen ==
                        Screen::PokemonSelect
                        ) {
                        for (int i = 0; i < 8; i++) {
                            if (
                                isButtonClicked(
                                    pokemonButtons[i],
                                    clickPosition
                                )
                                ) {
                                int playerChoice = i + 1;

                                int enemyChoice =
                                    std::rand() % 8 + 1;

                                while (
                                    enemyChoice ==
                                    playerChoice
                                    ) {
                                    enemyChoice =
                                        std::rand() % 8 + 1;
                                }

                                playerPokemon =
                                    getPokemonByChoice(
                                        playerChoice
                                    );

                                enemyPokemon =
                                    getPokemonByChoice(
                                        enemyChoice
                                    );

                                playerMaxHp =
                                    playerPokemon.getHp();

                                enemyMaxHp =
                                    enemyPokemon.getHp();

                                battle = Battle(
                                    playerPokemon,
                                    enemyPokemon,
                                    difficulty
                                );

                                setupPokemonVisuals(
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

                                updateBattleText(
                                    battle,
                                    playerNameText,
                                    enemyNameText,
                                    playerHpText,
                                    enemyHpText,
                                    moveText1,
                                    moveText2,
                                    moveText3,
                                    moveText4,
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

                                battleMessage.setString(
                                    "Choose a move."
                                );

                                currentScreen =
                                    Screen::BattleScreen;

                                break;
                            }
                        }
                    }

                    // Battle screen
                    else if (
                        currentScreen ==
                        Screen::BattleScreen &&
                        !battle.isBattleOver()
                        ) {
                        int selectedMoveIndex = -1;

                        if (
                            isButtonClicked(
                                moveButton1,
                                clickPosition
                            )
                            ) {
                            selectedMoveIndex = 0;
                        }
                        else if (
                            isButtonClicked(
                                moveButton2,
                                clickPosition
                            )
                            ) {
                            selectedMoveIndex = 1;
                        }
                        else if (
                            isButtonClicked(
                                moveButton3,
                                clickPosition
                            )
                            ) {
                            selectedMoveIndex = 2;
                        }
                        else if (
                            isButtonClicked(
                                moveButton4,
                                clickPosition
                            )
                            ) {
                            selectedMoveIndex = 3;
                        }

                        if (
                            selectedMoveIndex != -1 &&
                            selectedMoveIndex <
                            battle
                            .getPlayerPokemon()
                            .getMoveCount()
                            ) {
                            std::string moveName =
                                battle
                                .getPlayerPokemon()
                                .getMove(
                                    selectedMoveIndex
                                )
                                .name;

                            battle.playerAttack(
                                selectedMoveIndex
                            );

                            if (!battle.isBattleOver()) {
                                battle.enemyTurn();
                            }

                            if (!battle.isBattleOver()) {
                                battle.applyEndRoundEffects();
                            }

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

                            if (battle.isBattleOver()) {
                                if (battle.didPlayerWin()) {
                                    resultTitle.setString(
                                        "You Won!"
                                    );

                                    resultMessage.setString(
                                        battle
                                        .getEnemyPokemon()
                                        .getName() +
                                        " fainted."
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
                                        " fainted."
                                    );
                                }

                                currentScreen =
                                    Screen::ResultScreen;
                            }
                            else {
                                battleMessage.setString(
                                    "You used " +
                                    moveName +
                                    ". Enemy responded."
                                );
                            }
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
            }
        }

        // --------------------------------------------------
        // Drawing
        // --------------------------------------------------

        window.clear();

        if (backgroundTextureLoaded) {
            window.draw(backgroundSprite);
        }
        else {
            window.draw(background);
        }

        if (currentScreen == Screen::MainMenu) {
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
            window.draw(difficultyTitle);

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
            Screen::PokemonSelect
            ) {
            window.draw(selectTitle);
            window.draw(selectHint);

            for (int i = 0; i < 8; i++) {
                window.draw(pokemonButtons[i]);
                window.draw(pokemonTexts[i]);
            }
        }

        else if (
            currentScreen ==
            Screen::BattleScreen
            ) {
            window.draw(ground);

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

            window.draw(playerHpBack);
            window.draw(playerHpFill);

            window.draw(enemyHpBack);
            window.draw(enemyHpFill);

            window.draw(moveButton1);
            window.draw(moveButton2);
            window.draw(moveButton3);
            window.draw(moveButton4);

            window.draw(battleTitle);

            window.draw(enemyNameText);
            window.draw(enemyHpText);

            window.draw(playerNameText);
            window.draw(playerHpText);

            window.draw(battleMessage);

            window.draw(moveText1);
            window.draw(moveText2);
            window.draw(moveText3);
            window.draw(moveText4);
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