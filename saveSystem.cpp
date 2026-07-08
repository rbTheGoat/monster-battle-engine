#include "saveSystem.h"
#include <iostream>
#include <fstream>
#include <limits>

const std::string SAVE_FILE_NAME = "save.txt";

PlayerProfile createDefaultProfile() {
    PlayerProfile profile;
    profile.playerName = "Player";
    profile.battlesPlayed = 0;
    profile.wins = 0;
    profile.losses = 0;
    profile.totalDamageDealt = 0;
    profile.totalDamageTaken = 0;
    profile.totalRoundsPlayed = 0;

    return profile;
}

PlayerProfile loadProfile() {
    PlayerProfile profile = createDefaultProfile();

    std::ifstream inputFile(SAVE_FILE_NAME);

    if (!inputFile.is_open()) {
        saveProfile(profile);
        return profile;
    }

    std::getline(inputFile, profile.playerName);

    if (profile.playerName.empty()) {
        profile.playerName = "Player";
    }

    inputFile >> profile.battlesPlayed;
    inputFile >> profile.wins;
    inputFile >> profile.losses;
    inputFile >> profile.totalDamageDealt;
    inputFile >> profile.totalDamageTaken;
    inputFile >> profile.totalRoundsPlayed;

    if (inputFile.fail()) {
        std::cout << "Save file was corrupted. Loading default profile.\n";
        return createDefaultProfile();
    }

    inputFile.close();

    return profile;
}

void saveProfile(const PlayerProfile& profile) {
    std::ofstream outputFile(SAVE_FILE_NAME);

    if (!outputFile.is_open()) {
        std::cout << "Error: Could not save profile.\n";
        return;
    }

    outputFile << profile.playerName << "\n";
    outputFile << profile.battlesPlayed << "\n";
    outputFile << profile.wins << "\n";
    outputFile << profile.losses << "\n";
    outputFile << profile.totalDamageDealt << "\n";
    outputFile << profile.totalDamageTaken << "\n";
    outputFile << profile.totalRoundsPlayed << "\n";

    outputFile.close();
}

void displayProfile(const PlayerProfile& profile) {
    std::cout << "\n========= Player Profile =========\n";
    std::cout << "Player Name: " << profile.playerName << "\n";
    std::cout << "Battles Played: " << profile.battlesPlayed << "\n";
    std::cout << "Wins: " << profile.wins << "\n";
    std::cout << "Losses: " << profile.losses << "\n";

    if (profile.battlesPlayed > 0) {
        float winRate = (profile.wins * 100.0f) / profile.battlesPlayed;
        std::cout << "Win Rate: " << winRate << "%\n";
    } else {
        std::cout << "Win Rate: 0%\n";
    }

    std::cout << "Total Damage Dealt: " << profile.totalDamageDealt << "\n";
    std::cout << "Total Damage Taken: " << profile.totalDamageTaken << "\n";
    std::cout << "Total Rounds Played: " << profile.totalRoundsPlayed << "\n";
}

void resetProfile() {
    PlayerProfile defaultProfile = createDefaultProfile();
    saveProfile(defaultProfile);
}
void changePlayerName(PlayerProfile& profile) {
    std::cout << "\nEnter new player name: ";

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, profile.playerName);

    if (profile.playerName.empty()) {
        profile.playerName = "Player";
    }

    saveProfile(profile);

    std::cout << "Player name updated to: " << profile.playerName << "\n";
}