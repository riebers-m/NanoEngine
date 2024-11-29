//
// Created by HP on 26.11.2024.
//

#include "RenderGUI.hpp"

#include <SDL_render.h>

#include <chrono>
#include "components/BoxCollider.hpp"
#include "components/Health.hpp"
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
    void RenderGUI::update(SDL_Renderer *renderer, ecs::Registry *registry, SDL_Rect const &camera) {
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        auto constexpr window_flags = ImGuiWindowFlags_AlwaysAutoResize;
        if (ImGui::Begin("Spawn Enemies", nullptr, window_flags)) {
            static int pos_x = 0;
            static int pos_y = 0;
            static int scale_x = 1;
            static int scale_y = 1;
            static int vel_x = 0;
            static int vel_y = 0;
            static int health = 100;
            static float rotation = 0.0;
            static float proj_rot = 0.0;
            static float proj_speed = 100.0;
            static int proj_repeat = 10;
            static int proj_duration = 10;
            std::array<const char *, 3> constexpr combo_items = {"Tank", "Truck", "Helicopter"};
            std::array<const char *, 3> constexpr sprite_names = {"tank-image", "truck-image", "chopper-image"};
            static int selected_index = 0;

            // Input enemy sprite texture
            if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
                if (ImGui::BeginCombo("texture id", sprite_names[selected_index], ImGuiComboFlags_WidthFitPreview)) {
                    for (int i = 0; i < sprite_names.size(); ++i) {
                        bool const is_selected = (selected_index == i);

                        if (ImGui::Selectable(sprite_names[i], is_selected)) {
                            selected_index = i;
                        }
                    }
                    ImGui::EndCombo();
                }
            }
            ImGui::Spacing();
            // Enemy Transform values
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::InputInt("x position", &pos_x);
                ImGui::InputInt("y position", &pos_y);
                ImGui::SliderInt("x scale", &scale_x, 1, 10);
                ImGui::SliderInt("y scale", &scale_y, 1, 10);
                ImGui::SliderAngle("rotation (deg)", &rotation, 0, 360);
            }
            // Enemy Rigidbody values
            ImGui::Spacing();
            if (ImGui::CollapsingHeader("Rigidbody", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::InputInt("x velocity", &vel_x);
                ImGui::InputInt("y velocity", &vel_y);
            }
            // Enemy ProjectileEmitter values
            ImGui::Spacing();
            if (ImGui::CollapsingHeader("Projectile Emitter", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::SliderAngle("angle (deg)", &proj_rot, 0, 360);
                ImGui::SliderFloat("speed (px/sec)", &proj_speed, 10, 500);
                ImGui::InputInt("repeat (sec)", &proj_repeat);
                ImGui::InputInt("duration (sec)", &proj_duration);
            }
            // Enemy Health
            ImGui::Spacing();
            if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::SliderInt("percentage", &health, 1, 100);
            }
            ImGui::Spacing();
            if (ImGui::Button("Spawn enemy")) {
                ecs::Entity new_enemy = registry->create_entity();
                new_enemy.group("enemies");
                new_enemy.add_component<component::Transform>(glm::vec2{pos_x, pos_y}, glm::vec2{scale_x, scale_y},
                                                              glm::degrees(rotation));
                new_enemy.add_component<component::RigidBody>(glm::vec2{vel_x, vel_y});
                new_enemy.add_component<component::Sprite>(sprite_names[selected_index], 32, 32, 2);
                new_enemy.add_component<component::BoxCollider>(static_cast<int>(32 * scale_x),
                                                                static_cast<int>(32 * scale_y));

                auto const proj_vel_y = std::cos(proj_rot) * proj_speed;
                auto const proj_vel_x = std::sin(proj_rot) * proj_speed;
                new_enemy.add_component<component::ProjectileEmitter>(
                        glm::vec2{proj_vel_x, proj_vel_y}, std::chrono::milliseconds{proj_repeat * 1000},
                        std::chrono::milliseconds{proj_duration * 1000}, component::ProjectileEmitter::Attitude::enemy,
                        10);
                new_enemy.add_component<component::Health>(health);

                // reset all values
                pos_x = pos_y = rotation = proj_rot = 0;
                scale_x = scale_y = 1;
                proj_duration = proj_repeat = 10;
                proj_speed = health = 100;
            }
        }
        ImGui::End();

        // ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.7);
        if (ImGui::Begin("Cursor position", nullptr, window_flags)) {
            ImGui::Text("x = %.1f, y = %.f", ImGui::GetIO().MousePos.x + camera.x,
                        ImGui::GetIO().MousePos.y + camera.y);
        }
        ImGui::End();

        ImGui::Render();
        ImGuiIO const &io = ImGui::GetIO();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
    }
} // namespace systems
