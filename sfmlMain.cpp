#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include <string>

#include "pokemon.h"
#include "pokemonDatabase.h"
#include "battle.h"

bool isButtonClicked(sf::RectangleShape& button, sf::Vector2f mousePosition) {
    return button.getGlobalBounds().contains(mousePosition);
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

int main() {
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

    Pokemon playerPokemon = getPikachu();
    Pokemon enemyPokemon = getSquirtle();

    int playerMaxHp = playerPokemon.getHp();
    int enemyMaxHp = enemyPokemon.getHp();

    int difficulty = 2; // Normal difficulty
    Battle battle(playerPokemon, enemyPokemon, difficulty);

    sf::RectangleShape background({ 1280.0f, 720.0f });
    background.setFillColor(sf::Color(32, 36, 48));

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

    sf::Text title(font, "Battle Turn Simulator", 34);
    title.setPosition({ 430.0f, 20.0f });
    title.setFillColor(sf::Color::White);

    sf::Text enemyName(font, "Enemy: " + battle.getEnemyPokemon().getName(), 24);
    enemyName.setPosition({ 860.0f, 35.0f });
    enemyName.setFillColor(sf::Color::White);

    sf::Text enemyHpText(font, makeHpText("Enemy", battle.getEnemyPokemon(), enemyMaxHp), 22);
    enemyHpText.setPosition({ 860.0f, 105.0f });
    enemyHpText.setFillColor(sf::Color::White);

    sf::Text playerNameText(font, "Player: " + battle.getPlayerPokemon().getName(), 24);
    playerNameText.setPosition({ 120.0f, 575.0f });
    playerNameText.setFillColor(sf::Color::White);

    sf::Text playerHpText(font, makeHpText("Player", battle.getPlayerPokemon(), playerMaxHp), 22);
    playerHpText.setPosition({ 120.0f, 640.0f });
    playerHpText.setFillColor(sf::Color::White);

    sf::Text battleMessage(font, "Choose a move.", 26);
    battleMessage.setPosition({ 455.0f, 585.0f });
    battleMessage.setFillColor(sf::Color::White);

    sf::Text moveText1(font, getMoveNameOrEmpty(battle.getPlayerPokemon(), 0), 22);
    moveText1.setPosition({ 105.0f, 666.0f });
    moveText1.setFillColor(sf::Color::White);

    sf::Text moveText2(font, getMoveNameOrEmpty(battle.getPlayerPokemon(), 1), 22);
    moveText2.setPosition({ 385.0f, 666.0f });
    moveText2.setFillColor(sf::Color::White);

    sf::Text moveText3(font, getMoveNameOrEmpty(battle.getPlayerPokemon(), 2), 22);
    moveText3.setPosition({ 665.0f, 666.0f });
    moveText3.setFillColor(sf::Color::White);

    sf::Text moveText4(font, getMoveNameOrEmpty(battle.getPlayerPokemon(), 3), 22);
    moveText4.setPosition({ 945.0f, 666.0f });
    moveText4.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        sf::Vector2i mousePixelPosition = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosition = window.mapPixelToCoords(mousePixelPosition);

        moveButton1.setFillColor(sf::Color(90, 90, 120));
        moveButton2.setFillColor(sf::Color(90, 90, 120));
        moveButton3.setFillColor(sf::Color(90, 90, 120));
        moveButton4.setFillColor(sf::Color(90, 90, 120));

        if (!battle.isBattleOver()) {
            if (moveButton1.getGlobalBounds().contains(mousePosition)) {
                moveButton1.setFillColor(sf::Color(120, 120, 170));
            }

            if (moveButton2.getGlobalBounds().contains(mousePosition)) {
                moveButton2.setFillColor(sf::Color(120, 120, 170));
            }

            if (moveButton3.getGlobalBounds().contains(mousePosition)) {
                moveButton3.setFillColor(sf::Color(120, 120, 170));
            }

            if (moveButton4.getGlobalBounds().contains(mousePosition)) {
                moveButton4.setFillColor(sf::Color(120, 120, 170));
            }
        }

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButton->button == sf::Mouse::Button::Left && !battle.isBattleOver()) {
                    sf::Vector2f clickPosition = window.mapPixelToCoords(mouseButton->position);

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

        window.clear();

        window.draw(background);
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

        window.draw(title);
        window.draw(enemyName);
        window.draw(enemyHpText);
        window.draw(playerNameText);
        window.draw(playerHpText);
        window.draw(battleMessage);

        window.draw(moveText1);
        window.draw(moveText2);
        window.draw(moveText3);
        window.draw(moveText4);

        window.display();
    }

    return 0;
}