//
// Created by HP on 08.11.2024.
//

#pragma once
#include <chrono>

#include "glm/vec2.hpp"
namespace component {
    struct ProjectileEmitter {
        enum class Attitude {
            enemy,
            friendly,
        };
        enum class Repeater {
            automatic,
            manual,
        };

        glm::vec2 velocity;
        std::chrono::milliseconds repeat_frequence;
        std::chrono::milliseconds duration;
        Attitude attitude;
        size_t percentDamage;
        std::chrono::time_point<std::chrono::high_resolution_clock> lastEmissionTime;
        Repeater repeater;

        explicit ProjectileEmitter(const glm::vec2 &velocity = {},
                                   const std::chrono::milliseconds &repeat_frequence = {},
                                   const std::chrono::milliseconds &duration = std::chrono::milliseconds{10000},
                                   Attitude attitude = Attitude::enemy, int percent_damage = 10,
                                   Repeater repeater = Repeater::automatic) :
            velocity{velocity}, repeat_frequence{repeat_frequence}, duration{duration}, attitude{attitude},
            percentDamage{static_cast<std::size_t>(percent_damage)},
            lastEmissionTime{std::chrono::high_resolution_clock::now()}, repeater{repeater} {}
    };
} // namespace component
