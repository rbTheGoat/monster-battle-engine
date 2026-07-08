#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "pokemon.h"
#include "pokemonDatabase.h"
#include "battle.h"

enum class Screen {
    MainMenu,
    PokemonSelect,
    BattleScreen
};

bool isButtonClicked(sf::RectangleShape& button, sf::Vector2f mousePosition) {
    return button.getGlobalBounds().contains(mousePosition);
}

void updateButtonHover(sf::RectangleShape& button, sf::Vector2f mousePosition) {
    if (button.getGlobalBounds().contains(mousePosition)) {
        button.setFillColor(sf::Color(120, 120, 170));
    }
    else {
        button.setFillColor(sf::Color(90, 90, 120));
    }
}

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

std::string getPokemonNameByChoice(int choice) {
    Pokemon pokemon = getPokemonByChoice(choice);
    return pokemon.getName();
}

void updateHpBar(sf::RectangleShape& hpFill, int currentHp, int maxHp, float maxWidth) {
    if (currentHp < 0) {
        currentHp = 0;
    }

    float hpPercent = static_cast<float>(currentHp) / maxHp;
    hpFill.setSize({ maxWidth * hpPercent, hpFill.getSize().y });

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

std::string makeHpText(std::string label, Pokemon& pokemon, int maxHp) {
    return label + " HP: " + std::to_string(pokemon.getHp()) + " / " + std::to_string(maxHp);
}

std::string getMoveNameOrEmpty(Pokemon& pokemon, int index) {
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
    playerNameText.setString("Player: " + battle.getPlayerPokemon().getName());
    enemyNameText.setString("Enemy: " + battle.getEnemyPokemon().getName());

    playerHpText.setString(makeHpText("Player", battle.getPlayerPokemon(), playerMaxHp));
    enemyHpText.setString(makeHpText("Enemy", battle.getEnemyPokemon(), enemyMaxHp));

    moveText1.setString(getMoveNameOrEmpty(battle.getPlayerPokemon(), 0));
    moveText2.setString(getMoveNameOrEmpty(battle.getPlayerPokemon(), 1));
    moveText3.setString(getMoveNameOrEmpty(battle.getPlayerPokemon(), 2));
    moveText4.setString(getMoveNameOrEmpty(battle.getPlayerPokemon(), 3));
}

int main() {
    srand(time(0));

    sf::RenderWindow window(
        sf::VideoMode({ 1280, 720 }),
        "Battle Turn Simulator - 2D Prototype"
    );

    window.setFramerateLimit(60);

    sf::Font font;

    if (!font.openFromFile("assets/fonts/main.ttf")) {
        std::cout << "Failed to load font: assets/fonts/main.ttf\n";
        return -1;
    }

    Screen currentScreen = Screen::MainMenu;

    int difficulty = 2;

    Pokemon playerPokemon = getPikachu();
    Pokemon enemyPokemon = getSquirtle();

    int playerMaxHp = playerPokemon.getHp();
    int enemyMaxHp = enemyPokemon.getHp();

    Battle battle(playerPokemon, enemyPokemon, difficulty);

    sf::RectangleShape background({ 1280.0f, 720.0f });
    background.setFillColor(sf::Color(32, 36, 48));

    sf::Text title(font, "Battle Turn Simulator", 42);
    title.setPosition({ 390.0f, 70.0f });
    title.setFillColor(sf::Color::White);

    // Main menu buttons
    sf::RectangleShape startButton({ 340.0f, 70.0f });
    startButton.setPosition({ 470.0f, 260.0f });
    startButton.setFillColor(sf::Color(90, 90, 120));

    sf::Text startText(font, "Start Battle", 30);
    startText.setPosition({ 560.0f, 278.0f });
    startText.setFillColor(sf::Color::White);

    sf::RectangleShape exitButton({ 340.0f, 70.0f });
    exitButton.setPosition({ 470.0f, 360.0f });
    exitButton.setFillColor(sf::Color(90, 90, 120));

    sf::Text exitText(font, "Exit", 30);
    exitText.setPosition({ 610.0f, 378.0f });
    exitText.setFillColor(sf::Color::White);

    // Pokemon selection buttons
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
        float x = 190.0f + (i % 2) * 480.0f;
        float y = 170.0f + (i / 2) * 95.0f;

        pokemonButtons[i].setSize({ 420.0f, 65.0f });
        pokemonButtons[i].setPosition({ x, y });
        pokemonButtons[i].setFillColor(sf::Color(90, 90, 120));

        pokemonTexts[i].setPosition({ x + 25.0f, y + 17.0f });
        pokemonTexts[i].setFillColor(sf::Color::White);
    }

    sf::Text selectTitle(font, "Choose Your Pokemon", 38);
    selectTitle.setPosition({ 420.0f, 70.0f });
    selectTitle.setFillColor(sf::Color::White);

    sf::Text selectHint(font, "Enemy will be selected randomly.", 24);
    selectHint.setPosition({ 455.0f, 610.0f });
    selectHint.setFillColor(sf::Color::White);

    // Battle screen shapes
    sf::RectangleShape ground({ 1000.0f, 180.0f });
    ground.setPosition({ 140.0f, 400.0f });
    ground.setFillColor(sf::Color(70, 90, 70));

    sf::RectangleShape playerBody({ 120.0f, 120.0f });
    playerBody.setPosition({ 220.0f, 300.0f });
    playerBody.setFillColor(sf::Color(240, 210, 70));

    sf::RectangleShape playerEye({ 20.0f, 20.0f });
    playerEye.setPosition({ 295.0f, 335.0f });
    playerEye.setFillColor(sf::Color::Black);

    sf::RectangleShape enemyBody({ 120.0f, 120.0f });
    enemyBody.setPosition({ 900.0f, 190.0f });
    enemyBody.setFillColor(sf::Color(80, 170, 240));

    sf::RectangleShape enemyEye({ 20.0f, 20.0f });
    enemyEye.setPosition({ 925.0f, 225.0f });
    enemyEye.setFillColor(sf::Color::Black);

    const float hpBarMaxWidth = 300.0f;

    sf::RectangleShape playerHpBack({ hpBarMaxWidth, 28.0f });
    playerHpBack.setPosition({ 120.0f, 610.0f });
    playerHpBack.setFillColor(sf::Color(60, 60, 60));

    sf::RectangleShape playerHpFill({ hpBarMaxWidth, 28.0f });
    playerHpFill.setPosition({ 120.0f, 610.0f });
    playerHpFill.setFillColor(sf::Color(80, 220, 100));

    sf::RectangleShape enemyHpBack({ hpBarMaxWidth, 28.0f });
    enemyHpBack.setPosition({ 860.0f, 70.0f });
    enemyHpBack.setFillColor(sf::Color(60, 60, 60));

    sf::RectangleShape enemyHpFill({ hpBarMaxWidth, 28.0f });
    enemyHpFill.setPosition({ 860.0f, 70.0f });
    enemyHpFill.setFillColor(sf::Color(80, 220, 100));

    sf::RectangleShape moveButton1({ 260.0f, 60.0f });
    moveButton1.setPosition({ 80.0f, 650.0f });

    sf::RectangleShape moveButton2({ 260.0f, 60.0f });
    moveButton2.setPosition({ 360.0f, 650.0f });

    sf::RectangleShape moveButton3({ 260.0f, 60.0f });
    moveButton3.setPosition({ 640.0f, 650.0f });

    sf::RectangleShape moveButton4({ 260.0f, 60.0f });
    moveButton4.setPosition({ 920.0f, 650.0f });

    sf::Text battleTitle(font, "Battle Turn Simulator", 34);
    battleTitle.setPosition({ 430.0f, 20.0f });
    battleTitle.setFillColor(sf::Color::White);

    sf::Text enemyNameText(font, "Enemy: Squirtle", 24);
    enemyNameText.setPosition({ 860.0f, 35.0f });
    enemyNameText.setFillColor(sf::Color::White);

    sf::Text enemyHpText(font, "Enemy HP", 22);
    enemyHpText.setPosition({ 860.0f, 105.0f });
    enemyHpText.setFillColor(sf::Color::White);

    sf::Text playerNameText(font, "Player: Pikachu", 24);
    playerNameText.setPosition({ 120.0f, 575.0f });
    playerNameText.setFillColor(sf::Color::White);

    sf::Text playerHpText(font, "Player HP", 22);
    playerHpText.setPosition({ 120.0f, 640.0f });
    playerHpText.setFillColor(sf::Color::White);

    sf::Text battleMessage(font, "Choose a move.", 26);
    battleMessage.setPosition({ 455.0f, 585.0f });
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

    while (window.isOpen()) {
        sf::Vector2i mousePixelPosition = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosition = window.mapPixelToCoords(mousePixelPosition);

        if (currentScreen == Screen::MainMenu) {
            updateButtonHover(startButton, mousePosition);
            updateButtonHover(exitButton, mousePosition);
        }

        if (currentScreen == Screen::PokemonSelect) {
            for (int i = 0; i < 8; i++) {
                updateButtonHover(pokemonButtons[i], mousePosition);
            }
        }

        if (currentScreen == Screen::BattleScreen && !battle.isBattleOver()) {
            updateButtonHover(moveButton1, mousePosition);
            updateButtonHover(moveButton2, mousePosition);
            updateButtonHover(moveButton3, mousePosition);
            updateButtonHover(moveButton4, mousePosition);
        }

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButton->button == sf::Mouse::Button::Left) {
                    sf::Vector2f clickPosition = window.mapPixelToCoords(mouseButton->position);

                    if (currentScreen == Screen::MainMenu) {
                        if (isButtonClicked(startButton, clickPosition)) {
                            currentScreen = Screen::PokemonSelect;
                        }

                        else if (isButtonClicked(exitButton, clickPosition)) {
                            window.close();
                        }
                    }

                    else if (currentScreen == Screen::PokemonSelect) {
                        for (int i = 0; i < 8; i++) {
                            if (isButtonClicked(pokemonButtons[i], clickPosition)) {
                                int playerChoice = i + 1;

                                int enemyChoice = rand() % 8 + 1;

                                while (enemyChoice == playerChoice) {
                                    enemyChoice = rand() % 8 + 1;
                                }

                                playerPokemon = getPokemonByChoice(playerChoice);
                                enemyPokemon = getPokemonByChoice(enemyChoice);

                                playerMaxHp = playerPokemon.getHp();
                                enemyMaxHp = enemyPokemon.getHp();

                                battle = Battle(playerPokemon, enemyPokemon, difficulty);

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

                                updateHpBar(playerHpFill, battle.getPlayerPokemon().getHp(), playerMaxHp, hpBarMaxWidth);
                                updateHpBar(enemyHpFill, battle.getEnemyPokemon().getHp(), enemyMaxHp, hpBarMaxWidth);

                                battleMessage.setString("Choose a move.");

                                currentScreen = Screen::BattleScreen;
                            }
                        }
                    }

                    else if (currentScreen == Screen::BattleScreen && !battle.isBattleOver()) {
                        int selectedMoveIndex = -1;

                        if (isButtonClicked(moveButton1, clickPosition)) {
                            selectedMoveIndex = 0;
                        }
                        else if (isButtonClicked(moveButton2, clickPosition)) {
                            selectedMoveIndex = 1;
                        }
                        else if (isButtonClicked(moveButton3, clickPosition)) {
                            selectedMoveIndex = 2;
                        }
                        else if (isButtonClicked(moveButton4, clickPosition)) {
                            selectedMoveIndex = 3;
                        }

                        if (selectedMoveIndex != -1 &&
                            selectedMoveIndex < battle.getPlayerPokemon().getMoveCount()) {

                            std::string moveName = battle.getPlayerPokemon().getMove(selectedMoveIndex).name;

                            battle.playerAttack(selectedMoveIndex);

                            if (!battle.isBattleOver()) {
                                battle.enemyTurn();
                            }

                            if (!battle.isBattleOver()) {
                                battle.applyEndRoundEffects();
                            }

                            playerHpText.setString(makeHpText("Player", battle.getPlayerPokemon(), playerMaxHp));
                            enemyHpText.setString(makeHpText("Enemy", battle.getEnemyPokemon(), enemyMaxHp));

                            updateHpBar(playerHpFill, battle.getPlayerPokemon().getHp(), playerMaxHp, hpBarMaxWidth);
                            updateHpBar(enemyHpFill, battle.getEnemyPokemon().getHp(), enemyMaxHp, hpBarMaxWidth);

                            if (battle.isBattleOver()) {
                                if (battle.didPlayerWin()) {
                                    battleMessage.setString("You won! " + battle.getEnemyPokemon().getName() + " fainted.");
                                }
                                else {
                                    battleMessage.setString("You lost! " + battle.getPlayerPokemon().getName() + " fainted.");
                                }
                            }
                            else {
                                battleMessage.setString("You used " + moveName + ". Enemy responded.");
                            }
                        }
                    }
                }
            }
        }

        window.clear();

        window.draw(background);

        if (currentScreen == Screen::MainMenu) {
            window.draw(title);
            window.draw(startButton);
            window.draw(exitButton);
            window.draw(startText);
            window.draw(exitText);
        }

        else if (currentScreen == Screen::PokemonSelect) {
            window.draw(selectTitle);
            window.draw(selectHint);

            for (int i = 0; i < 8; i++) {
                window.draw(pokemonButtons[i]);
                window.draw(pokemonTexts[i]);
            }
        }

        else if (currentScreen == Screen::BattleScreen) {
            window.draw(ground);

            window.draw(playerBody);
            window.draw(playerEye);

            window.draw(enemyBody);
            window.draw(enemyEye);

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

        window.display();
    }

    return 0;
}