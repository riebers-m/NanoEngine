//
// Created by HP on 18.10.2024.
//
#include "ECS/ECS.hpp"

#include <algorithm>

#include "spdlog/logger.h"

ecs::Entity::Entity(std::size_t id, Registry *registry) : m_id(id), m_registry{registry} {}
void ecs::Entity::tag(std::string const &tag) { m_registry->add_tag(*this, tag); }
bool ecs::Entity::has_tag(std::string const &tag) const { return m_registry->has_tag(*this, tag); }
void ecs::Entity::group(std::string const &group) { m_registry->add_to_group(*this, group); }
bool ecs::Entity::has_group(std::string const &group) const { return m_registry->has_group(*this, group); }
bool ecs::Entity::operator==(const Entity &other) const { return m_id == other.m_id; }
bool ecs::Entity::operator!=(const Entity &other) const { return m_id != other.m_id; }
bool ecs::Entity::operator>(const Entity &other) const { return m_id > other.m_id; }
bool ecs::Entity::operator<(const Entity &other) const { return m_id < other.m_id; }
void ecs::Entity::kill() { m_registry->remove_entity(*this); }
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

///////////////////////////////////////////////////////////////////////////////////////////
/// Registry
///////////////////////////////////////////////////////////////////////////////////////////
ecs::Registry::Registry(Logger logger) :
    m_Entitycount{}, m_to_be_added_entities{}, m_to_be_removed_entities{}, m_components{}, m_signatures{}, m_systems{},
    m_logger{logger}, m_free_ids{}, m_tag_per_entity{}, m_Entityper_tag{}, m_Entitygroups{}, m_groupe_per_entity{} {}


void ecs::Registry::clear() {
    for (const auto &pool: m_components) {
        if (pool) {
            pool->clear();
        }
    }
}

ecs::Entity ecs::Registry::create_entity() {
    size_t Entityid{};
    if (m_free_ids.empty()) {
        Entityid = m_Entitycount++;
    } else {
        Entityid = m_free_ids.front();
        m_free_ids.pop_front();
    }

    if (Entityid >= m_signatures.size()) {
        m_signatures.resize((m_signatures.size() + 1) * 2);
    }
    auto const entity = Entity{Entityid, this};
    m_to_be_added_entities.insert(entity);
    return entity;
}

void ecs::Registry::remove_entity(Entity const &entity) {
    if (auto const [_, ok] = m_to_be_removed_entities.insert(entity); !ok) {
        throw std::runtime_error(std::format("could not add entity {} to removed entities", entity.get_id()));
    }
}
void ecs::Registry::add_Entityto_system(Entity const &entity) {
    auto const Entityid = entity.get_id();
    auto const Entitysignatures = m_signatures[Entityid];

    for (auto &[index, system]: m_systems) {
        auto const &system_component_signature = system->get_signature();
        if (bool is_interest = (Entitysignatures & system_component_signature) == system_component_signature;
            is_interest) {
            system->add_entity(entity);
        }
    }
}
void ecs::Registry::remove_Entityfrom_system(Entity entity) {
    for (auto [_, system]: m_systems) {
        system->remove_entity(entity);
    }
}
void ecs::Registry::update() {
    for (auto entity: m_to_be_added_entities) {
        add_Entityto_system(entity);
    }
    m_to_be_added_entities.clear();

    for (auto entity: m_to_be_removed_entities) {
        remove_Entityfrom_system(entity);
        m_signatures[entity.get_id()].reset();

        // remove the entity from the component pools
        for (auto pool: m_components) {
            if (pool) {
                pool->remove_entity(entity.get_id());
            }
        }

        m_free_ids.push_back(entity.get_id());

        remove_tag(entity);
        remove_from_group(entity);
    }
    m_to_be_removed_entities.clear();
}
void ecs::Registry::add_tag(Entity entity, std::string const &tag) {
    m_tag_per_entity.insert({entity.get_id(), tag});
    m_Entityper_tag.insert({tag, entity});
}
bool ecs::Registry::has_tag(Entity entity, std::string const &tag) const {
    if (!m_tag_per_entity.contains(entity.get_id())) {
        return false;
    }
    return m_tag_per_entity.find(entity.get_id())->second == tag;
}
ecs::Entity ecs::Registry::get_entity(std::string const &tag) const { return m_Entityper_tag.at(tag); }
void ecs::Registry::remove_tag(Entity entity) {
    if (auto itr = m_tag_per_entity.find(entity.get_id()); itr != m_tag_per_entity.end()) {
        m_Entityper_tag.erase(itr->second);
        m_tag_per_entity.erase(itr);
    }
}
void ecs::Registry::add_to_group(Entity entity, std::string const &group) {
    m_Entitygroups.insert({group, std::set<Entity>{}});
    m_Entitygroups[group].insert(entity);
    m_groupe_per_entity.insert({entity.get_id(), group});
}
bool ecs::Registry::has_group(Entity entity, std::string const &group) const {
    if (auto const itr = m_groupe_per_entity.find(entity.get_id()); itr != m_groupe_per_entity.end()) {
        return itr->second == group;
    }
    return false;
}
std::vector<ecs::Entity> ecs::Registry::get_entities_from_group(std::string const &group) const {
    auto const group_set = m_Entitygroups.at(group);
    return std::vector<Entity>{group_set.cbegin(), group_set.cend()};
}
void ecs::Registry::remove_from_group(Entity entity) {
    if (auto group_tag = m_groupe_per_entity.find(entity.get_id()); group_tag != m_groupe_per_entity.end()) {
        if (auto group = m_Entitygroups.find(group_tag->second); group != m_Entitygroups.end()) {
            if (auto Entityin_group = group->second.find(entity); Entityin_group != group->second.end()) {
                group->second.erase(Entityin_group);
                m_groupe_per_entity.erase(group_tag);
            }
        }
    }
}
