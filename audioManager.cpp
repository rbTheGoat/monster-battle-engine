#include "audioManager.h"

#include <iostream>

bool AudioManager::loadSound(
    sf::SoundBuffer& buffer,
    std::optional<sf::Sound>& sound,
    const std::string& path
)
{
    if (!buffer.loadFromFile(path)) {
        std::cout
            << "Warning: could not load sound: "
            << path
            << '\n';

        return false;
    }

    sound.emplace(buffer);

    return true;
}

bool AudioManager::load()
{
    bool allLoaded = true;

    if (!loadSound(
        buttonBuffer,
        buttonSound,
        "assets/audio/button.wav"
    )) {
        allLoaded = false;
    }

    if (!loadSound(
        hitBuffer,
        hitSound,
        "assets/audio/hit.wav"
    )) {
        allLoaded = false;
    }

    if (!loadSound(
        fireBuffer,
        fireSound,
        "assets/audio/fire.wav"
    )) {
        allLoaded = false;
    }

    if (!loadSound(
        waterBuffer,
        waterSound,
        "assets/audio/water.wav"
    )) {
        allLoaded = false;
    }

    if (!loadSound(
        electricBuffer,
        electricSound,
        "assets/audio/electric.wav"
    )) {
        allLoaded = false;
    }

    if (!loadSound(
        ghostBuffer,
        ghostSound,
        "assets/audio/ghost.wav"
    )) {
        allLoaded = false;
    }

    if (!loadSound(
        winBuffer,
        winSound,
        "assets/audio/win.wav"
    )) {
        allLoaded = false;
    }

    if (!loadSound(
        loseBuffer,
        loseSound,
        "assets/audio/lose.wav"
    )) {
        allLoaded = false;
    }

    // --------------------------------------------------
    // Volumes
    // --------------------------------------------------

    if (buttonSound) {
        buttonSound->setVolume(35.0f);
    }

    if (hitSound) {
        hitSound->setVolume(55.0f);
    }

    if (fireSound) {
        fireSound->setVolume(55.0f);
    }

    if (waterSound) {
        waterSound->setVolume(55.0f);
    }

    if (electricSound) {
        electricSound->setVolume(55.0f);
    }

    if (ghostSound) {
        ghostSound->setVolume(55.0f);
    }

    if (winSound) {
        winSound->setVolume(65.0f);
    }

    if (loseSound) {
        loseSound->setVolume(65.0f);
    }

    return allLoaded;
}

void AudioManager::playSound(
    std::optional<sf::Sound>& sound
)
{
    if (!sound) {
        return;
    }

    // Restart the effect if it was already playing.
    sound->stop();
    sound->play();
}

void AudioManager::playButton()
{
    playSound(buttonSound);
}

void AudioManager::playHit()
{
    playSound(hitSound);
}

void AudioManager::playMove(Type type)
{
    switch (type) {

    case Type::Fire:
        playSound(fireSound);
        break;

    case Type::Water:
        playSound(waterSound);
        break;

    case Type::Electric:
        playSound(electricSound);
        break;

    case Type::Ghost:
        playSound(ghostSound);
        break;

    default:
        // Normal, Grass, Fighting, Dragon, etc.
        // receive the generic impact sound when damage lands.
        break;
    }
}

void AudioManager::playWin()
{
    playSound(winSound);
}

void AudioManager::playLose()
{
    playSound(loseSound);
}