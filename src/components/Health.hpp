//
// Created by HP on 08.11.2024.
//
#pragma once

namespace component {
    struct Health {
        std::size_t health_percentage{};

        explicit Health(int health_percentage = 100) : health_percentage{static_cast<std::size_t>(health_percentage)} {}
    };
} // namespace component
