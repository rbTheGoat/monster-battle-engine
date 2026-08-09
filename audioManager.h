#pragma once

#include <SFML/Audio.hpp>

#include <optional>
#include <string>

#include "pokemon.h"

class AudioManager {
public:
    AudioManager() = default;

    bool load();

    void playButton();
    void playHit();

    void playMove(Type type);

    void playWin();
    void playLose();

private:
    bool loadSound(
        sf::SoundBuffer& buffer,
        std::optional<sf::Sound>& sound,
        const std::string& path
    );

    void playSound(
        std::optional<sf::Sound>& sound
    );

    // --------------------------------------------------
    // Buffers
    //
    // These MUST live as long as their sf::Sound objects.
    // --------------------------------------------------

    sf::SoundBuffer buttonBuffer;
    sf::SoundBuffer hitBuffer;

    sf::SoundBuffer fireBuffer;
    sf::SoundBuffer waterBuffer;
    sf::SoundBuffer electricBuffer;
    sf::SoundBuffer ghostBuffer;

    sf::SoundBuffer winBuffer;
    sf::SoundBuffer loseBuffer;

    // --------------------------------------------------
    // Sounds
    // --------------------------------------------------

    std::optional<sf::Sound> buttonSound;
    std::optional<sf::Sound> hitSound;

    std::optional<sf::Sound> fireSound;
    std::optional<sf::Sound> waterSound;
    std::optional<sf::Sound> electricSound;
    std::optional<sf::Sound> ghostSound;

    std::optional<sf::Sound> winSound;
    std::optional<sf::Sound> loseSound;
};