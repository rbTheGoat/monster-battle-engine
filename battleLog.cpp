#include "battleLog.h"

#include <string>
#include <vector>

void addBattleLog(
    std::vector<std::string>& battleLog,
    const std::string& message
)
{
    if (message.empty()) {
        return;
    }

    battleLog.push_back(message);

    const std::size_t maximumLogLines = 5;

    while (battleLog.size() > maximumLogLines) {
        battleLog.erase(battleLog.begin());
    }
}

void updateBattleLogTexts(
    const std::vector<std::string>& battleLog,
    std::vector<sf::Text>& battleLogTexts
)
{
    for (
        std::size_t i = 0;
        i < battleLogTexts.size();
        i++
        ) {
        if (i < battleLog.size()) {
            battleLogTexts[i].setString(
                battleLog[i]
            );
        }
        else {
            battleLogTexts[i].setString("");
        }
    }
}

std::string getStatusEventText(Status status)
{
    if (status == Status::Burn) {
        return "burned";
    }

    if (status == Status::Poison) {
        return "poisoned";
    }

    if (status == Status::Sleep) {
        return "asleep";
    }

    if (status == Status::Paralysis) {
        return "paralyzed";
    }

    return "affected";
}

std::string getStatusConditionName(Status status)
{
    if (status == Status::Burn) {
        return "burn";
    }

    if (status == Status::Poison) {
        return "poison";
    }

    if (status == Status::Sleep) {
        return "sleep";
    }

    if (status == Status::Paralysis) {
        return "paralysis";
    }

    return "status effect";
}

void addMoveResultToBattleLog(
    std::vector<std::string>& battleLog,
    const std::string& targetName,
    int hpBefore,
    int hpAfter,
    Status statusBefore,
    Status statusAfter,
    const Move& move
)
{
    int damageTaken =
        hpBefore - hpAfter;

    bool damageLogged = false;
    bool statusLogged = false;

    if (damageTaken > 0) {
        addBattleLog(
            battleLog,
            targetName +
            " took " +
            std::to_string(damageTaken) +
            " damage."
        );

        damageLogged = true;
    }

    if (
        statusBefore == Status::None &&
        statusAfter != Status::None
        ) {
        addBattleLog(
            battleLog,
            targetName +
            " became " +
            getStatusEventText(statusAfter) +
            "."
        );

        statusLogged = true;
    }

    if (!damageLogged && !statusLogged) {
        if (move.damage > 0) {
            addBattleLog(
                battleLog,
                move.name +
                " caused no damage."
            );
        }
        else {
            addBattleLog(
                battleLog,
                move.name +
                " had no visible effect."
            );
        }
    }
}