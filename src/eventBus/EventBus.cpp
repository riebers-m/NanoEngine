//
// Created by HP on 01.11.2024.
//

#include "EventBus.hpp"

#include <utility>

namespace events {
    EventBus::EventBus(Logger logger) : m_logger{std::move(logger)} {}
} // namespace events
