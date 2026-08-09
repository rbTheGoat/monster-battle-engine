#include "moveAnimations.h"
#include "sfmlHelpers.h"

#include <cmath>
#include <string>

MoveVisual getMoveVisual(
    const std::string& moveName
)
{
    if (moveName == "Arc Pounce") {
        return MoveVisual::ArcPounce;
    }

    if (moveName == "Coil Discharge") {
        return MoveVisual::CoilDischarge;
    }

    if (moveName == "Flashstep") {
        return MoveVisual::Flashstep;
    }

    if (moveName == "Cinder Claw") {
        return MoveVisual::CinderClaw;
    }

    if (moveName == "Flame Spit") {
        return MoveVisual::FlameSpit;
    }

    if (moveName == "Tail Sweep") {
        return MoveVisual::TailSweep;
    }

    if (moveName == "Vine Coil") {
        return MoveVisual::VineCoil;
    }

    if (moveName == "Thorn Burst") {
        return MoveVisual::ThornBurst;
    }

    if (moveName == "Dream Spores") {
        return MoveVisual::DreamSpores;
    }

    if (moveName == "Venom Bloom") {
        return MoveVisual::VenomBloom;
    }

    if (moveName == "Riptide Rush") {
        return MoveVisual::RiptideRush;
    }

    if (moveName == "Razor Fin") {
        return MoveVisual::RazorFin;
    }

    if (moveName == "Tidal Lance") {
        return MoveVisual::TidalLance;
    }

    if (moveName == "Burrow Rush") {
        return MoveVisual::BurrowRush;
    }

    if (moveName == "Sand Slam") {
        return MoveVisual::SandSlam;
    }

    if (moveName == "Tremor Stomp") {
        return MoveVisual::TremorStomp;
    }

    if (moveName == "Phantom Dive") {
        return MoveVisual::PhantomDive;
    }

    if (moveName == "Void Orb") {
        return MoveVisual::VoidOrb;
    }

    if (moveName == "Dread Gaze") {
        return MoveVisual::DreadGaze;
    }

    if (moveName == "Shadow Talon") {
        return MoveVisual::ShadowTalon;
    }

    if (moveName == "Alloy Charge") {
        return MoveVisual::AlloyCharge;
    }

    if (moveName == "Horn Breaker") {
        return MoveVisual::HornBreaker;
    }

    if (moveName == "Steel Jab") {
        return MoveVisual::SteelJab;
    }

    if (moveName == "Groundbreaker") {
        return MoveVisual::Groundbreaker;
    }

    if (moveName == "Wyrm Claw") {
        return MoveVisual::WyrmClaw;
    }

    if (moveName == "Gale Sweep") {
        return MoveVisual::GaleSweep;
    }

    if (moveName == "Azure Beam") {
        return MoveVisual::AzureBeam;
    }

    return MoveVisual::Generic;
}

sf::Color getMoveEffectColor(Type type)
{
    if (type == Type::Electric) {
        return sf::Color(255, 230, 60);
    }

    if (type == Type::Fire) {
        return sf::Color(255, 95, 35);
    }

    if (type == Type::Water) {
        return sf::Color(55, 180, 255);
    }

    if (type == Type::Grass) {
        return sf::Color(85, 220, 100);
    }

    if (type == Type::Ghost) {
        return sf::Color(155, 75, 225);
    }

    if (type == Type::Dark) {
        return sf::Color(80, 45, 125);
    }

    if (type == Type::Steel) {
        return sf::Color(195, 210, 225);
    }

    if (type == Type::Fighting) {
        return sf::Color(225, 80, 55);
    }

    if (type == Type::Dragon) {
        return sf::Color(75, 120, 255);
    }

    if (type == Type::Flying) {
        return sf::Color(190, 225, 255);
    }

    if (type == Type::Poison) {
        return sf::Color(180, 60, 205);
    }

    return sf::Color(235, 235, 235);
}

float getMoveDuration(MoveVisual visual)
{
    if (
        visual == MoveVisual::AzureBeam ||
        visual == MoveVisual::CoilDischarge
        ) {
        return 1.20f;
    }

    if (
        visual == MoveVisual::DreamSpores ||
        visual == MoveVisual::VenomBloom ||
        visual == MoveVisual::DreadGaze
        ) {
        return 1.10f;
    }

    if (
        visual == MoveVisual::BurrowRush ||
        visual == MoveVisual::PhantomDive
        ) {
        return 1.00f;
    }

    return 0.85f;
}

float getMoveImpactProgress(MoveVisual visual)
{
    if (
        visual == MoveVisual::AzureBeam ||
        visual == MoveVisual::CoilDischarge
        ) {
        return 0.68f;
    }

    if (
        visual == MoveVisual::DreamSpores ||
        visual == MoveVisual::VenomBloom ||
        visual == MoveVisual::DreadGaze
        ) {
        return 0.72f;
    }

    return 0.58f;
}

