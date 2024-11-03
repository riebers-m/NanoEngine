//
// Created by HP on 01.11.2024.
//

#include "RenderCollision.hpp"

#include <SDL_render.h>

#include "common/Commons.hpp"
#include "components/BoxColider.hpp"
#include "components/Transform.hpp"

namespace systems {
    RenderCollision::RenderCollision(Logger logger) : m_logger{logger} {
        require_component<component::Transform>();
        require_component<component::BoxColider>();
    }

    void RenderCollision::update(SDL_Renderer *renderer, SDL_Rect const &camera) {
        if (!renderer) {
            throw std::runtime_error(std::format("render collision error: invalid renderer"));
        }
        auto entities = get_entities();

        std::vector<size_t> already_drawn;

        for (auto itr = entities.begin(); itr != entities.end(); ++itr) {
            auto const a = *itr;
            auto const transform_a = a.get_component<component::Transform>();
            auto const collider_a = a.get_component<component::BoxColider>();

            if (std::ranges::find(already_drawn, a.get_id()) != already_drawn.end()) {
                continue;
            }
            SDL_SetRenderDrawColor(renderer, 255, 240, 0, SDL_ALPHA_OPAQUE);
            SDL_Rect const rect_a = {static_cast<int>(transform_a.position.x + collider_a.offset.x - camera.x),
                                     static_cast<int>(transform_a.position.y + collider_a.offset.y - camera.y),
                                     collider_a.width, collider_a.height};
            SDL_RenderDrawRect(renderer, &rect_a);

            for (auto j = itr + 1; j != entities.end(); ++j) {
                auto const b = *j;
                auto const transform_b = b.get_component<component::Transform>();
                auto const collider_b = b.get_component<component::BoxColider>();

                if (engine::Commons::AABB_collision_check(
                            transform_a.position.x + collider_a.offset.x, transform_a.position.y + collider_a.offset.y,
                            collider_a.width, collider_a.height, transform_b.position.x + collider_b.offset.x,
                            transform_b.position.y + collider_b.offset.y, collider_b.width, collider_b.height)) {
                    SDL_SetRenderDrawColor(renderer, 242, 38, 19, SDL_ALPHA_OPAQUE);
                    if (std::ranges::find(already_drawn, a.get_id()) == already_drawn.end()) {
                        SDL_RenderDrawRect(renderer, &rect_a);
                        already_drawn.emplace_back(a.get_id());
                    }
                    if (std::ranges::find(already_drawn, b.get_id()) == already_drawn.end()) {
                        SDL_Rect const rect_b = {
                                static_cast<int>(transform_b.position.x + collider_b.offset.x - camera.x),
                                static_cast<int>(transform_b.position.y + collider_b.offset.y - camera.y),
                                collider_b.width, collider_b.height};
                        SDL_RenderDrawRect(renderer, &rect_b);
                        already_drawn.emplace_back(b.get_id());
                    }
                }
            }
        }
    }
} // namespace systems
