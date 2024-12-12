//
// Created by HP on 20.10.2024.
//

#include "systems/Render.hpp"

#include <SDL.h>

#include <utility>

#include "assetStore/AssetStore.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "glm/ext/scalar_reciprocal.hpp"

systems::RenderSystem::RenderSystem(ecs::registry registry, Logger logger) :
    System{std::move(registry)}, m_logger{std::move(logger)} {
    // require_component<component::Transform>();
    // require_component<component::Sprite>();
}

void systems::RenderSystem::update(SDL_Renderer *renderer, engine::AssetStore const *asset_store,
                                   SDL_Rect const &camera) const {
    try {
        struct RenderableEntity {
            component::Transform transform;
            component::Sprite sprite;
        };
        std::vector<RenderableEntity> renderables;

        auto const view = m_registry->view<component::Transform, component::Sprite>();
        // for (auto const entity: get_entities()) {
        for (auto entity_id: view) {
            auto const [transform, sprite] = m_registry->get<component::Transform, component::Sprite>(entity_id);
            // auto const transform = entity.get_component<component::Transform>();
            // auto const sprite = entity.get_component<component::Sprite>();

            // bypass rendering entities if they are outside the camera view
            bool const is_entity_outside_camera_view =
                    (transform.position.x + transform.scale.x * static_cast<float>(sprite.width) <
                             static_cast<float>(camera.x) ||
                     transform.position.x > static_cast<float>(camera.x + camera.w) ||
                     transform.position.y + transform.scale.y * static_cast<float>(sprite.height) <
                             static_cast<float>(camera.y) ||
                     transform.position.y > static_cast<float>(camera.y + camera.h));
            if (is_entity_outside_camera_view && sprite.position == component::WorldPosition::free) {
                continue;
            }
            renderables.emplace_back(transform, sprite);
        }

        std::sort(renderables.begin(), renderables.end(),
                  [](RenderableEntity const &first, RenderableEntity const &second) {
                      return first.sprite.z_index < second.sprite.z_index;
                  });

        for (auto const &[transform, sprite]: renderables) {
            // Set source rect of original texture (sometime subrectangle if src is a sprite map
            SDL_Rect const src_rect = sprite.src_rect;
            // Set dest rect with x,y position to be rendered
            SDL_Rect const dest_rect = {
                    static_cast<int>(transform.position.x -
                                     (sprite.position == component::WorldPosition::fixed ? 0 : camera.x)),
                    static_cast<int>(transform.position.y -
                                     (sprite.position == component::WorldPosition::fixed ? 0 : camera.y)),
                    static_cast<int>(sprite.width * transform.scale.x),
                    static_cast<int>(sprite.height * transform.scale.y)

            };
            try {
                // NULL center point uses w/2 and h/2
                if (SDL_RenderCopyEx(renderer, asset_store->get_texture(sprite.asset_id), &src_rect, &dest_rect,
                                     transform.rotation, nullptr, sprite.flip) != 0) {
                    m_logger->error("rendering failed for asset id {}: {}", sprite.asset_id, SDL_GetError());
                }
            } catch (std::out_of_range const &e) {
                m_logger->error("rendering out of range for asset id {}: {}", sprite.asset_id, e.what());
            }
        }
    } catch (std::exception const &e) {
        m_logger->error("render system error: {}", e.what());
    }
}