// ==================================================
// Shape drawing helpers
// ==================================================

void drawLine(
    sf::RenderWindow& window,
    sf::Vector2f start,
    sf::Vector2f end,
    float thickness,
    sf::Color color
)
{
    float dx = end.x - start.x;
    float dy = end.y - start.y;

    float length =
        std::sqrt(dx * dx + dy * dy);

    if (length <= 0.0f) {
        return;
    }

    float angle =
        std::atan2(dy, dx) *
        180.0f /
        3.14159265358979323846f;

    sf::RectangleShape line({
        length,
        thickness
        });

    line.setOrigin({
        0.0f,
        thickness / 2.0f
        });

    line.setPosition(start);
    line.setRotation(sf::degrees(angle));
    line.setFillColor(color);

    window.draw(line);
}

void drawRing(
    sf::RenderWindow& window,
    sf::Vector2f center,
    float radius,
    float thickness,
    sf::Color color
)
{
    sf::CircleShape ring(radius);

    ring.setOrigin({
        radius,
        radius
        });

    ring.setPosition(center);

    ring.setFillColor(
        sf::Color::Transparent
    );

    ring.setOutlineThickness(thickness);
    ring.setOutlineColor(color);

    window.draw(ring);
}

void drawFilledCircle(
    sf::RenderWindow& window,
    sf::Vector2f center,
    float radius,
    sf::Color color
)
{
    sf::CircleShape circle(radius);

    circle.setOrigin({
        radius,
        radius
        });

    circle.setPosition(center);
    circle.setFillColor(color);

    window.draw(circle);
}

void drawSparkBurst(
    sf::RenderWindow& window,
    sf::Vector2f center,
    float radius,
    sf::Color color,
    int amount
)
{
    for (int i = 0; i < amount; i++) {
        float angle =
            (
                6.28318530718f *
                static_cast<float>(i)
                ) /
            static_cast<float>(amount);

        sf::Vector2f inner({
            center.x +
                std::cos(angle) *
                radius *
                0.3f,
            center.y +
                std::sin(angle) *
                radius *
                0.3f
            });

        sf::Vector2f outer({
            center.x +
                std::cos(angle) *
                radius,
            center.y +
                std::sin(angle) *
                radius
            });

        drawLine(
            window,
            inner,
            outer,
            4.0f,
            color
        );
    }
}

void drawLightningBolt(
    sf::RenderWindow& window,
    sf::Vector2f start,
    sf::Vector2f end,
    sf::Color color,
    float time
)
{
    const int segments = 8;

    sf::Vector2f previous = start;

    for (int i = 1; i <= segments; i++) {
        float progress =
            static_cast<float>(i) /
            static_cast<float>(segments);

        sf::Vector2f current =
            interpolatePosition(
                start,
                end,
                progress
            );

        if (i < segments) {
            float offset =
                std::sin(
                    time * 70.0f +
                    static_cast<float>(i) *
                    2.7f
                ) * 18.0f;

            current.x += offset;
        }

        drawLine(
            window,
            previous,
            current,
            8.0f,
            withAlpha(color, 245)
        );

        drawLine(
            window,
            previous,
            current,
            3.0f,
            sf::Color::White
        );

        previous = current;
    }
}

void drawFloatingParticles(
    sf::RenderWindow& window,
    sf::Vector2f center,
    float time,
    sf::Color color,
    int amount,
    float spread,
    float radius
)
{
    for (int i = 0; i < amount; i++) {
        float phase =
            static_cast<float>(i) *
            1.85f;

        float x =
            center.x +
            std::sin(
                time * 2.8f +
                phase
            ) * spread;

        float y =
            center.y +
            std::cos(
                time * 2.0f +
                phase
            ) * spread * 0.55f -
            std::fmod(
                time * 45.0f +
                static_cast<float>(i) *
                17.0f,
                spread
            );

        drawFilledCircle(
            window,
            { x, y },
            radius +
            static_cast<float>(i % 3),
            withAlpha(
                color,
                130 +
                (i % 3) *
                35
            )
        );
    }
}

// ==================================================
// Move-effect drawing
// ==================================================

