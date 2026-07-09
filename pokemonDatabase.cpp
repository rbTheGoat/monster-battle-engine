#include "pokemonDatabase.h"

Pokemon getVoltbit()
{
    Pokemon voltbit(
        "Voltbit",
        100,
        Type::Electric
    );

    voltbit.addMove({
        "Arc Pounce",
        25,
        100,
        Type::Electric,
        Status::Paralysis,
        15,
        2,
        MoveAnimation::Lunge
    });

    voltbit.addMove({
        "Coil Discharge",
        35,
        95,
        Type::Electric,
        Status::Paralysis,
        20,
        2,
        MoveAnimation::Beam
    });

    voltbit.addMove({
        "Flashstep",
        20,
        100,
        Type::Normal,
        Status::None,
        0,
        0,
        MoveAnimation::Lunge
    });

    return voltbit;
}

Pokemon getEmberling()
{
    Pokemon emberling(
        "Emberling",
        95,
        Type::Fire
    );

    emberling.addMove({
        "Cinder Claw",
        25,
        100,
        Type::Fire,
        Status::Burn,
        15,
        3,
        MoveAnimation::Lunge
    });

    emberling.addMove({
        "Flame Spit",
        35,
        90,
        Type::Fire,
        Status::Burn,
        20,
        3,
        MoveAnimation::Projectile
    });

    emberling.addMove({
        "Tail Sweep",
        20,
        100,
        Type::Normal,
        Status::None,
        0,
        0,
        MoveAnimation::Impact
    });

    return emberling;
}

Pokemon getMossback()
{
    Pokemon mossback(
        "Mossback",
        110,
        Type::Grass
    );

    mossback.addMove({
        "Vine Coil",
        25,
        100,
        Type::Grass,
        Status::None,
        0,
        0,
        MoveAnimation::Lunge
    });

    mossback.addMove({
        "Thorn Burst",
        35,
        90,
        Type::Grass,
        Status::None,
        0,
        0,
        MoveAnimation::Projectile
    });

    mossback.addMove({
        "Dream Spores",
        0,
        75,
        Type::Grass,
        Status::Sleep,
        100,
        2,
        MoveAnimation::StatusPulse
    });

    mossback.addMove({
        "Venom Bloom",
        0,
        80,
        Type::Poison,
        Status::Poison,
        100,
        4,
        MoveAnimation::StatusPulse
    });

    return mossback;
}

Pokemon getAquafin()
{
    Pokemon aquafin(
        "Aquafin",
        105,
        Type::Water
    );

    aquafin.addMove({
        "Riptide Rush",
        30,
        100,
        Type::Water,
        Status::None,
        0,
        0,
        MoveAnimation::Lunge
    });

    aquafin.addMove({
        "Razor Fin",
        25,
        100,
        Type::Normal,
        Status::None,
        0,
        0,
        MoveAnimation::Lunge
    });

    aquafin.addMove({
        "Tidal Lance",
        40,
        85,
        Type::Water,
        Status::None,
        0,
        0,
        MoveAnimation::Projectile
    });

    return aquafin;
}

Pokemon getWildkin()
{
    Pokemon wildkin(
        "Wildkin",
        100,
        Type::Normal
    );

    wildkin.addMove({
        "Burrow Rush",
        25,
        100,
        Type::Normal,
        Status::None,
        0,
        0,
        MoveAnimation::Lunge
    });

    wildkin.addMove({
        "Sand Slam",
        35,
        90,
        Type::Normal,
        Status::None,
        0,
        0,
        MoveAnimation::Impact
    });

    wildkin.addMove({
        "Tremor Stomp",
        30,
        90,
        Type::Normal,
        Status::Paralysis,
        20,
        2,
        MoveAnimation::Impact
    });

    return wildkin;
}

Pokemon getShadegeist()
{
    Pokemon shadegeist(
        "Shadegeist",
        90,
        Type::Ghost
    );

    shadegeist.addMove({
        "Phantom Dive",
        35,
        95,
        Type::Ghost,
        Status::None,
        0,
        0,
        MoveAnimation::Lunge
    });

    shadegeist.addMove({
        "Void Orb",
        40,
        90,
        Type::Ghost,
        Status::None,
        0,
        0,
        MoveAnimation::Projectile
    });

    shadegeist.addMove({
        "Dread Gaze",
        0,
        80,
        Type::Dark,
        Status::Sleep,
        100,
        2,
        MoveAnimation::StatusPulse
    });

    shadegeist.addMove({
        "Shadow Talon",
        30,
        100,
        Type::Dark,
        Status::None,
        0,
        0,
        MoveAnimation::Lunge
    });

    return shadegeist;
}

Pokemon getIronfang()
{
    Pokemon ironfang(
        "Ironfang",
        115,
        Type::Fighting
    );

    ironfang.addMove({
        "Alloy Charge",
        35,
        95,
        Type::Steel,
        Status::None,
        0,
        0,
        MoveAnimation::Lunge
    });

    ironfang.addMove({
        "Horn Breaker",
        40,
        85,
        Type::Fighting,
        Status::None,
        0,
        0,
        MoveAnimation::Lunge
    });

    ironfang.addMove({
        "Steel Jab",
        25,
        100,
        Type::Steel,
        Status::None,
        0,
        0,
        MoveAnimation::Lunge
    });

    ironfang.addMove({
        "Groundbreaker",
        30,
        90,
        Type::Fighting,
        Status::Paralysis,
        20,
        2,
        MoveAnimation::Impact
    });

    return ironfang;
}

Pokemon getSkywyrm()
{
    Pokemon skywyrm(
        "Skywyrm",
        130,
        Type::Dragon
    );

    skywyrm.addMove({
        "Wyrm Claw",
        40,
        95,
        Type::Dragon,
        Status::None,
        0,
        0,
        MoveAnimation::Lunge
    });

    skywyrm.addMove({
        "Gale Sweep",
        30,
        100,
        Type::Flying,
        Status::None,
        0,
        0,
        MoveAnimation::Projectile
    });

    skywyrm.addMove({
        "Azure Beam",
        50,
        75,
        Type::Dragon,
        Status::None,
        0,
        0,
        MoveAnimation::Beam
    });

    return skywyrm;
}