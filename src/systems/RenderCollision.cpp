//
// Created by HP on 01.11.2024.
//

#include "RenderCollision.hpp"

#include <SDL_render.h>

#include "common/Commons.hpp"
#include "components/BoxCollider.hpp"
#include "components/Transform.hpp"

namespace systems {
    RenderCollision::RenderCollision(ecs::registry registry, Logger logger) :
        System{std::move(registry)}, m_logger{std::move(logger)} {
        // require_component<component::Transform>();
        // require_component<component::BoxCollider>();
    }

    void RenderCollision::update(SDL_Renderer *renderer, SDL_Rect const &camera) {
        if (!renderer) {
            throw std::runtime_error(std::format("render collision error: invalid renderer"));
        }
        // auto entities = get_entities();
        auto entities = m_registry->view<component::Transform, component::BoxCollider>();

        std::vector<ecs::Entityid> already_drawn;

        for (auto itr = entities.begin(); itr != entities.end(); ++itr) {
            auto const entity_a = *itr;
            // auto const transform_a = a.get_component<component::Transform>();
            // auto const collider_a = a.get_component<component::BoxCollider>();
            auto const [transform_a, collider_a] =
                    m_registry->get<component::Transform, component::BoxCollider>(entity_a);

            if (std::ranges::find(already_drawn, entity_a) != already_drawn.end()) {
                continue;
            }
            SDL_SetRenderDrawColor(renderer, 255, 240, 0, SDL_ALPHA_OPAQUE);
            SDL_Rect const rect_a = {static_cast<int>(transform_a.position.x + collider_a.offset.x - camera.x),
                                     static_cast<int>(transform_a.position.y + collider_a.offset.y - camera.y),
                                     collider_a.width, collider_a.height};
            SDL_RenderDrawRect(renderer, &rect_a);

            for (auto j = itr; j != entities.end(); ++j) {
                auto const entity_b = *j;
                if (entity_a == entity_b) {
                    continue;
                }
                auto const [transform_b, collider_b] =
                        m_registry->get<component::Transform, component::BoxCollider>(entity_b);
                // auto const transform_b = b.get_component<component::Transform>();
                // auto const collider_b = b.get_component<component::BoxCollider>();

                if (engine::Commons::AABB_collision_check(
                            transform_a.position.x + collider_a.offset.x, transform_a.position.y + collider_a.offset.y,
                            collider_a.width, collider_a.height, transform_b.position.x + collider_b.offset.x,
                            transform_b.position.y + collider_b.offset.y, collider_b.width, collider_b.height)) {
                    SDL_SetRenderDrawColor(renderer, 242, 38, 19, SDL_ALPHA_OPAQUE);
                    if (std::ranges::find(already_drawn, entity_a) == already_drawn.end()) {
                        SDL_RenderDrawRect(renderer, &rect_a);
                        already_drawn.emplace_back(entity_a);
                    }
                    if (std::ranges::find(already_drawn, entity_b) == already_drawn.end()) {
                        SDL_Rect const rect_b = {
                                static_cast<int>(transform_b.position.x + collider_b.offset.x - camera.x),
                                static_cast<int>(transform_b.position.y + collider_b.offset.y - camera.y),
                                collider_b.width, collider_b.height};
                        SDL_RenderDrawRect(renderer, &rect_b);
                        already_drawn.emplace_back(entity_b);
                    }
                }
            }
        }
    }
} // namespace systems
