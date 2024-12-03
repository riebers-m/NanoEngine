//
// Created by HP on 10.10.2024.
//

#include "game/Game.hpp"
#include <SDL.h>
#include <thread>

#include <chrono>
#include "ECS/ECS.hpp"
#include "assetStore/AssetStore.hpp"

#include "config/Configuration.hpp"
#include "const/Const.hpp"
#include "eventBus/EventBus.hpp"
#include "events/KeyPressedEvent.hpp"

#include "common/LevelLoader.hpp"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "systems/Animation.hpp"
#include "systems/CameraMovement.hpp"
#include "systems/Collision.hpp"
#include "systems/Damage.hpp"
#include "systems/HealthBar.hpp"
#include "systems/KeyboardMovement.hpp"
#include "systems/Movement.hpp"
#include "systems/ProjectileEmitter.hpp"
#include "systems/ProjectileLifecycle.hpp"
#include "systems/Render.hpp"
#include "systems/RenderCollision.hpp"
#include "systems/RenderGUI.hpp"
#include "systems/RenderText.hpp"

using namespace std::chrono_literals;

namespace engine {
    static bool draw_collsion_bb = true;

    Game::Game(SDL_Window *window, SDL_Renderer *renderer, Logger logger, Configuration const &config) :
        m_window(window, [](SDL_Window *w) { SDL_DestroyWindow(w); }),
        m_renderer(renderer, [](SDL_Renderer *r) { SDL_DestroyRenderer(r); }), m_is_running(true), m_timer{},
        m_logger(logger), m_registry{std::make_unique<ecs::Registry>(logger)},
        m_asset_store{std::make_unique<AssetStore>(logger)}, m_event_bus{std::make_unique<events::EventBus>(m_logger)},
        m_config{config}, m_camera{0, 0, config.window_width, config.window_height} {}

    Game::~Game() {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    void Game::run() {
        setup();

        while (m_is_running) {
            auto const dt = variable_time();
            // auto const dt = fixed_time();
            process_input();
            update(dt);
            render();
        }
    }

    void Game::setup() {
        m_registry->add_system<systems::Movement>(m_logger);
        m_registry->add_system<systems::RenderSystem>(m_logger);
        m_registry->add_system<systems::Animation>(m_logger);
        m_registry->add_system<systems::Collision>(m_logger);
        m_registry->add_system<systems::RenderCollision>(m_logger);
        m_registry->add_system<systems::Damage>(m_logger);
        m_registry->add_system<systems::KeyboardMovement>(m_logger);
        m_registry->add_system<systems::CameraMovement>(m_logger);
        m_registry->add_system<systems::ProjectileEmitter>(m_logger, m_registry.get());
        m_registry->add_system<systems::ProjectileLifecycle>(m_logger);
        m_registry->add_system<systems::RenderText>(m_logger);
        m_registry->add_system<systems::HealthBar>(m_logger);
        m_registry->add_system<systems::RenderGUI>(m_logger);

        LevelLoader level_loader;
        m_config = level_loader.load_level(m_registry.get(), m_asset_store.get(), m_renderer.get(), 1);

        m_timer.start();
        // Initialize IMGUI
        ImGui::CreateContext();
        ImGui_ImplSDL2_InitForSDLRenderer(m_window.get(), m_renderer.get());
        ImGui_ImplSDLRenderer2_Init(m_renderer.get());
    }

    float Game::variable_time() {
        if (auto time_to_wait = engine::MILL_SEC_PER_FRAME - m_timer.round();
            time_to_wait > std::chrono::milliseconds(0) && time_to_wait <= engine::MILL_SEC_PER_FRAME) {
            std::this_thread::sleep_for(time_to_wait);
        }

        float const delta_time = static_cast<float>(m_timer.round().count() / 1000.0);
        m_timer.start();
        return delta_time;
    }

    float Game::fixed_time() {
        if (auto time_to_wait = engine::MILL_SEC_PER_FRAME - m_timer.round();
            time_to_wait > std::chrono::milliseconds(0) && time_to_wait <= engine::MILL_SEC_PER_FRAME) {
            std::this_thread::sleep_for(time_to_wait);
        }
        m_timer.start();
        return 1;
    }


    void Game::process_input() {
        SDL_Event evnt = {};
        while (SDL_PollEvent(&evnt)) {
            ImGui_ImplSDL2_ProcessEvent(&evnt);

            switch (evnt.type) {
                case SDL_QUIT:
                    m_is_running = false;
                    break;
                case SDL_KEYDOWN:
                    if (evnt.key.keysym.sym == SDLK_ESCAPE) {
                        m_is_running = false;
                    }
                    if (evnt.key.keysym.sym == SDLK_d) {
                        draw_collsion_bb = !draw_collsion_bb;
                    }
                    m_event_bus->emit<events::KeyPressedEvent>(evnt.key.keysym.sym);
                default:
                    break;
            }
        }
    }
    void Game::update(float dt) {
        // reset all event handlers for the current frame
        m_event_bus->clear_subscribers();

        // systems subscribe to events
        m_registry->get_system<systems::Damage>().subscribe_to_event(m_event_bus.get());
        m_registry->get_system<systems::ProjectileEmitter>().subscribe_to_event(m_event_bus.get());
        m_registry->get_system<systems::KeyboardMovement>().subscribe_to_event(m_event_bus.get());
        m_registry->get_system<systems::Movement>().subscribe_to_event(m_event_bus.get());

        m_registry->update();
        m_registry->get_system<systems::Movement>().update(dt, m_config.map_width, m_config.map_height);
        m_registry->get_system<systems::ProjectileEmitter>().update();
        m_registry->get_system<systems::Collision>().update(m_event_bus.get());
        m_registry->get_system<systems::Animation>().update(dt);
        m_registry->get_system<systems::CameraMovement>().update(m_camera, m_config);
        m_registry->get_system<systems::ProjectileLifecycle>().update();
    }
    void Game::render() const {
        SDL_SetRenderDrawColor(m_renderer.get(), 21, 21, 21, 255);
        SDL_RenderClear(m_renderer.get());
        try {
            m_registry->get_system<systems::RenderSystem>().update(m_renderer.get(), m_asset_store.get(), m_camera);
            m_registry->get_system<systems::RenderText>().update(m_renderer.get(), m_asset_store.get(), m_camera);
            m_registry->get_system<systems::HealthBar>().update(m_renderer.get(), m_asset_store.get(), m_camera);
            if (draw_collsion_bb) {
                m_registry->get_system<systems::RenderCollision>().update(m_renderer.get(), m_camera);

                ImGui_ImplSDLRenderer2_NewFrame();
                ImGui_ImplSDL2_NewFrame();
                ImGui::NewFrame();
                m_registry->get_system<systems::RenderGUI>().update(m_registry.get(), m_camera);

                // Render Debug console
                m_logger->draw("Log Console");

                ImGui::Render();
                ImGuiIO const &io = ImGui::GetIO();
                SDL_RenderSetScale(m_renderer.get(), io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
                ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_renderer.get());
            }
        } catch (std::exception const &e) {
            // TODO
            // m_logger->error("render system error: {}", e.what());
        }
        SDL_RenderPresent(m_renderer.get());
    }
} // namespace engine
