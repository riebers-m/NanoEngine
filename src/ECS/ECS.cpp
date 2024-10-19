//
// Created by HP on 18.10.2024.
//
#include "ECS/ECS.hpp"

#include <algorithm>

#include "spdlog/logger.h"

ecs::Entity::Entity(std::size_t id, Registry *registry) : m_id(id), m_registry{registry} {}
bool ecs::Entity::operator==(const Entity &other) const { return m_id == other.m_id; }
bool ecs::Entity::operator!=(const Entity &other) const { return m_id != other.m_id; }
bool ecs::Entity::operator>(const Entity &other) const { return m_id > other.m_id; }
bool ecs::Entity::operator<(const Entity &other) const { return m_id < other.m_id; }
std::size_t ecs::Entity::get_id() const { return m_id; }

std::size_t ecs::BaseComponent::next_id = 0;

///////////////////////////////////////////////////////////////////////////////////////////
/// System
///////////////////////////////////////////////////////////////////////////////////////////
ecs::System::System() : m_component_signature{}, m_entities{} {}


void ecs::System::add_entity(Entity entity) {
    auto const is_equal = [&entity](Entity const &other) { return entity.get_id() == other.get_id(); };
    if (auto result = std::ranges::find_if(m_entities, is_equal); result != m_entities.end()) {
        return;
    }

    m_entities.push_back(entity);
}

void ecs::System::remove_entity(Entity entity) {
    auto const is_equal = [&entity](Entity const &other) { return entity == other; };
    m_entities.erase(std::ranges::remove_if(m_entities, is_equal).begin(), m_entities.end());
}
ecs::System::Entities ecs::System::get_entities() const { return m_entities; }
ecs::Signature ecs::System::get_signature() const { return m_component_signature; }

ecs::Registry::Registry(Logger logger) :
    m_entity_count{}, m_to_be_added_entities{}, m_to_be_removed_entities{}, m_components{}, m_signatures{}, m_systems{},
    m_logger{logger} {}
///////////////////////////////////////////////////////////////////////////////////////////
/// Registry
///////////////////////////////////////////////////////////////////////////////////////////
ecs::Entity ecs::Registry::create_entity() {
    auto const entity_id = m_entity_count++;
    if (entity_id >= m_signatures.size()) {
        m_signatures.resize((m_signatures.size() + 1) * 2);
        m_logger->debug("resizing signatures vector to {}", m_signatures.size());
    }
    auto const entity = Entity{entity_id, this};
    m_to_be_added_entities.insert(entity);

    m_logger->debug("Entity created with id: {}", entity_id);
    return entity;
}
void ecs::Registry::remove_entity(Entity const &entity) {}
void ecs::Registry::add_entity_to_system(Entity const &entity) {
    auto const entity_id = entity.get_id();
    auto const entity_signatures = m_signatures[entity_id];

    for (auto &[index, system]: m_systems) {
        auto const &system_component_signature = system->get_signature();
        if (bool is_interest = (entity_signatures & system_component_signature) == system_component_signature;
            is_interest) {
            system->add_entity(entity);
        }
    }
}
void ecs::Registry::update() {
    for (auto entity: m_to_be_added_entities) {
        add_entity_to_system(entity);
    }
    m_to_be_added_entities.clear();
}
