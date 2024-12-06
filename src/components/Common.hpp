//
// Created by HP on 21.11.2024.
//

#pragma once

namespace component {
    enum class WorldPosition {
        free,
        fixed,
    };

    inline WorldPosition from_string(std::string const &pos) {
        if (pos == "free") {
            return WorldPosition::free;
        } else if (pos == "fixed") {
            return WorldPosition::fixed;
        }
        throw std::runtime_error(std::format("Could not convert WorldPosition string {}", pos));
    }
} // namespace component