void drawMoveEffects(
    sf::RenderWindow& window,
    MoveVisual visual,
    Type moveType,
    bool playerAttacking,
    float elapsed,
    float duration
)
{
    float progress =
        clamp01(
            elapsed /
            duration
        );

    float impactProgress =
        getMoveImpactProgress(
            visual
        );

    sf::Vector2f playerCenter({
        285.0f,
        335.0f
        });

    sf::Vector2f enemyCenter({
        955.0f,
        230.0f
        });

    sf::Vector2f attackerCenter =
        playerAttacking
        ? playerCenter
        : enemyCenter;

    sf::Vector2f targetCenter =
        playerAttacking
        ? enemyCenter
        : playerCenter;

    sf::Color color =
        getMoveEffectColor(
            moveType
        );

    float direction =
        playerAttacking
        ? 1.0f
        : -1.0f;

    // --------------------------------------------------
    // VOLTBIT
    // --------------------------------------------------

    if (visual == MoveVisual::ArcPounce) {
        if (progress > 0.30f) {
            float boltStrength =
                clamp01(
                    (progress - 0.30f) /
                    0.25f
                );

            drawLightningBolt(
                window,
                {
                    targetCenter.x,
                    -20.0f
                },
                targetCenter,
                withAlpha(
                    sf::Color(
                        255,
                        235,
                        70
                    ),
                    static_cast<int>(
                        255.0f *
                        boltStrength
                        )
                ),
                elapsed
            );

            drawSparkBurst(
                window,
                targetCenter,
                45.0f +
                pulse(
                    elapsed,
                    25.0f
                ) *
                20.0f,
                sf::Color(
                    255,
                    235,
                    70
                ),
                10
            );
        }

        return;
    }

    if (
        visual ==
        MoveVisual::CoilDischarge
        ) {
        float chargeProgress =
            clamp01(
                progress /
                0.48f
            );

        for (int i = 0; i < 3; i++) {
            drawRing(
                window,
                attackerCenter,
                30.0f +
                chargeProgress *
                65.0f +
                static_cast<float>(i) *
                16.0f,
                4.0f,
                withAlpha(
                    color,
                    200 -
                    i *
                    45
                )
            );
        }

        if (progress > 0.48f) {
            drawLightningBolt(
                window,
                attackerCenter,
                targetCenter,
                color,
                elapsed
            );

            drawLightningBolt(
                window,
                {
                    attackerCenter.x,
                    attackerCenter.y -
                        18.0f
                },
                {
                    targetCenter.x,
                    targetCenter.y +
                        22.0f
                },
                withAlpha(
                    color,
                    190
                ),
                elapsed +
                0.13f
            );
        }

        return;
    }

    if (
        visual ==
        MoveVisual::Flashstep
        ) {
        float travel =
            progress <
            impactProgress
            ? progress /
            impactProgress
            : 1.0f;

        sf::Vector2f current =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(travel)
            );

        for (int i = 1; i <= 4; i++) {
            drawLine(
                window,
                {
                    current.x -
                        direction *
                        static_cast<float>(i) *
                        42.0f,
                    current.y +
                        static_cast<float>(
                            i % 2
                        ) *
                        8.0f
                },
                {
                    current.x -
                        direction *
                        static_cast<float>(i) *
                        8.0f,
                    current.y
                },
                5.0f,
                withAlpha(
                    sf::Color::White,
                    190 -
                    i *
                    32
                )
            );
        }

        if (
            progress >
            impactProgress -
            0.06f
            ) {
            drawSparkBurst(
                window,
                targetCenter,
                55.0f,
                sf::Color::White,
                8
            );
        }

        return;
    }

    // --------------------------------------------------
    // EMBERLING
    // --------------------------------------------------

    if (
        visual ==
        MoveVisual::CinderClaw
        ) {
        if (progress > 0.38f) {
            float slashProgress =
                clamp01(
                    (progress - 0.38f) /
                    0.26f
                );

            for (
                int i = -1;
                i <= 1;
                i++
                ) {
                sf::Vector2f start({
                    targetCenter.x -
                        direction *
                        55.0f,
                    targetCenter.y -
                        55.0f +
                        static_cast<float>(i) *
                        16.0f
                    });

                sf::Vector2f end({
                    targetCenter.x +
                        direction *
                        55.0f *
                        slashProgress,
                    targetCenter.y +
                        50.0f +
                        static_cast<float>(i) *
                        16.0f
                    });

                drawLine(
                    window,
                    start,
                    end,
                    7.0f,
                    withAlpha(
                        sf::Color(
                            255,
                            115,
                            35
                        ),
                        220
                    )
                );
            }

            drawFloatingParticles(
                window,
                targetCenter,
                elapsed,
                sf::Color(
                    255,
                    125,
                    40
                ),
                8,
                55.0f,
                4.0f
            );
        }

        return;
    }

    if (
        visual ==
        MoveVisual::FlameSpit
        ) {
        float travel =
            clamp01(
                progress /
                impactProgress
            );

        sf::Vector2f fireballPosition =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(travel)
            );

        if (
            progress <
            impactProgress
            ) {
            drawFilledCircle(
                window,
                fireballPosition,
                18.0f +
                pulse(
                    elapsed,
                    30.0f
                ) *
                7.0f,
                sf::Color(
                    255,
                    90,
                    25
                )
            );

            drawFilledCircle(
                window,
                fireballPosition,
                9.0f,
                sf::Color(
                    255,
                    225,
                    90
                )
            );

            for (int i = 1; i <= 4; i++) {
                drawFilledCircle(
                    window,
                    {
                        fireballPosition.x -
                            direction *
                            static_cast<float>(i) *
                            16.0f,
                        fireballPosition.y +
                            std::sin(
                                elapsed *
                                    20.0f +
                                static_cast<float>(i)
                            ) *
                            7.0f
                    },
                    7.0f -
                    static_cast<float>(i),
                    withAlpha(
                        sf::Color(
                            255,
                            115,
                            35
                        ),
                        210 -
                        i *
                        35
                    )
                );
            }
        }
        else {
            float explosion =
                clamp01(
                    (
                        progress -
                        impactProgress
                        ) /
                    (
                        1.0f -
                        impactProgress
                        )
                );

            drawRing(
                window,
                targetCenter,
                25.0f +
                explosion *
                85.0f,
                10.0f,
                withAlpha(
                    sf::Color(
                        255,
                        80,
                        20
                    ),
                    static_cast<int>(
                        255.0f *
                        (
                            1.0f -
                            explosion
                            )
                        )
                )
            );

            drawSparkBurst(
                window,
                targetCenter,
                45.0f +
                explosion *
                55.0f,
                sf::Color(
                    255,
                    135,
                    35
                ),
                12
            );
        }

        return;
    }

    if (
        visual ==
        MoveVisual::TailSweep
        ) {
        if (progress > 0.30f) {
            float sweep =
                clamp01(
                    (progress - 0.30f) /
                    0.35f
                );

            for (int i = 0; i < 3; i++) {
                float offset =
                    static_cast<float>(i) *
                    15.0f;

                drawLine(
                    window,
                    {
                        targetCenter.x -
                            direction *
                            (
                                80.0f -
                                offset
                            ),
                        targetCenter.y +
                            45.0f +
                            offset
                    },
                    {
                        targetCenter.x +
                            direction *
                            70.0f *
                            sweep,
                        targetCenter.y -
                            35.0f +
                            offset
                    },
                    6.0f,
                    withAlpha(
                        sf::Color(
                            255,
                            160,
                            70
                        ),
                        220 -
                        i *
                        45
                    )
                );
            }
        }

        return;
    }

    // --------------------------------------------------
    // MOSSBACK
    // --------------------------------------------------

    if (
        visual ==
        MoveVisual::VineCoil
        ) {
        if (progress > 0.25f) {
            float coilProgress =
                clamp01(
                    (progress - 0.25f) /
                    0.55f
                );

            for (int i = 0; i < 4; i++) {
                float radius =
                    95.0f -
                    coilProgress *
                    45.0f +
                    static_cast<float>(i) *
                    10.0f;

                drawRing(
                    window,
                    {
                        targetCenter.x,
                        targetCenter.y +
                            static_cast<float>(
                                i - 2
                            ) *
                            12.0f
                    },
                    radius,
                    7.0f,
                    withAlpha(
                        sf::Color(
                            60,
                            190,
                            85
                        ),
                        210 -
                        i *
                        25
                    )
                );
            }
        }

        return;
    }

    if (
        visual ==
        MoveVisual::ThornBurst
        ) {
        for (int i = 0; i < 3; i++) {
            float localProgress =
                clamp01(
                    (
                        progress -
                        static_cast<float>(i) *
                        0.12f
                        ) /
                    0.58f
                );

            sf::Vector2f position =
                interpolatePosition(
                    attackerCenter,
                    targetCenter,
                    smoothStep(
                        localProgress
                    )
                );

            sf::RectangleShape thorn({
                36.0f,
                8.0f
                });

            thorn.setOrigin({
                18.0f,
                4.0f
                });

            thorn.setPosition({
                position.x,
                position.y +
                    static_cast<float>(
                        i - 1
                    ) *
                    25.0f
                });

            thorn.setFillColor(
                sf::Color(
                    90,
                    220,
                    95
                )
            );

            thorn.setRotation(
                sf::degrees(
                    playerAttacking
                    ? -10.0f
                    : 170.0f
                )
            );

            window.draw(thorn);
        }

        if (
            progress >
            impactProgress
            ) {
            drawSparkBurst(
                window,
                targetCenter,
                50.0f,
                sf::Color(
                    100,
                    235,
                    105
                ),
                8
            );
        }

        return;
    }

    if (
        visual ==
        MoveVisual::DreamSpores
        ) {
        float fadeIn =
            clamp01(
                progress /
                0.35f
            );

        drawFloatingParticles(
            window,
            targetCenter,
            elapsed,
            withAlpha(
                sf::Color(
                    120,
                    150,
                    255
                ),
                static_cast<int>(
                    220.0f *
                    fadeIn
                    )
            ),
            16,
            95.0f,
            7.0f
        );

        for (int i = 0; i < 3; i++) {
            float radius =
                35.0f +
                std::fmod(
                    elapsed *
                    40.0f +
                    static_cast<float>(i) *
                    30.0f,
                    95.0f
                );

            drawRing(
                window,
                targetCenter,
                radius,
                3.0f,
                withAlpha(
                    sf::Color(
                        105,
                        125,
                        235
                    ),
                    160 -
                    i *
                    35
                )
            );
        }

        return;
    }

    if (
        visual ==
        MoveVisual::VenomBloom
        ) {
        float bloom =
            smoothStep(
                clamp01(
                    progress /
                    0.55f
                )
            );

        for (int i = 0; i < 6; i++) {
            float angle =
                static_cast<float>(i) *
                1.0471975512f;

            sf::Vector2f petalCenter({
                targetCenter.x +
                    std::cos(angle) *
                    48.0f *
                    bloom,
                targetCenter.y +
                    std::sin(angle) *
                    35.0f *
                    bloom
                });

            drawFilledCircle(
                window,
                petalCenter,
                19.0f *
                bloom,
                withAlpha(
                    sf::Color(
                        175,
                        55,
                        205
                    ),
                    185
                )
            );
        }

        drawFloatingParticles(
            window,
            targetCenter,
            elapsed,
            sf::Color(
                205,
                80,
                225
            ),
            12,
            80.0f,
            6.0f
        );

        return;
    }

    // --------------------------------------------------
    // AQUAFIN
    // --------------------------------------------------

    if (
        visual ==
        MoveVisual::RiptideRush
        ) {
        float travel =
            clamp01(
                progress /
                impactProgress
            );

        sf::Vector2f current =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(travel)
            );

        for (int i = 0; i < 5; i++) {
            drawRing(
                window,
                {
                    current.x -
                        direction *
                        static_cast<float>(i) *
                        25.0f,
                    current.y +
                        static_cast<float>(
                            i % 2
                        ) *
                        8.0f
                },
                20.0f +
                static_cast<float>(i) *
                7.0f,
                6.0f,
                withAlpha(
                    sf::Color(
                        70,
                        190,
                        255
                    ),
                    210 -
                    i *
                    32
                )
            );
        }

        if (
            progress >
            impactProgress
            ) {
            drawSparkBurst(
                window,
                targetCenter,
                75.0f,
                sf::Color(
                    100,
                    220,
                    255
                ),
                14
            );
        }

        return;
    }

    if (
        visual ==
        MoveVisual::RazorFin
        ) {
        float travel =
            clamp01(
                progress /
                impactProgress
            );

        sf::Vector2f current =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(travel)
            );

        drawLine(
            window,
            {
                current.x -
                    direction *
                    38.0f,
                current.y +
                    45.0f
            },
            {
                current.x +
                    direction *
                    38.0f,
                current.y
            },
            9.0f,
            sf::Color(
                100,
                225,
                255
            )
        );

        drawLine(
            window,
            {
                current.x +
                    direction *
                    38.0f,
                current.y
            },
            {
                current.x -
                    direction *
                    28.0f,
                current.y -
                    42.0f
            },
            9.0f,
            sf::Color(
                100,
                225,
                255
            )
        );

        return;
    }

    if (
        visual ==
        MoveVisual::TidalLance
        ) {
        float travel =
            clamp01(
                progress /
                impactProgress
            );

        sf::Vector2f currentEnd =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(travel)
            );

        drawLine(
            window,
            attackerCenter,
            currentEnd,
            11.0f,
            sf::Color(
                70,
                185,
                255
            )
        );

        drawLine(
            window,
            attackerCenter,
            currentEnd,
            4.0f,
            sf::Color::White
        );

        if (
            progress >
            impactProgress
            ) {
            drawSparkBurst(
                window,
                targetCenter,
                80.0f,
                sf::Color(
                    85,
                    205,
                    255
                ),
                16
            );

            for (int i = 0; i < 7; i++) {
                drawFilledCircle(
                    window,
                    {
                        targetCenter.x +
                            std::sin(
                                elapsed *
                                    13.0f +
                                static_cast<float>(i)
                            ) *
                            65.0f,
                        targetCenter.y +
                            std::cos(
                                elapsed *
                                    9.0f +
                                static_cast<float>(i)
                            ) *
                            50.0f
                    },
                    6.0f,
                    sf::Color(
                        100,
                        215,
                        255
                    )
                );
            }
        }

        return;
    }

    // --------------------------------------------------
    // WILDKIN
    // --------------------------------------------------

    if (
        visual ==
        MoveVisual::BurrowRush
        ) {
        float trailProgress =
            clamp01(
                (progress - 0.22f) /
                0.45f
            );

        sf::Vector2f trailPosition =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(
                    trailProgress
                )
            );

        for (int i = 0; i < 8; i++) {
            float dustOffset =
                static_cast<float>(i) *
                18.0f;

            drawFilledCircle(
                window,
                {
                    trailPosition.x -
                        direction *
                        dustOffset,
                    410.0f +
                        std::sin(
                            elapsed *
                                15.0f +
                            static_cast<float>(i)
                        ) *
                        8.0f
                },
                8.0f +
                static_cast<float>(
                    i % 3
                    ) *
                3.0f,
                withAlpha(
                    sf::Color(
                        190,
                        155,
                        95
                    ),
                    190 -
                    i *
                    15
                )
            );
        }

        if (
            progress >
            impactProgress
            ) {
            drawSparkBurst(
                window,
                targetCenter,
                60.0f,
                sf::Color(
                    205,
                    170,
                    105
                ),
                10
            );
        }

        return;
    }

    if (
        visual ==
        MoveVisual::SandSlam
        ) {
        if (progress > 0.28f) {
            drawFloatingParticles(
                window,
                targetCenter,
                elapsed,
                sf::Color(
                    205,
                    170,
                    100
                ),
                22,
                100.0f,
                8.0f
            );

            for (int i = 0; i < 4; i++) {
                drawRing(
                    window,
                    targetCenter,
                    35.0f +
                    static_cast<float>(i) *
                    22.0f +
                    pulse(
                        elapsed +
                        static_cast<float>(i),
                        8.0f
                    ) *
                    12.0f,
                    5.0f,
                    withAlpha(
                        sf::Color(
                            210,
                            175,
                            105
                        ),
                        170 -
                        i *
                        25
                    )
                );
            }
        }

        return;
    }

    if (
        visual ==
        MoveVisual::TremorStomp
        ) {
        if (progress > 0.25f) {
            float wave =
                clamp01(
                    (progress - 0.25f) /
                    0.65f
                );

            for (int i = 0; i < 4; i++) {
                float radius =
                    wave *
                    140.0f +
                    static_cast<float>(i) *
                    28.0f;

                drawRing(
                    window,
                    {
                        targetCenter.x,
                        408.0f
                    },
                    radius,
                    5.0f,
                    withAlpha(
                        sf::Color(
                            205,
                            165,
                            95
                        ),
                        205 -
                        i *
                        40
                    )
                );
            }

            for (
                int i = -2;
                i <= 2;
                i++
                ) {
                drawLine(
                    window,
                    {
                        targetCenter.x +
                            static_cast<float>(i) *
                            30.0f,
                        405.0f
                    },
                    {
                        targetCenter.x +
                            static_cast<float>(i) *
                            45.0f,
                        435.0f
                    },
                    5.0f,
                    sf::Color(
                        120,
                        90,
                        55
                    )
                );
            }
        }

        return;
    }

    // --------------------------------------------------
    // SHADEGEIST
    // --------------------------------------------------

    if (
        visual ==
        MoveVisual::PhantomDive
        ) {
        float travel =
            clamp01(
                progress /
                impactProgress
            );

        sf::Vector2f current =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(travel)
            );

        for (int i = 1; i <= 5; i++) {
            drawRing(
                window,
                {
                    current.x -
                        direction *
                        static_cast<float>(i) *
                        30.0f,
                    current.y
                },
                18.0f +
                static_cast<float>(i) *
                5.0f,
                4.0f,
                withAlpha(
                    sf::Color(
                        145,
                        65,
                        220
                    ),
                    190 -
                    i *
                    28
                )
            );
        }

        if (
            progress >
            impactProgress
            ) {
            drawRing(
                window,
                targetCenter,
                80.0f *
                (
                    1.0f -
                    clamp01(
                        (
                            progress -
                            impactProgress
                            ) /
                        0.25f
                    )
                    ),
                10.0f,
                sf::Color(
                    165,
                    75,
                    235
                )
            );
        }

        return;
    }

    if (
        visual ==
        MoveVisual::VoidOrb
        ) {
        float travel =
            clamp01(
                progress /
                impactProgress
            );

        sf::Vector2f orbPosition =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(travel)
            );

        if (
            progress <
            impactProgress
            ) {
            float orbRadius =
                20.0f +
                pulse(
                    elapsed,
                    11.0f
                ) *
                10.0f;

            drawFilledCircle(
                window,
                orbPosition,
                orbRadius,
                sf::Color(
                    50,
                    25,
                    80
                )
            );

            drawRing(
                window,
                orbPosition,
                orbRadius +
                12.0f,
                6.0f,
                sf::Color(
                    165,
                    75,
                    230
                )
            );
        }
        else {
            float collapse =
                clamp01(
                    (
                        progress -
                        impactProgress
                        ) /
                    0.28f
                );

            drawRing(
                window,
                targetCenter,
                95.0f *
                (
                    1.0f -
                    collapse
                    ),
                12.0f,
                withAlpha(
                    sf::Color(
                        160,
                        70,
                        225
                    ),
                    230
                )
            );

            drawFilledCircle(
                window,
                targetCenter,
                55.0f *
                (
                    1.0f -
                    collapse
                    ),
                withAlpha(
                    sf::Color(
                        40,
                        20,
                        65
                    ),
                    230
                )
            );
        }

        return;
    }

    if (
        visual ==
        MoveVisual::DreadGaze
        ) {
        float gaze =
            smoothStep(
                clamp01(
                    progress /
                    0.50f
                )
            );

        drawFilledCircle(
            window,
            targetCenter,
            55.0f *
            gaze,
            withAlpha(
                sf::Color(
                    155,
                    70,
                    220
                ),
                150
            )
        );

        drawFilledCircle(
            window,
            targetCenter,
            25.0f *
            gaze,
            sf::Color(
                30,
                10,
                45
            )
        );

        drawFilledCircle(
            window,
            targetCenter,
            8.0f *
            gaze,
            sf::Color(
                235,
                210,
                255
            )
        );

        for (int i = 0; i < 3; i++) {
            drawRing(
                window,
                targetCenter,
                70.0f +
                static_cast<float>(i) *
                25.0f +
                pulse(
                    elapsed +
                    static_cast<float>(i),
                    5.0f
                ) *
                12.0f,
                4.0f,
                withAlpha(
                    sf::Color(
                        150,
                        70,
                        220
                    ),
                    170 -
                    i *
                    35
                )
            );
        }

        return;
    }

    if (
        visual ==
        MoveVisual::ShadowTalon
        ) {
        if (progress > 0.34f) {
            float slashProgress =
                clamp01(
                    (progress - 0.34f) /
                    0.30f
                );

            for (
                int i = -1;
                i <= 1;
                i++
                ) {
                drawLine(
                    window,
                    {
                        targetCenter.x -
                            direction *
                            65.0f,
                        targetCenter.y -
                            65.0f +
                            static_cast<float>(i) *
                            20.0f
                    },
                    {
                        targetCenter.x +
                            direction *
                            65.0f *
                            slashProgress,
                        targetCenter.y +
                            60.0f +
                            static_cast<float>(i) *
                            20.0f
                    },
                    9.0f,
                    withAlpha(
                        sf::Color(
                            105,
                            45,
                            165
                        ),
                        230
                    )
                );
            }
        }

        return;
    }

    // --------------------------------------------------
    // IRONFANG
    // --------------------------------------------------

    if (
        visual ==
        MoveVisual::AlloyCharge
        ) {
        float travel =
            clamp01(
                progress /
                impactProgress
            );

        sf::Vector2f current =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(travel)
            );

        for (int i = 1; i <= 6; i++) {
            drawLine(
                window,
                {
                    current.x -
                        direction *
                        static_cast<float>(i) *
                        30.0f,
                    current.y +
                        static_cast<float>(
                            i % 2
                        ) *
                        5.0f
                },
                {
                    current.x -
                        direction *
                        static_cast<float>(
                            i - 1
                        ) *
                        20.0f,
                    current.y
                },
                7.0f,
                withAlpha(
                    sf::Color(
                        200,
                        215,
                        230
                    ),
                    210 -
                    i *
                    25
                )
            );
        }

        if (
            progress >
            impactProgress
            ) {
            drawSparkBurst(
                window,
                targetCenter,
                70.0f,
                sf::Color(
                    220,
                    230,
                    240
                ),
                14
            );
        }

        return;
    }

    if (
        visual ==
        MoveVisual::HornBreaker
        ) {
        if (progress > 0.32f) {
            float strike =
                clamp01(
                    (progress - 0.32f) /
                    0.32f
                );

            sf::Vector2f tip =
                interpolatePosition(
                    {
                        targetCenter.x -
                            direction *
                            90.0f,
                        targetCenter.y +
                            45.0f
                    },
                    targetCenter,
                    strike
                );

            drawLine(
                window,
                {
                    tip.x -
                        direction *
                        70.0f,
                    tip.y +
                        35.0f
                },
                tip,
                16.0f,
                sf::Color(
                    210,
                    220,
                    230
                )
            );

            drawSparkBurst(
                window,
                tip,
                50.0f *
                strike,
                sf::Color::White,
                8
            );
        }

        return;
    }

    if (
        visual ==
        MoveVisual::SteelJab
        ) {
        float travel =
            clamp01(
                progress /
                impactProgress
            );

        sf::Vector2f end =
            interpolatePosition(
                attackerCenter,
                targetCenter,
                smoothStep(travel)
            );

        drawLine(
            window,
            attackerCenter,
            end,
            8.0f,
            sf::Color(
                205,
                220,
                235
            )
        );

        drawLine(
            window,
            attackerCenter,
            end,
            3.0f,
            sf::Color::White
        );

        if (
            progress >
            impactProgress -
            0.06f
            ) {
            drawSparkBurst(
                window,
                targetCenter,
                52.0f,
                sf::Color::White,
                10
            );
        }

        return;
    }

    if (
        visual ==
        MoveVisual::Groundbreaker
        ) {
        if (progress > 0.24f) {
            float crackProgress =
                clamp01(
                    (progress - 0.24f) /
                    0.60f
                );

            for (
                int i = -3;
                i <= 3;
                i++
                ) {
                sf::Vector2f start({
                    attackerCenter.x +
                        static_cast<float>(i) *
                        15.0f,
                    405.0f
                    });

                sf::Vector2f middle({
                    start.x +
                        direction *
                        100.0f *
                        crackProgress,
                    425.0f +
                        static_cast<float>(
                            std::abs(i)
                        ) *
                        5.0f
                    });

                sf::Vector2f end({
                    start.x +
                        direction *
                        220.0f *
                        crackProgress,
                    410.0f
                    });

                drawLine(
                    window,
                    start,
                    middle,
                    6.0f,
                    sf::Color(
                        95,
                        75,
                        55
                    )
                );

                drawLine(
                    window,
                    middle,
                    end,
                    6.0f,
                    sf::Color(
                        95,
                        75,
                        55
                    )
                );
            }

            drawRing(
                window,
                {
                    targetCenter.x,
                    410.0f
                },
                40.0f +
                crackProgress *
                150.0f,
                8.0f,
                withAlpha(
                    sf::Color(
                        190,
                        155,
                        100
                    ),
                    190
                )
            );
        }

        return;
    }

    // --------------------------------------------------
    // SKYWYRM
    // --------------------------------------------------

    if (
        visual ==
        MoveVisual::WyrmClaw
        ) {
        if (progress > 0.34f) {
            float slash =
                clamp01(
                    (progress - 0.34f) /
                    0.32f
                );

            drawLine(
                window,
                {
                    targetCenter.x -
                        75.0f,
                    targetCenter.y -
                        65.0f
                },
                {
                    targetCenter.x +
                        75.0f *
                        slash,
                    targetCenter.y +
                        65.0f
                },
                10.0f,
                sf::Color(
                    90,
                    135,
                    255
                )
            );

            drawLine(
                window,
                {
                    targetCenter.x +
                        75.0f,
                    targetCenter.y -
                        65.0f
                },
                {
                    targetCenter.x -
                        75.0f *
                        slash,
                    targetCenter.y +
                        65.0f
                },
                10.0f,
                sf::Color(
                    125,
                    165,
                    255
                )
            );
        }

        return;
    }

    if (
        visual ==
        MoveVisual::GaleSweep
        ) {
        float travel =
            clamp01(
                progress /
                impactProgress
            );

        for (int i = 0; i < 5; i++) {
            float localProgress =
                clamp01(
                    travel -
                    static_cast<float>(i) *
                    0.08f
                );

            sf::Vector2f current =
                interpolatePosition(
                    attackerCenter,
                    targetCenter,
                    smoothStep(
                        localProgress
                    )
                );

            drawLine(
                window,
                {
                    current.x -
                        direction *
                        45.0f,
                    current.y -
                        35.0f +
                        static_cast<float>(i) *
                        18.0f
                },
                {
                    current.x +
                        direction *
                        45.0f,
                    current.y -
                        15.0f +
                        static_cast<float>(i) *
                        18.0f
                },
                6.0f,
                withAlpha(
                    sf::Color(
                        205,
                        235,
                        255
                    ),
                    215 -
                    i *
                    28
                )
            );
        }

        return;
    }

    if (
        visual ==
        MoveVisual::AzureBeam
        ) {
        float chargeProgress =
            clamp01(
                progress /
                0.42f
            );

        float chargeRadius =
            12.0f +
            chargeProgress *
            35.0f;

        sf::Vector2f chargeCenter({
            attackerCenter.x +
                direction *
                45.0f,
            attackerCenter.y
            });

        drawFilledCircle(
            window,
            chargeCenter,
            chargeRadius,
            withAlpha(
                sf::Color(
                    80,
                    135,
                    255
                ),
                220
            )
        );

        drawRing(
            window,
            chargeCenter,
            chargeRadius +
            15.0f,
            7.0f,
            sf::Color(
                170,
                205,
                255
            )
        );

        if (progress > 0.42f) {
            float beamProgress =
                clamp01(
                    (progress - 0.42f) /
                    0.24f
                );

            sf::Vector2f beamEnd =
                interpolatePosition(
                    chargeCenter,
                    targetCenter,
                    beamProgress
                );

            drawLine(
                window,
                chargeCenter,
                beamEnd,
                34.0f +
                pulse(
                    elapsed,
                    28.0f
                ) *
                12.0f,
                withAlpha(
                    sf::Color(
                        75,
                        125,
                        255
                    ),
                    225
                )
            );

            drawLine(
                window,
                chargeCenter,
                beamEnd,
                13.0f,
                sf::Color(
                    220,
                    235,
                    255
                )
            );
        }

        if (
            progress >
            impactProgress
            ) {
            drawSparkBurst(
                window,
                targetCenter,
                95.0f,
                sf::Color(
                    110,
                    160,
                    255
                ),
                16
            );
        }

        return;
    }

    // --------------------------------------------------
    // Generic fallback
    // --------------------------------------------------

    float travel =
        clamp01(
            progress /
            impactProgress
        );

    sf::Vector2f effectPosition =
        interpolatePosition(
            attackerCenter,
            targetCenter,
            smoothStep(travel)
        );

    drawFilledCircle(
        window,
        effectPosition,
        18.0f,
        color
    );
}