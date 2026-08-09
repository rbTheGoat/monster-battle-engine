#include "profileScreen.h"

#include <string>
#include <vector>

void updateProfileAfterBattleSFML(
    PlayerProfile& profile,
    const BattleStats& stats,
    bool playerWon,
    int round
)
{
    profile.battlesPlayed++;

    if (playerWon) {
        profile.wins++;
    }
    else {
        profile.losses++;
    }

    profile.totalDamageDealt +=
        stats.playerDirectDamage +
        stats.statusDamageToEnemy;

    profile.totalDamageTaken +=
        stats.enemyDirectDamage +
        stats.statusDamageToPlayer;

    profile.totalRoundsPlayed += round;

    saveProfile(profile);
}

void updateProfileTexts(
    const PlayerProfile& profile,
    std::vector<sf::Text>& profileTexts
)
{
    if (profileTexts.size() < 8) {
        return;
    }

    profileTexts[0].setString(
        "Player Name: " +
        profile.playerName
    );

    profileTexts[1].setString(
        "Battles Played: " +
        std::to_string(
            profile.battlesPlayed
        )
    );

    profileTexts[2].setString(
        "Wins: " +
        std::to_string(
            profile.wins
        )
    );

    profileTexts[3].setString(
        "Losses: " +
        std::to_string(
            profile.losses
        )
    );

    if (profile.battlesPlayed > 0) {
        float winRate =
            (
                static_cast<float>(
                    profile.wins
                    ) *
                100.0f
                ) /
            static_cast<float>(
                profile.battlesPlayed
                );

        profileTexts[4].setString(
            "Win Rate: " +
            std::to_string(
                static_cast<int>(
                    winRate
                    )
            ) +
            "%"
        );
    }
    else {
        profileTexts[4].setString(
            "Win Rate: 0%"
        );
    }

    profileTexts[5].setString(
        "Total Damage Dealt: " +
        std::to_string(
            profile.totalDamageDealt
        )
    );

    profileTexts[6].setString(
        "Total Damage Taken: " +
        std::to_string(
            profile.totalDamageTaken
        )
    );

    profileTexts[7].setString(
        "Total Rounds Played: " +
        std::to_string(
            profile.totalRoundsPlayed
        )
    );
}