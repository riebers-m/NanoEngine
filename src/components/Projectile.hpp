//
// Created by HP on 08.11.2024.
//
#pragma once
#include <chrono>
#include "components/ProjectileEmitter.hpp"

using namespace std::chrono_literals;
namespace component {
    struct Projectile {

        ProjectileEmitter::Attitude attitude;
        std::size_t hit_percent_damage{};
        std::chrono::milliseconds duration;
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

        explicit Projectile(ProjectileEmitter::Attitude attitude = ProjectileEmitter::Attitude::enemy,
                            std::size_t hit_percent_damage = 0, std::chrono::milliseconds const &duration = 0s) :
            attitude{attitude}, hit_percent_damage{hit_percent_damage}, duration{duration},
            start_time{std::chrono::high_resolution_clock::now()} {}
    };
} // namespace component
