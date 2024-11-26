//
// Created by HP on 26.11.2024.
//

#include "RenderGUI.hpp"

#include <SDL_render.h>

#include <chrono>
#include "components/BoxColider.hpp"
#include "components/ProjectileEmitter.hpp"
#include "components/RigidBody.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

using namespace std::chrono_literals;

namespace systems {
    RenderGUI::RenderGUI(Logger logger) : m_logger{logger} {}
    void RenderGUI::update(SDL_Renderer *renderer, ecs::Registry *registry) {
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        auto constexpr window_flags = ImGuiWindowFlags_AlwaysAutoResize;
        if (ImGui::Begin("Spawn Enemies", nullptr, window_flags)) {
            static int x_pos = 0;
            static int y_pos = 0;
            ImGui::InputInt("Enemy x position", &x_pos);
            ImGui::InputInt("Enemy y position", &y_pos);

            if (ImGui::Button("Create new enemy")) {
                ecs::Entity new_enemy = registry->create_entity();
                new_enemy.group("enemies");
                new_enemy.add_component<component::Transform>(glm::vec2{x_pos, y_pos}, glm::vec2{1.0, 1.0}, 0.0);
                new_enemy.add_component<component::RigidBody>(glm::vec2{0.0, 0.0});
                new_enemy.add_component<component::Sprite>("tank-image", 32, 32, 2);
                new_enemy.add_component<component::BoxColider>(32, 32);
                new_enemy.add_component<component::ProjectileEmitter>(
                        glm::vec2{0.0, 100.0}, 1s, 3s, component::ProjectileEmitter::Attitude::enemy, 10);
            }
        }
        ImGui::End();

        ImGui::Render();
        ImGuiIO const &io = ImGui::GetIO();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
    }
} // namespace systems
