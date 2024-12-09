//
// Created by HP on 09.12.2024.
//

#ifndef SCRIPT_HPP
#define SCRIPT_HPP
#include <sol/function.hpp>
#include <utility>

namespace component {
    struct Script {
        sol::function func;

        explicit Script(sol::function func = sol::lua_nil) : func{std::move(func)} {}
    };
} // namespace component

#endif // SCRIPT_HPP
