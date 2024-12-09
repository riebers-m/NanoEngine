//
// Created by HP on 09.12.2024.
//

#ifndef SCRIPT_HPP
#define SCRIPT_HPP
#include <sol/function.hpp>
#include <utility>

namespace components {
    struct Script {
        sol::function func;

        explicit Script(sol::function func = sol::lua_nil) : func{std::move(func)} {}
    };
} // namespace components

#endif // SCRIPT_HPP
