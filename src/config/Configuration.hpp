//
// Created by HP on 03.11.2024.
//

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP
#include <cstdint>

namespace engine {
    struct Configuration {
        uint16_t window_width;
        uint16_t window_height;
        uint16_t map_width;
        uint16_t map_height;
    };
} // namespace engine
#endif // CONFIGURATION_HPP
