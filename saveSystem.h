#pragma once
#include <string>

struct PlayerProfile {
    std::string playerName = "Player";
    int battlesPlayed = 0;
    int wins = 0;
    int losses = 0;
    int totalDamageDealt = 0;
    int totalDamageTaken = 0;
    int totalRoundsPlayed = 0;
};

PlayerProfile createDefaultProfile();
PlayerProfile loadProfile();
void saveProfile(const PlayerProfile& profile);
void displayProfile(const PlayerProfile& profile);
void resetProfile();